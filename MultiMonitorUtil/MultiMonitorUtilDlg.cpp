
// MultiMonitorUtilDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "MultiMonitorUtil.h"
#include "MultiMonitorUtilDlg.h"
#include "afxdialogex.h"
#include "NiseMainDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//タスクトレイがクリックされたとき
const int WM_USER_TASKTRAY_CLICK = WM_USER+100;
const int WM_TASKBAR_CREATED = RegisterWindowMessage(_T("TaskbarCreated"));

// CMultiMonitorUtilDlg ダイアログ




CMultiMonitorUtilDlg::CMultiMonitorUtilDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMultiMonitorUtilDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMultiMonitorUtilDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMultiMonitorUtilDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_USER_TASKTRAY_CLICK, &CMultiMonitorUtilDlg::OnTaskTrayClick)
	ON_REGISTERED_MESSAGE(WM_TASKBAR_CREATED, OnTaskbarCreated)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_ACTIVATE()
	ON_COMMAND(IDM_SETUP, OnMenuCommandDialog)
	ON_COMMAND(IDM_QUIT, OnMenuCommandExit)
	ON_WM_HOTKEY()
END_MESSAGE_MAP()


// CMultiMonitorUtilDlg メッセージ ハンドラー

BOOL CMultiMonitorUtilDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	// TODO: 初期化をここに追加します。
	ModifyStyleEx(WS_EX_APPWINDOW, 0);
	ShowWindow( SW_HIDE );

	//どうしてもダイアログを消せなかったので、、これで逃げてみる。
	MoveWindow(1,1,1,1,TRUE);

	//コンフィグを読み込む
	LoadConfig();
	//設定を有効にする.
	SetHotKey();

	//通知領域にアイコンなどを追加.
	bool ret = this->AddNotifyIcon();
	if (!ret)
	{
		return FALSE;
	}

	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}



// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void CMultiMonitorUtilDlg::OnPaint()
{
	//非表示
	ModifyStyleEx(WS_EX_APPWINDOW, 0);
	ShowWindow( SW_HIDE );

	// タスクバーに表示しない
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// クライアントの四角形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンの描画
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR CMultiMonitorUtilDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMultiMonitorUtilDlg::PostNcDestroy()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	//アイコンを消す.
	this->DelNotifyIcon();

	CDialogEx::PostNcDestroy();
}




//通知領域に追加
bool CMultiMonitorUtilDlg::AddNotifyIcon()
{
	//タスクトレイにアイコンを出す
	this->NotifyIcon.cbSize = sizeof(NOTIFYICONDATA);
	this->NotifyIcon.uID = 0;
	this->NotifyIcon.hWnd = m_hWnd;
	this->NotifyIcon.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	this->NotifyIcon.hIcon = AfxGetApp()->LoadIcon( IDR_MAINFRAME );
	this->NotifyIcon.uCallbackMessage = WM_USER_TASKTRAY_CLICK;
	lstrcpy( this->NotifyIcon.szTip, L"MultiMonitorUtil by rti" );

	// アイコンを登録できるかエラーになるまで繰り返す
	while (true)
	{
		if (Shell_NotifyIcon(NIM_ADD, &this->NotifyIcon))
		{
			// 登録できたら終わり
			break;
		}
		else
		{
			// タイムアウトかどうか調べる
			if (::GetLastError() != ERROR_TIMEOUT)
			{
				// アイコン登録エラー
				return false;
			}

			// 登録できていないことを確認する
			if (Shell_NotifyIcon(NIM_MODIFY, &this->NotifyIcon))
			{
				// 登録できていた
				break;
			}
			else
			{
				// 登録できていなかった
				::Sleep(1000);
			}
		}
	}
	return true;
}

//タスクトレイからアイコンを消す
bool CMultiMonitorUtilDlg::DelNotifyIcon()
{
	::Shell_NotifyIcon( NIM_DELETE, &this->NotifyIcon );
	return true;
}

//タスクトレイのアイコンがクリックされたとき
afx_msg LRESULT CMultiMonitorUtilDlg::OnTaskTrayClick(WPARAM wParam, LPARAM lParam)
{
	// カーソルの現在位置を取得
	POINT point;
	::GetCursorPos(&point);

	switch(lParam) 
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
		SetForegroundWindow();	
		CMenu menu;
		VERIFY(menu.LoadMenu(IDR_MENU1));
		CMenu* pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
		PostMessage(WM_NULL);
		break;
	}
	return 0;
}

//シェルが死んだ時に飛んでくるメッセージ
afx_msg LRESULT CMultiMonitorUtilDlg::OnTaskbarCreated(WPARAM wParam, LPARAM lParam)
{
	this->AddNotifyIcon();
	return 0;
}


//設定ダイアログを開く
void CMultiMonitorUtilDlg::OnMenuCommandDialog() 
{
	// ダイアログを開く。
	CNiseMainDialog mainDialog;

	//コンフィグを渡す
	mainDialog.Config = this->Config;

	mainDialog.DoModal();

	//コンフィグを貰う.
	this->Config = mainDialog.Config;

	//設定を有効にする.
	SetHotKey();
	//コンフィグを書きこむ
	SaveConfig();
}

