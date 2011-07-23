#include "stdafx.h"
#include "MultiMonitor.h"

#define IsWindowOwner(h)   (GetWindow(h,GW_OWNER) == NULL)
#define IsWindowDialog(s)  (lstrcmp(s,L"#32770") == 0)



//汎用的に使う関数群
class CMultiMonitorUtil
{
public:
	//Z-Orderの逆順にウィンドウリストを取得する.
	static std::deque<HWND> CMultiMonitorUtil::getWindows()
	{
		std::deque<HWND> windowList;
		HWND hwnd = GetTopWindow(GetDesktopWindow());
		do
		{
			windowList.push_front(hwnd);
		}
		while( (hwnd = GetNextWindow(hwnd,GW_HWNDNEXT)) !=NULL );

		return windowList;
	}
	//マウスカーソルが今いるモニタを返す
	static HMONITOR CMultiMonitorUtil::GetActiveMonitor()
	{
		//マウスポインタの位置
		POINT mousePoint;
		if ( ! ::GetCursorPos(&mousePoint) )
		{
			return NULL;
		}

		//マウスポインタがあるモニタ
		HMONITOR monitorHandle = ::MonitorFromPoint(mousePoint, MONITOR_DEFAULTTONULL);
		if (!monitorHandle)
		{
			return NULL;
		}

		return monitorHandle;

	}

	//モニターの詳細を取得する.
	static MONITORINFO GetMonitor(HMONITOR monitorHandle)
	{
		ASSERT(monitorHandle != NULL);

		//そのモニタの情報を取得.
		MONITORINFO info = { sizeof(MONITORINFO) };
		::GetMonitorInfo(monitorHandle, &info);

		return info;
	}

	//モニタにwindowが表示されているかどうか.
	static bool CMultiMonitorUtil::isDisplayWindowByMonitor(HWND hwnd, const MONITORINFO * monitorInfo) 
	{
		//windowを元のサイズを求める.
		WINDOWPLACEMENT wndPlace = { sizeof(WINDOWPLACEMENT) };
		GetWindowPlacement(hwnd, &wndPlace);

		//モニタに表示しているかどうか
		return isRectByMonitor(&wndPlace.rcNormalPosition , monitorInfo);
	}

	//モニタにwindowが表示されているかどうか.
	static bool CMultiMonitorUtil::isRectByMonitor(const RECT *rect, const MONITORINFO * monitorInfo) 
	{
		//モニタに表示しているかどうか
		RECT crossRect;
		if (! ::IntersectRect(&crossRect , &monitorInfo->rcMonitor ,rect ) )
		{
			return false;
		}
		return true;
	}
	//ウィンドウの状態を取得する.
	static MultiMonitorSaveData CMultiMonitorUtil::GetSaveInfo(HWND hwnd) 
	{
		MultiMonitorSaveData data;
		data.Handle = hwnd;
		data.IsIconic = ::IsIconic(hwnd) != FALSE;
		data.IsZoomed = ::IsZoomed(hwnd) != FALSE;

		//windowを元のサイズを求める.
		WINDOWPLACEMENT wndPlace = { sizeof(WINDOWPLACEMENT) };
		GetWindowPlacement(hwnd, &wndPlace);
		data.Rect = wndPlace.rcNormalPosition;

		return data;
	}

