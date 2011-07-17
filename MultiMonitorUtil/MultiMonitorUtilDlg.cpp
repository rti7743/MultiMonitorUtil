
// MultiMonitorUtilDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "MultiMonitorUtil.h"
#include "MultiMonitorUtilDlg.h"
#include "afxdialogex.h"
#include "NiseMainDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//�^�X�N�g���C���N���b�N���ꂽ�Ƃ�
const int WM_USER_TASKTRAY_CLICK = WM_USER+100;
const int WM_TASKBAR_CREATED = RegisterWindowMessage(_T("TaskbarCreated"));

// CMultiMonitorUtilDlg �_�C�A���O




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


// CMultiMonitorUtilDlg ���b�Z�[�W �n���h���[

BOOL CMultiMonitorUtilDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���̃_�C�A���O�̃A�C�R����ݒ肵�܂��B�A�v���P�[�V�����̃��C�� �E�B���h�E���_�C�A���O�łȂ��ꍇ�A
	//  Framework �́A���̐ݒ�������I�ɍs���܂��B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R���̐ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R���̐ݒ�

	// TODO: �������������ɒǉ����܂��B
	ModifyStyleEx(WS_EX_APPWINDOW, 0);
	ShowWindow( SW_HIDE );

	//�ǂ����Ă��_�C�A���O�������Ȃ������̂ŁA�A����œ����Ă݂�B
	MoveWindow(1,1,1,1,TRUE);

	//�R���t�B�O��ǂݍ���
	LoadConfig();
	//�ݒ��L���ɂ���.
	SetHotKey();

	//�ʒm�̈�ɃA�C�R���Ȃǂ�ǉ�.
	bool ret = this->AddNotifyIcon();
	if (!ret)
	{
		return FALSE;
	}

	return TRUE;  // �t�H�[�J�X���R���g���[���ɐݒ肵���ꍇ�������ATRUE ��Ԃ��܂��B
}



// �_�C�A���O�ɍŏ����{�^����ǉ�����ꍇ�A�A�C�R����`�悷�邽�߂�
//  ���̃R�[�h���K�v�ł��B�h�L�������g/�r���[ ���f�����g�� MFC �A�v���P�[�V�����̏ꍇ�A
//  ����́AFramework �ɂ���Ď����I�ɐݒ肳��܂��B

void CMultiMonitorUtilDlg::OnPaint()
{
	//��\��
	ModifyStyleEx(WS_EX_APPWINDOW, 0);
	ShowWindow( SW_HIDE );

	// �^�X�N�o�[�ɕ\�����Ȃ�
	if (IsIconic())
	{
		CPaintDC dc(this); // �`��̃f�o�C�X �R���e�L�X�g

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// �N���C�A���g�̎l�p�`�̈���̒���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �A�C�R���̕`��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ���[�U�[���ŏ��������E�B���h�E���h���b�O���Ă���Ƃ��ɕ\������J�[�\�����擾���邽�߂ɁA
//  �V�X�e�������̊֐����Ăяo���܂��B
HCURSOR CMultiMonitorUtilDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMultiMonitorUtilDlg::PostNcDestroy()
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B

	//�A�C�R��������.
	this->DelNotifyIcon();

	CDialogEx::PostNcDestroy();
}




//�ʒm�̈�ɒǉ�
bool CMultiMonitorUtilDlg::AddNotifyIcon()
{
	//�^�X�N�g���C�ɃA�C�R�����o��
	this->NotifyIcon.cbSize = sizeof(NOTIFYICONDATA);
	this->NotifyIcon.uID = 0;
	this->NotifyIcon.hWnd = m_hWnd;
	this->NotifyIcon.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	this->NotifyIcon.hIcon = AfxGetApp()->LoadIcon( IDR_MAINFRAME );
	this->NotifyIcon.uCallbackMessage = WM_USER_TASKTRAY_CLICK;
	lstrcpy( this->NotifyIcon.szTip, L"MultiMonitorUtil by rti" );

	// �A�C�R����o�^�ł��邩�G���[�ɂȂ�܂ŌJ��Ԃ�
	while (true)
	{
		if (Shell_NotifyIcon(NIM_ADD, &this->NotifyIcon))
		{
			// �o�^�ł�����I���
			break;
		}
		else
		{
			// �^�C���A�E�g���ǂ������ׂ�
			if (::GetLastError() != ERROR_TIMEOUT)
			{
				// �A�C�R���o�^�G���[
				return false;
			}

			// �o�^�ł��Ă��Ȃ����Ƃ��m�F����
			if (Shell_NotifyIcon(NIM_MODIFY, &this->NotifyIcon))
			{
				// �o�^�ł��Ă���
				break;
			}
			else
			{
				// �o�^�ł��Ă��Ȃ�����
				::Sleep(1000);
			}
		}
	}
	return true;
}

//�^�X�N�g���C����A�C�R��������
bool CMultiMonitorUtilDlg::DelNotifyIcon()
{
	::Shell_NotifyIcon( NIM_DELETE, &this->NotifyIcon );
	return true;
}

//�^�X�N�g���C�̃A�C�R�����N���b�N���ꂽ�Ƃ�
afx_msg LRESULT CMultiMonitorUtilDlg::OnTaskTrayClick(WPARAM wParam, LPARAM lParam)
{
	// �J�[�\���̌��݈ʒu���擾
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

//�V�F�������񂾎��ɔ��ł��郁�b�Z�[�W
afx_msg LRESULT CMultiMonitorUtilDlg::OnTaskbarCreated(WPARAM wParam, LPARAM lParam)
{
	this->AddNotifyIcon();
	return 0;
}


//�ݒ�_�C�A���O���J��
void CMultiMonitorUtilDlg::OnMenuCommandDialog() 
{
	// �_�C�A���O���J���B
	CNiseMainDialog mainDialog;

	//�R���t�B�O��n��
	mainDialog.Config = this->Config;

	mainDialog.DoModal();

	//�R���t�B�O��Ⴄ.
	this->Config = mainDialog.Config;

	//�ݒ��L���ɂ���.
	SetHotKey();
	//�R���t�B�O����������
	SaveConfig();
}

//�I��
void CMultiMonitorUtilDlg::OnMenuCommandExit() 
{
	// �I�����I�΂ꂽ�烁�C���E�B���h�E����ăv���O�������I������B
	PostMessage(WM_CLOSE);
}


void CMultiMonitorUtilDlg::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	lpwndpos->flags &= ~SWP_SHOWWINDOW;
	CDialogEx::OnWindowPosChanged(lpwndpos);

	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����܂��B
}


//�ݒ��ǂݍ���
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

//�ݒ��ۑ�����
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

//�ݒ��L���ɂ���
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
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
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