//終了
void CMultiMonitorUtilDlg::OnMenuCommandExit() 
{
	// 終了が選ばれたらメインウィンドウを閉じてプログラムを終了する。
	PostMessage(WM_CLOSE);
}


void CMultiMonitorUtilDlg::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	lpwndpos->flags &= ~SWP_SHOWWINDOW;
	CDialogEx::OnWindowPosChanged(lpwndpos);

	// TODO: ここにメッセージ ハンドラー コードを追加します。
}


//設定を読み込む
void CMultiMonitorUtilDlg::LoadConfig()
{
	for(int id = HOTKEY_START ; id < HOTKEY_END ; id ++)
	{
		UINT key = 0;
		bool isWin = false;
		bool isAlt = false;
		bool isCtrl = false;
		bool inShift = false;

		CString keyname;
		keyname.Format(L"ID_%d",id);

		key = static_cast<UINT>( AfxGetApp()->GetProfileInt(keyname,L"KEY" ,0) );
		isWin = static_cast<bool>( AfxGetApp()->GetProfileInt(keyname,L"Modifiers_Win" ,0) != 0);
		isAlt = static_cast<bool>( AfxGetApp()->GetProfileInt(keyname,L"Modifiers_Alt" ,0) != 0);
		isCtrl = static_cast<bool>( AfxGetApp()->GetProfileInt(keyname,L"Modifiers_Ctrl" ,0) != 0);
		inShift = static_cast<bool>( AfxGetApp()->GetProfileInt(keyname,L"Modifiers_Shift" ,0) != 0);

		this->Config.KeyConfigs[id].key = key;
		this->Config.KeyConfigs[id].isWin = isWin;
		this->Config.KeyConfigs[id].isAlt = isAlt;
		this->Config.KeyConfigs[id].isCtrl = isCtrl;
		this->Config.KeyConfigs[id].inShift = inShift;

	}
}

//設定を保存する
void CMultiMonitorUtilDlg::SaveConfig()
{
	for(int id = HOTKEY_START ; id < HOTKEY_END ; id ++)
	{
		CString keyname;
		keyname.Format(L"ID_%d",id);

		AfxGetApp()->WriteProfileInt(keyname,L"KEY" , static_cast<int>(this->Config.KeyConfigs[id].key)  );
		AfxGetApp()->WriteProfileInt(keyname,L"Modifiers_Win" , static_cast<int>(this->Config.KeyConfigs[id].isWin)  );
		AfxGetApp()->WriteProfileInt(keyname,L"Modifiers_Alt" , static_cast<int>(this->Config.KeyConfigs[id].isAlt)  );
		AfxGetApp()->WriteProfileInt(keyname,L"Modifiers_Ctrl" , static_cast<int>(this->Config.KeyConfigs[id].isCtrl)  );
		AfxGetApp()->WriteProfileInt(keyname,L"Modifiers_Shift" , static_cast<int>(this->Config.KeyConfigs[id].inShift)  );
	}
}

//設定を有効にする
void CMultiMonitorUtilDlg::SetHotKey()
{
	int id;
	for(id = HOTKEY_START ; id < HOTKEY_END ; id ++)
	{
		UnregisterHotKey( this->m_hWnd  , id );
	}

	for(id = HOTKEY_START ; id < HOTKEY_END ; id ++)
	{
		if ( this->Config.KeyConfigs[id].key != 0 )
		{
			RegisterHotKey( this->m_hWnd  , id 
				,     (this->Config.KeyConfigs[id].isWin ? MOD_WIN : 0)  
					| (this->Config.KeyConfigs[id].isAlt ? MOD_ALT : 0)  
					| (this->Config.KeyConfigs[id].isCtrl ? MOD_CONTROL : 0)  
					| (this->Config.KeyConfigs[id].inShift ? MOD_SHIFT : 0) 
				,  this->Config.KeyConfigs[id].key);
		}
	}
}



void CMultiMonitorUtilDlg::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
	switch(nHotKeyId)
	{
	case HOTKEY_MIN_RUN:
		this->MonitorUtil.MinRun();
		break;
	case HOTKEY_MIN_RESTORE:
		this->MonitorUtil.MinRestore();
		break;
	case HOTKEY_MIN_TOGGLE:
		this->MonitorUtil.MinToggle();
		break;
	case HOTKEY_SUMMIT_RUN:
		this->MonitorUtil.SummitRun();
		break;
	case HOTKEY_SUMMIT_RESTORE:
		this->MonitorUtil.SummitRestore();
		break;
	case HOTKEY_SUMMIT_TOGGLE:
		this->MonitorUtil.SummitToggle();
		break;
	}

	CDialogEx::OnHotKey(nHotKeyId, nKey1, nKey2);
}