	//ウィンドウの位置調整.
	static RECT WindowAspectFit(const MultiMonitorSaveData* data,const MONITORINFO * monitor) 
	{
		//ターゲットのウインドウがあるモニタの情報の取得.
		HMONITOR targetMonitorHandle = ::MonitorFromWindow(data->Handle, MONITOR_DEFAULTTONULL);
		if (!targetMonitorHandle)
		{
			return data->Rect;
		}
		MONITORINFO targetMonitor = GetMonitor( targetMonitorHandle );

		//モニタからの相対位置に変換しなおす.
		RECT activeMonitorRect = monitor->rcMonitor;
		activeMonitorRect.right  -= activeMonitorRect.left;
		activeMonitorRect.bottom -= activeMonitorRect.top;
		activeMonitorRect.left   = 0;
		activeMonitorRect.top    = 0;

		RECT targetMonitorRect = targetMonitor.rcMonitor;
		targetMonitorRect.right  -= targetMonitorRect.left;
		targetMonitorRect.bottom -= targetMonitorRect.top;
		targetMonitorRect.left   = 0;
		targetMonitorRect.top    = 0;

		RECT targetWindowRect = data->Rect;
		targetWindowRect.right  -= targetMonitor.rcMonitor.left;
		targetWindowRect.bottom -= targetMonitor.rcMonitor.top;
		targetWindowRect.left   -= targetMonitor.rcMonitor.left;
		targetWindowRect.top    -= targetMonitor.rcMonitor.top;

		//アスペクト比計算
		RECT rect = targetWindowRect;
		rect.left   = targetWindowRect.left  * activeMonitorRect.right / targetMonitorRect.right;
		rect.top    = targetWindowRect.top   * activeMonitorRect.bottom   / targetMonitorRect.bottom;
		rect.right  = targetWindowRect.right * activeMonitorRect.right / targetMonitorRect.right;
		rect.bottom = targetWindowRect.bottom* activeMonitorRect.bottom   / targetMonitorRect.bottom;

		rect.left   += monitor->rcMonitor.left;
		rect.top    += monitor->rcMonitor.top;
		rect.right  += monitor->rcMonitor.left;
		rect.bottom += monitor->rcMonitor.top;

		//モニターの幅を超えていたら微調整する
		if (rect.right  > monitor->rcMonitor.right )
		{
			int hami = monitor->rcMonitor.right - rect.right;
			if (rect.left - hami < monitor->rcMonitor.left)
			{
				//左に寄せても何とかならないのでモニタの幅に合わせる.
			}
			else
			{
				//左に寄せればなんとかなる.
				rect.left -= hami;
				rect.right -= hami;
			}
		}
		if (rect.bottom  > monitor->rcMonitor.bottom )
		{
			int hami = monitor->rcMonitor.bottom - rect.bottom;
			if (rect.top - hami < monitor->rcMonitor.top)
			{
				//左に寄せても何とかならないのでモニタの幅に合わせる.
			}
			else
			{
				//左に寄せればなんとかなる.
				rect.top -= hami;
				rect.bottom -= hami;
			}
		}
		return rect;
	}
};


//マルチモニターの最小化
class CMultiMonitorMin
{
private:
	std::vector<MultiMonitorSaveData> SaveDatabase;

	//処理対象かどうか.
	bool CMultiMonitorMin::isTarget(HWND hwnd) const
	{
		//キャプション
		TCHAR title[ MAX_PATH ]= {0};
		TCHAR classname[ MAX_PATH ]= {0};
		GetWindowText( hwnd, title, sizeof(title) );
		GetClassName(  hwnd, classname, sizeof(classname) );

		//Windowスタイル
		LONG style = GetWindowLong( hwnd, GWL_STYLE );

		//最小化するべきウィンドウかどうか
		//http://blog.goo.ne.jp/masaki_goo_2006/e/6d4532acb9c09d90169752e377a97b21
		if (! IsWindowVisible(hwnd) )	return false;
		if (! (IsWindowOwner(hwnd) || IsWindowDialog(classname)) )	return false;
		if (! title[0] != '\0' )	return false;
		if (! ((style & WS_MINIMIZEBOX) && !IsIconic(hwnd)) )	return false;

		return true;
	}

	//すでに全部 最小化している？
	bool CMultiMonitorMin::isAlreadyMin(const MONITORINFO * MonitorInfo) const
	{
		auto windows = CMultiMonitorUtil::getWindows();
		for(auto i = windows.begin() ; i != windows.end() ; ++i )
		{
			HWND hwnd = *i;
			if ( ::IsWindow(hwnd) || IsIconic(hwnd) )
			{
				//モニターに表示されているかどうか？
				if (!CMultiMonitorUtil::isDisplayWindowByMonitor(hwnd,MonitorInfo))
				{
					continue;
				}
				//処理対象かどうか.
				if (isTarget(hwnd))
				{
					return false;
				}
			}
		}

		return true;
	}
public:

	//最小化実行
	void CMultiMonitorMin::Run(HMONITOR handle)
	{
		auto monitor = CMultiMonitorUtil::GetMonitor(handle);

		//保存していたデータを忘れる
		this->SaveDatabase.clear();

		//既に全て最小化されていたらなにもしない!
		if (isAlreadyMin(&monitor))
		{
			return ;
		}

		//ウィンドウのZ-order の 「逆」から追加していきます。
		//一番後ろにいるウィンドウが前です。
		auto windows = CMultiMonitorUtil::getWindows();
		for(auto i = windows.begin() ; i != windows.end() ; ++i )
		{
			auto hwnd = *i;

			//モニターに表示されているかどうか？
			if (!CMultiMonitorUtil::isDisplayWindowByMonitor(hwnd,&monitor))
			{
				continue;
			}
			//処理対象かどうか.
			if (!isTarget(hwnd))
			{
				continue;
			}

			//ウィンドウの状態を保存する
			auto data = CMultiMonitorUtil::GetSaveInfo(hwnd);
			this->SaveDatabase.push_back(data);

			//最小化
			::ShowWindow( hwnd, SW_SHOWMINNOACTIVE );
		}
	}

