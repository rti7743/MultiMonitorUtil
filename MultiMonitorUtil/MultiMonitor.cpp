#include "stdafx.h"
#include "MultiMonitor.h"

#define IsWindowOwner(h)   (GetWindow(h,GW_OWNER) == NULL)
#define IsWindowDialog(s)  (lstrcmp(s,L"#32770") == 0)



//�ėp�I�Ɏg���֐��Q
class CMultiMonitorUtil
{
public:
	//Z-Order�̋t���ɃE�B���h�E���X�g���擾����.
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
	//�}�E�X�J�[�\���������郂�j�^��Ԃ�
	static HMONITOR CMultiMonitorUtil::GetActiveMonitor()
	{
		//�}�E�X�|�C���^�̈ʒu
		POINT mousePoint;
		if ( ! ::GetCursorPos(&mousePoint) )
		{
			return NULL;
		}

		//�}�E�X�|�C���^�����郂�j�^
		HMONITOR monitorHandle = ::MonitorFromPoint(mousePoint, MONITOR_DEFAULTTONULL);
		if (!monitorHandle)
		{
			return NULL;
		}

		return monitorHandle;

	}

	//���j�^�[�̏ڍׂ��擾����.
	static MONITORINFO GetMonitor(HMONITOR monitorHandle)
	{
		ASSERT(monitorHandle != NULL);

		//���̃��j�^�̏����擾.
		MONITORINFO info = { sizeof(MONITORINFO) };
		::GetMonitorInfo(monitorHandle, &info);

		return info;
	}

	//���j�^��window���\������Ă��邩�ǂ���.
	static bool CMultiMonitorUtil::isDisplayWindowByMonitor(HWND hwnd, const MONITORINFO * monitorInfo) 
	{
		//window�����̃T�C�Y�����߂�.
		WINDOWPLACEMENT wndPlace = { sizeof(WINDOWPLACEMENT) };
		GetWindowPlacement(hwnd, &wndPlace);

		//���j�^�ɕ\�����Ă��邩�ǂ���
		return isRectByMonitor(&wndPlace.rcNormalPosition , monitorInfo);
	}

	//���j�^��window���\������Ă��邩�ǂ���.
	static bool CMultiMonitorUtil::isRectByMonitor(const RECT *rect, const MONITORINFO * monitorInfo) 
	{
		//���j�^�ɕ\�����Ă��邩�ǂ���
		RECT crossRect;
		if (! ::IntersectRect(&crossRect , &monitorInfo->rcMonitor ,rect ) )
		{
			return false;
		}
		return true;
	}
	//�E�B���h�E�̏�Ԃ��擾����.
	static MultiMonitorSaveData CMultiMonitorUtil::GetSaveInfo(HWND hwnd) 
	{
		MultiMonitorSaveData data;
		data.Handle = hwnd;
		data.IsIconic = ::IsIconic(hwnd) != FALSE;
		data.IsZoomed = ::IsZoomed(hwnd) != FALSE;

		//window�����̃T�C�Y�����߂�.
		WINDOWPLACEMENT wndPlace = { sizeof(WINDOWPLACEMENT) };
		GetWindowPlacement(hwnd, &wndPlace);
		data.Rect = wndPlace.rcNormalPosition;

		return data;
	}