	//最小化解除
	void CMultiMonitorMin::Restore(HMONITOR handle)
	{
		for(auto i = this->SaveDatabase.begin() ; i != this->SaveDatabase.end() ; ++i)
		{
			if ( ::IsWindow(i->Handle) || IsIconic(i->Handle) )
			{
				::ShowWindow( i->Handle, SW_RESTORE );
			}
		}
	
		this->SaveDatabase.clear();
	}

	//最小化トグルスイッチ
	void CMultiMonitorMin::Toggle(HMONITOR handle)
	{
		if (this->SaveDatabase.size() <= 0)
		{
			//保存されているウィンドウがないので最小化していないと判断.
			Run(handle);
			return ;
		}

		auto monitor = CMultiMonitorUtil::GetMonitor(handle);

		//既に全て最小化されていたらもとに戻す
		if (isAlreadyMin(&monitor))
		{
			Restore(handle);
		}
		else
		{
			Run(handle);
		}
	}
};


//マルチモニターの集合化 (サミットカード)
class CMultiMonitorSummit
{
private:
	std::vector<MultiMonitorSaveData> SaveDatabase;

	//処理対象かどうか.
	bool CMultiMonitorSummit::isTarget(HWND hwnd) const
	{
		//キャプション
		TCHAR title[ MAX_PATH ]= {0};
		TCHAR classname[ MAX_PATH ]= {0};
		GetWindowText( hwnd, title, sizeof(title) );
		GetClassName(  hwnd, classname, sizeof(classname) );

		//Windowスタイル
		LONG style = GetWindowLong( hwnd, GWL_STYLE );

		//サミットできるウィンドウかどうか
		//http://blog.goo.ne.jp/masaki_goo_2006/e/6d4532acb9c09d90169752e377a97b21
		if (! IsWindowVisible(hwnd) )	return false;
		if (! (IsWindowOwner(hwnd) || IsWindowDialog(classname)) )	return false;
		if (! title[0] != '\0' )	return false;
//		if (! ((style & WS_MINIMIZEBOX) && !IsIconic(hwnd)) )	return false;

		return true;
	}

	//すでに全部 集めている？
	bool CMultiMonitorSummit::isAlreadySummit(const MONITORINFO * MonitorInfo) const
	{
		auto windows = CMultiMonitorUtil::getWindows();
		for(auto i = windows.begin() ; i != windows.end() ; ++i )
		{
			HWND hwnd = *i;
			if ( ::IsWindow(hwnd) || IsIconic(hwnd) )
			{
				//モニターに表示されていないかどうか
				if (CMultiMonitorUtil::isDisplayWindowByMonitor(hwnd,MonitorInfo))
				{
					//されている
					continue;
				}
				//処理対象かどうか.
				if (isTarget(hwnd))
				{
					return false;
				}
			}
		}

		return true;
	}

public:
	//サミット実行
	void CMultiMonitorSummit::Run(HMONITOR handle)
	{
		auto monitor = CMultiMonitorUtil::GetMonitor(handle);

		//保存していたデータを忘れる
		this->SaveDatabase.clear();

		//既に全て最小化されていたらなにもしない!
		if (isAlreadySummit(&monitor))
		{
			return ;
		}

		//ウィンドウのZ-order の 「逆」から追加していきます。
		//一番後ろにいるウィンドウが前です。
		auto windows = CMultiMonitorUtil::getWindows();
		for(auto i = windows.begin() ; i != windows.end() ; ++i )
		{
			auto hwnd = *i;

			//モニターに表示されていないかどうか
			if (CMultiMonitorUtil::isDisplayWindowByMonitor(hwnd,&monitor))
			{
				//されている場合は無視.
				continue;
			}
			//処理対象かどうか.
			if (!isTarget(hwnd))
			{
				continue;
			}

			//ウィンドウの状態を保存する
			auto data = CMultiMonitorUtil::GetSaveInfo(hwnd);
			this->SaveDatabase.push_back(data);

			//最小化再打開しているならもとに戻す.
			if (data.IsZoomed || data.IsIconic)
			{
				::ShowWindow( hwnd , SW_RESTORE );
			}

			//ウィンドウの幅調整
			auto rect = CMultiMonitorUtil::WindowAspectFit(&data,&monitor) ;
			SetWindowPos(
				hwnd, NULL, rect.left, rect.top 
				,rect.right - rect.left
				,rect.bottom - rect.top
				,0);

			//最小化/最大化の復元
			if (data.IsZoomed)
			{
				::ShowWindow( hwnd , SW_MAXIMIZE );
			}
			//最小化/最大化の復元
			if (data.IsIconic)
			{
				::ShowWindow( hwnd , SW_MINIMIZE );
			}
		}
	}

	//サミット解除
	void CMultiMonitorSummit::Restore(HMONITOR handle)
	{
		for(auto i = this->SaveDatabase.begin() ; i != this->SaveDatabase.end() ; ++i)
		{
			if ( ::IsWindow( i->Handle ) )
			{
				//最小化再打開しているならもとに戻す.
				if ( IsZoomed(i->Handle) || IsIconic(i->Handle) )
				{
					::ShowWindow( i->Handle , SW_RESTORE );
				}

				SetWindowPos(
					i->Handle, NULL, i->Rect.left, i->Rect.top 
					,i->Rect.right - i->Rect.left
					,i->Rect.bottom - i->Rect.top 
					,0);

				//最小化/最大化の復元
				if (i->IsZoomed)
				{
					::ShowWindow( i->Handle , SW_MAXIMIZE );
				}
				//最小化/最大化の復元
				if (i->IsIconic)
				{
					::ShowWindow( i->Handle , SW_MINIMIZE );
				}
			}
		}

		this->SaveDatabase.clear();
	}

	//サミット実施トグルスイッチ
	void CMultiMonitorSummit::Toggle(HMONITOR handle)
	{
		if (this->SaveDatabase.size() <= 0)
		{
			//保存されているウィンドウがないので最小化していないと判断.
			Run(handle);
			return ;
		}

		auto monitor = CMultiMonitorUtil::GetMonitor(handle);

		if (isAlreadySummit(&monitor))
		{
			Restore(handle);
		}
		else
		{
			Run(handle);
		}
	}
};



//マルチモニター外部向けインターフェース
CMultiMonitor::CMultiMonitor(void)
{
}


CMultiMonitor::~CMultiMonitor(void)
{
}

//最小化実行
void CMultiMonitor::MinRun(void)
{
	auto handle = CMultiMonitorUtil::GetActiveMonitor();

	auto v = this->MinMap.find(handle);
	if ( v == this->MinMap.end() )
	{
		this->MinMap[handle] = new CMultiMonitorMin;
	}
	this->MinMap[handle]->Run(handle);
}

//最小化解除
void CMultiMonitor::MinRestore(void)
{
	auto handle = CMultiMonitorUtil::GetActiveMonitor();

	auto v = this->MinMap.find(handle);
	if ( v == this->MinMap.end() )
	{
		this->MinMap[handle] = new CMultiMonitorMin;
	}
	this->MinMap[handle]->Restore(handle);

}

//最小化トグルスイッチ
void CMultiMonitor::MinToggle(void)
{
	auto handle = CMultiMonitorUtil::GetActiveMonitor();

	auto v = this->MinMap.find(handle);
	if ( v == this->MinMap.end() )
	{
		this->MinMap[handle] = new CMultiMonitorMin;
	}
	this->MinMap[handle]->Toggle(handle);
}

//サミット実施
void CMultiMonitor::SummitRun(void)
{
	auto handle = CMultiMonitorUtil::GetActiveMonitor();

	auto v = this->SummitMap.find(handle);
	if ( v == this->SummitMap.end() )
	{
		this->SummitMap[handle] = new CMultiMonitorSummit;
	}
	this->SummitMap[handle]->Run(handle);
}

//サミット解除
void CMultiMonitor::SummitRestore(void)
{
	auto handle = CMultiMonitorUtil::GetActiveMonitor();

	auto v = this->SummitMap.find(handle);
	if ( v == this->SummitMap.end() )
	{
		this->SummitMap[handle] = new CMultiMonitorSummit;
	}
	this->SummitMap[handle]->Restore(handle);
}

//サミット実施トグルスイッチ
void CMultiMonitor::SummitToggle(void)
{
	auto handle = CMultiMonitorUtil::GetActiveMonitor();

	auto v = this->SummitMap.find(handle);
	if ( v == this->SummitMap.end() )
	{
		this->SummitMap[handle] = new CMultiMonitorSummit;
	}
	this->SummitMap[handle]->Toggle(handle);
}