	//�E�B���h�E�̈ʒu����.
	static RECT WindowAspectFit(const MultiMonitorSaveData* data,const MONITORINFO * monitor) 
	{
		//�^�[�Q�b�g�̃E�C���h�E�����郂�j�^�̏��̎擾.
		HMONITOR targetMonitorHandle = ::MonitorFromWindow(data->Handle, MONITOR_DEFAULTTONULL);
		if (!targetMonitorHandle)
		{
			return data->Rect;
		}
		MONITORINFO targetMonitor = GetMonitor( targetMonitorHandle );

		//���j�^����̑��Έʒu�ɕϊ����Ȃ���.
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

		//�A�X�y�N�g��v�Z
		RECT rect = targetWindowRect;
		rect.left   = targetWindowRect.left  * activeMonitorRect.right / targetMonitorRect.right;
		rect.top    = targetWindowRect.top   * activeMonitorRect.bottom   / targetMonitorRect.bottom;
		rect.right  = targetWindowRect.right * activeMonitorRect.right / targetMonitorRect.right;
		rect.bottom = targetWindowRect.bottom* activeMonitorRect.bottom   / targetMonitorRect.bottom;

		rect.left   += monitor->rcMonitor.left;
		rect.top    += monitor->rcMonitor.top;
		rect.right  += monitor->rcMonitor.left;
		rect.bottom += monitor->rcMonitor.top;

		//���j�^�[�̕��𒴂��Ă��������������
		if (rect.right  > monitor->rcMonitor.right )
		{
			int hami = monitor->rcMonitor.right - rect.right;
			if (rect.left - hami < monitor->rcMonitor.left)
			{
				//���Ɋ񂹂Ă����Ƃ��Ȃ�Ȃ��̂Ń��j�^�̕��ɍ��킹��.
			}
			else
			{
				//���Ɋ񂹂�΂Ȃ�Ƃ��Ȃ�.
				rect.left -= hami;
				rect.right -= hami;
			}
		}
		if (rect.bottom  > monitor->rcMonitor.bottom )
		{
			int hami = monitor->rcMonitor.bottom - rect.bottom;
			if (rect.top - hami < monitor->rcMonitor.top)
			{
				//���Ɋ񂹂Ă����Ƃ��Ȃ�Ȃ��̂Ń��j�^�̕��ɍ��킹��.
			}
			else
			{
				//���Ɋ񂹂�΂Ȃ�Ƃ��Ȃ�.
				rect.top -= hami;
				rect.bottom -= hami;
			}
		}
		return rect;
	}
};


//�}���`���j�^�[�̍ŏ���
class CMultiMonitorMin
{
private:
	std::vector<MultiMonitorSaveData> SaveDatabase;

	//�����Ώۂ��ǂ���.
	bool CMultiMonitorMin::isTarget(HWND hwnd) const
	{
		//�L���v�V����
		TCHAR title[ MAX_PATH ]= {0};
		TCHAR classname[ MAX_PATH ]= {0};
		GetWindowText( hwnd, title, sizeof(title) );
		GetClassName(  hwnd, classname, sizeof(classname) );

		//Window�X�^�C��
		LONG style = GetWindowLong( hwnd, GWL_STYLE );

		//�ŏ�������ׂ��E�B���h�E���ǂ���
		//http://blog.goo.ne.jp/masaki_goo_2006/e/6d4532acb9c09d90169752e377a97b21
		if (! IsWindowVisible(hwnd) )	return false;
		if (! (IsWindowOwner(hwnd) || IsWindowDialog(classname)) )	return false;
		if (! title[0] != '\0' )	return false;
		if (! ((style & WS_MINIMIZEBOX) && !IsIconic(hwnd)) )	return false;

		return true;
	}

	//���łɑS�� �ŏ������Ă���H
	bool CMultiMonitorMin::isAlreadyMin(const MONITORINFO * MonitorInfo) const
	{
		auto windows = CMultiMonitorUtil::getWindows();
		for(auto i = windows.begin() ; i != windows.end() ; ++i )
		{
			HWND hwnd = *i;
			if ( ::IsWindow(hwnd) || IsIconic(hwnd) )
			{
				//���j�^�[�ɕ\������Ă��邩�ǂ����H
				if (!CMultiMonitorUtil::isDisplayWindowByMonitor(hwnd,MonitorInfo))
				{
					continue;
				}
				//�����Ώۂ��ǂ���.
				if (isTarget(hwnd))
				{
					return false;
				}
			}
		}

		return true;
	}
public:

	//�ŏ������s
	void CMultiMonitorMin::Run(HMONITOR handle)
	{
		auto monitor = CMultiMonitorUtil::GetMonitor(handle);

		//�ۑ����Ă����f�[�^��Y���
		this->SaveDatabase.clear();

		//���ɑS�čŏ�������Ă�����Ȃɂ����Ȃ�!
		if (isAlreadyMin(&monitor))
		{
			return ;
		}

		//�E�B���h�E��Z-order �� �u�t�v����ǉ����Ă����܂��B
		//��Ԍ��ɂ���E�B���h�E���O�ł��B
		auto windows = CMultiMonitorUtil::getWindows();
		for(auto i = windows.begin() ; i != windows.end() ; ++i )
		{
			auto hwnd = *i;

			//���j�^�[�ɕ\������Ă��邩�ǂ����H
			if (!CMultiMonitorUtil::isDisplayWindowByMonitor(hwnd,&monitor))
			{
				continue;
			}
			//�����Ώۂ��ǂ���.
			if (!isTarget(hwnd))
			{
				continue;
			}

			//�E�B���h�E�̏�Ԃ�ۑ�����
			auto data = CMultiMonitorUtil::GetSaveInfo(hwnd);
			this->SaveDatabase.push_back(data);

			//�ŏ���
			::ShowWindow( hwnd, SW_SHOWMINNOACTIVE );
		}
	}

	//�ŏ�������
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

	//�ŏ����g�O���X�C�b�`
	void CMultiMonitorMin::Toggle(HMONITOR handle)
	{
		if (this->SaveDatabase.size() <= 0)
		{
			//�ۑ�����Ă���E�B���h�E���Ȃ��̂ōŏ������Ă��Ȃ��Ɣ��f.
			Run(handle);
			return ;
		}

		auto monitor = CMultiMonitorUtil::GetMonitor(handle);

		//���ɑS�čŏ�������Ă�������Ƃɖ߂�
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


//�}���`���j�^�[�̏W���� (�T�~�b�g�J�[�h)
class CMultiMonitorSummit
{
private:
	std::vector<MultiMonitorSaveData> SaveDatabase;

	//�����Ώۂ��ǂ���.
	bool CMultiMonitorSummit::isTarget(HWND hwnd) const
	{
		//�L���v�V����
		TCHAR title[ MAX_PATH ]= {0};
		TCHAR classname[ MAX_PATH ]= {0};
		GetWindowText( hwnd, title, sizeof(title) );
		GetClassName(  hwnd, classname, sizeof(classname) );

		//Window�X�^�C��
		LONG style = GetWindowLong( hwnd, GWL_STYLE );

		//�T�~�b�g�ł���E�B���h�E���ǂ���
		//http://blog.goo.ne.jp/masaki_goo_2006/e/6d4532acb9c09d90169752e377a97b21
		if (! IsWindowVisible(hwnd) )	return false;
		if (! (IsWindowOwner(hwnd) || IsWindowDialog(classname)) )	return false;
		if (! title[0] != '\0' )	return false;
//		if (! ((style & WS_MINIMIZEBOX) && !IsIconic(hwnd)) )	return false;

		return true;
	}

	//���łɑS�� �W�߂Ă���H
	bool CMultiMonitorSummit::isAlreadySummit(const MONITORINFO * MonitorInfo) const
	{
		auto windows = CMultiMonitorUtil::getWindows();
		for(auto i = windows.begin() ; i != windows.end() ; ++i )
		{
			HWND hwnd = *i;
			if ( ::IsWindow(hwnd) || IsIconic(hwnd) )
			{
				//���j�^�[�ɕ\������Ă��Ȃ����ǂ���
				if (CMultiMonitorUtil::isDisplayWindowByMonitor(hwnd,MonitorInfo))
				{
					//����Ă���
					continue;
				}
				//�����Ώۂ��ǂ���.
				if (isTarget(hwnd))
				{
					return false;
				}
			}
		}

		return true;
	}

public:
	//�T�~�b�g���s
	void CMultiMonitorSummit::Run(HMONITOR handle)
	{
		auto monitor = CMultiMonitorUtil::GetMonitor(handle);

		//�ۑ����Ă����f�[�^��Y���
		this->SaveDatabase.clear();

		//���ɑS�čŏ�������Ă�����Ȃɂ����Ȃ�!
		if (isAlreadySummit(&monitor))
		{
			return ;
		}

		//�E�B���h�E��Z-order �� �u�t�v����ǉ����Ă����܂��B
		//��Ԍ��ɂ���E�B���h�E���O�ł��B
		auto windows = CMultiMonitorUtil::getWindows();
		for(auto i = windows.begin() ; i != windows.end() ; ++i )
		{
			auto hwnd = *i;

			//���j�^�[�ɕ\������Ă��Ȃ����ǂ���
			if (CMultiMonitorUtil::isDisplayWindowByMonitor(hwnd,&monitor))
			{
				//����Ă���ꍇ�͖���.
				continue;
			}
			//�����Ώۂ��ǂ���.
			if (!isTarget(hwnd))
			{
				continue;
			}

			//�E�B���h�E�̏�Ԃ�ۑ�����
			auto data = CMultiMonitorUtil::GetSaveInfo(hwnd);
			this->SaveDatabase.push_back(data);

			//�ŏ����đŊJ���Ă���Ȃ���Ƃɖ߂�.
			if (data.IsZoomed || data.IsIconic)
			{
				::ShowWindow( hwnd , SW_RESTORE );
			}

			//�E�B���h�E�̕�����
			auto rect = CMultiMonitorUtil::WindowAspectFit(&data,&monitor) ;
			SetWindowPos(
				hwnd, NULL, rect.left, rect.top 
				,rect.right - rect.left
				,rect.bottom - rect.top
				,0);

			//�ŏ���/�ő剻�̕���
			if (data.IsZoomed)
			{
				::ShowWindow( hwnd , SW_MAXIMIZE );
			}
			//�ŏ���/�ő剻�̕���
			if (data.IsIconic)
			{
				::ShowWindow( hwnd , SW_MINIMIZE );
			}
		}
	}

	//�T�~�b�g����
	void CMultiMonitorSummit::Restore(HMONITOR handle)
	{
		for(auto i = this->SaveDatabase.begin() ; i != this->SaveDatabase.end() ; ++i)
		{
			if ( ::IsWindow( i->Handle ) )
			{
				//�ŏ����đŊJ���Ă���Ȃ���Ƃɖ߂�.
				if ( IsZoomed(i->Handle) || IsIconic(i->Handle) )
				{
					::ShowWindow( i->Handle , SW_RESTORE );
				}

				SetWindowPos(
					i->Handle, NULL, i->Rect.left, i->Rect.top 
					,i->Rect.right - i->Rect.left
					,i->Rect.bottom - i->Rect.top 
					,0);

				//�ŏ���/�ő剻�̕���
				if (i->IsZoomed)
				{
					::ShowWindow( i->Handle , SW_MAXIMIZE );
				}
				//�ŏ���/�ő剻�̕���
				if (i->IsIconic)
				{
					::ShowWindow( i->Handle , SW_MINIMIZE );
				}
			}
		}

		this->SaveDatabase.clear();
	}

	//�T�~�b�g���{�g�O���X�C�b�`
	void CMultiMonitorSummit::Toggle(HMONITOR handle)
	{
		if (this->SaveDatabase.size() <= 0)
		{
			//�ۑ�����Ă���E�B���h�E���Ȃ��̂ōŏ������Ă��Ȃ��Ɣ��f.
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



//�}���`���j�^�[�O�������C���^�[�t�F�[�X
CMultiMonitor::CMultiMonitor(void)
{
}


CMultiMonitor::~CMultiMonitor(void)
{
}

//�ŏ������s
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

//�ŏ�������
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

//�ŏ����g�O���X�C�b�`
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

//�T�~�b�g���{
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

//�T�~�b�g����
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

//�T�~�b�g���{�g�O���X�C�b�`
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

