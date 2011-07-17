// NiseMainDialog.cpp : 実装ファイル
//

#include "stdafx.h"
#include "MultiMonitorUtil.h"
#include "NiseMainDialog.h"
#include "afxdialogex.h"

// CNiseMainDialog ダイアログ

IMPLEMENT_DYNAMIC(CNiseMainDialog, CDialogEx)

CNiseMainDialog::CNiseMainDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNiseMainDialog::IDD, pParent)
{

}

CNiseMainDialog::~CNiseMainDialog()
{
}

void CNiseMainDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MIN_RUN_KEY, m_MinRunKey);
	DDX_Control(pDX, IDC_MIN_RUN_ALTCHECK, m_MinRunAltCheck);
	DDX_Control(pDX, IDC_MIN_RUN_CTRLCHECK, m_MinRunCtrlCheck);
	DDX_Control(pDX, IDC_MIN_RUN_SHIFTCHECK, m_MinRunShiftCheck);
	DDX_Control(pDX, IDC_MIN_RUN_WINCHECK, m_MinRunWinCheck);

	DDX_Control(pDX, IDC_MIN_RESTORE_KEY, m_MinRestoreKey);
	DDX_Control(pDX, IDC_MIN_RESTORE_ALTCHECK, m_MinRestoreAltCheck);
	DDX_Control(pDX, IDC_MIN_RESTORE_CTRLCHECK, m_MinRestoreCtrlCheck);
	DDX_Control(pDX, IDC_MIN_RESTORE_SHIFTCHECK, m_MinRestoreShiftCheck);
	DDX_Control(pDX, IDC_MIN_RESTORE_WINCHECK, m_MinRestoreWinCheck);

	DDX_Control(pDX, IDC_MIN_TOGGLE_KEY, m_MinToggleKey);
	DDX_Control(pDX, IDC_MIN_TOGGLE_ALTCHECK, m_MinToggleAltCheck);
	DDX_Control(pDX, IDC_MIN_TOGGLE_CTRLCHECK, m_MinToggleCtrlCheck);
	DDX_Control(pDX, IDC_MIN_TOGGLE_SHIFTCHECK, m_MinToggleShiftCheck);
	DDX_Control(pDX, IDC_MIN_TOGGLE_WINCHECK, m_MinToggleWinCheck);

	DDX_Control(pDX, IDC_SUMMIT_RUN_KEY, m_SummitRunKey);
	DDX_Control(pDX, IDC_SUMMIT_RUN_ALTCHECK, m_SummitRunAltCheck);
	DDX_Control(pDX, IDC_SUMMIT_RUN_CTRLCHECK, m_SummitRunCtrlCheck);
	DDX_Control(pDX, IDC_SUMMIT_RUN_SHIFTCHECK, m_SummitRunShiftCheck);
	DDX_Control(pDX, IDC_SUMMIT_RUN_WINCHECK, m_SummitRunWinCheck);

	DDX_Control(pDX, IDC_SUMMIT_RESTORE_KEY, m_SummitRestoreKey);
	DDX_Control(pDX, IDC_SUMMIT_RESTORE_ALTCHECK, m_SummitRestoreAltCheck);
	DDX_Control(pDX, IDC_SUMMIT_RESTORE_CTRLCHECK, m_SummitRestoreCtrlCheck);
	DDX_Control(pDX, IDC_SUMMIT_RESTORE_SHIFTCHECK, m_SummitRestoreShiftCheck);
	DDX_Control(pDX, IDC_SUMMIT_RESTORE_WINCHECK, m_SummitRestoreWinCheck);

	DDX_Control(pDX, IDC_SUMMIT_TOGGLE_KEY, m_SummitToggleKey);
	DDX_Control(pDX, IDC_SUMMIT_TOGGLE_ALTCHECK, m_SummitToggleAltCheck);
	DDX_Control(pDX, IDC_SUMMIT_TOGGLE_CTRLCHECK, m_SummitToggleCtrlCheck);
	DDX_Control(pDX, IDC_SUMMIT_TOGGLE_SHIFTCHECK, m_SummitToggleShiftCheck);
	DDX_Control(pDX, IDC_SUMMIT_TOGGLE_WINCHECK, m_SummitToggleWinCheck);
}


BEGIN_MESSAGE_MAP(CNiseMainDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &CNiseMainDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CNiseMainDialog メッセージ ハンドラー


BOOL CNiseMainDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ここに初期化を追加してください

	//ダイアログを最前面に
	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

	//設定の読み込み
	m_MinRunKey.SetKeyCode(Config.KeyConfigs[HOTKEY_MIN_RUN].key);
	m_MinRunWinCheck.SetCheck( static_cast<BOOL>(Config.KeyConfigs[HOTKEY_MIN_RUN].isWin) );
	m_MinRunAltCheck.SetCheck( static_cast<BOOL>(Config.KeyConfigs[HOTKEY_MIN_RUN].isAlt) );
	m_MinRunCtrlCheck.SetCheck( static_cast<BOOL>(Config.KeyConfigs[HOTKEY_MIN_RUN].isCtrl) );
	m_MinRunShiftCheck.SetCheck( static_cast<BOOL>(Config.KeyConfigs[HOTKEY_MIN_RUN].inShift) );

	m_MinRestoreKey.SetKeyCode(Config.KeyConfigs[HOTKEY_MIN_RESTORE].key);
	m_MinRestoreWinCheck.SetCheck( static_cast<BOOL>(Config.KeyConfigs[HOTKEY_MIN_RESTORE].isWin) );
	m_MinRestoreAltCheck.SetCheck( static_cast<BOOL>(Config.KeyConfigs[HOTKEY_MIN_RESTORE].isAlt) );
	m_MinRestoreCtrlCheck.SetCheck( static_cast<BOOL>(Config.KeyConfigs[HOTKEY_MIN_RESTORE].isCtrl) );
	m_MinRestoreShiftCheck.SetCheck( static_cast<BOOL>(Config.KeyConfigs[HOTKEY_MIN_RESTORE].inShift) );

	m_MinToggleKey.SetKeyCode(Config.KeyConfigs[HOTKEY_MIN_TOGGLE].key);
	m_MinToggleWinCheck.SetCheck( static_cast<BOOL>(Config.KeyConfigs[HOTKEY_MIN_TOGGLE].isWin) );
	m_MinToggleAltCheck.SetCheck( static_cast<BOOL>(Config.KeyConfigs[HOTKEY_MIN_TOGGLE].isAlt) );
	m_MinToggleCtrlCheck.SetCheck( static_cast<BOOL>(Config.KeyConfigs[HOTKEY_MIN_TOGGLE].isCtrl) );
	m_MinToggleShiftCheck.SetCheck( static_cast<BOOL>(Config.KeyConfigs[HOTKEY_MIN_TOGGLE].inShift) );

	m_SummitRunKey.SetKeyCode(Config.KeyConfigs[HOTKEY_SUMMIT_RUN].key);
	m_SummitRunWinCheck.SetCheck( static_cast<BOOL>(Config.KeyConfigs[HOTKEY_SUMMIT_RUN].isWin) );
	m_SummitRunAltCheck.SetCheck( static_cast<BOOL>(Config.KeyConfigs[HOTKEY_SUMMIT_RUN].isAlt) );
	m_SummitRunCtrlCheck.SetCheck( static_cast<BOOL>(Config.KeyConfigs[HOTKEY_SUMMIT_RUN].isCtrl) );
	m_SummitRunShiftCheck.SetCheck( static_cast<BOOL>(Config.KeyConfigs[HOTKEY_SUMMIT_RUN].inShift) );

	m_SummitRestoreKey.SetKeyCode(Config.KeyConfigs[HOTKEY_SUMMIT_RESTORE].key);
	m_SummitRestoreWinCheck.SetCheck( static_cast<BOOL>(Config.KeyConfigs[HOTKEY_SUMMIT_RESTORE].isWin) );
	m_SummitRestoreAltCheck.SetCheck( static_cast<BOOL>(Config.KeyConfigs[HOTKEY_SUMMIT_RESTORE].isAlt) );
	m_SummitRestoreCtrlCheck.SetCheck( static_cast<BOOL>(Config.KeyConfigs[HOTKEY_SUMMIT_RESTORE].isCtrl) );
	m_SummitRestoreShiftCheck.SetCheck( static_cast<BOOL>(Config.KeyConfigs[HOTKEY_SUMMIT_RESTORE].inShift) );

	m_SummitToggleKey.SetKeyCode(Config.KeyConfigs[HOTKEY_SUMMIT_TOGGLE].key);
	m_SummitToggleWinCheck.SetCheck( static_cast<BOOL>(Config.KeyConfigs[HOTKEY_SUMMIT_TOGGLE].isWin) );
	m_SummitToggleAltCheck.SetCheck( static_cast<BOOL>(Config.KeyConfigs[HOTKEY_SUMMIT_TOGGLE].isAlt) );
	m_SummitToggleCtrlCheck.SetCheck( static_cast<BOOL>(Config.KeyConfigs[HOTKEY_SUMMIT_TOGGLE].isCtrl) );
	m_SummitToggleShiftCheck.SetCheck( static_cast<BOOL>(Config.KeyConfigs[HOTKEY_SUMMIT_TOGGLE].inShift) );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


BOOL CNiseMainDialog::DestroyWindow()
{
	// TODO: ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	return CDialogEx::DestroyWindow();
}


void CNiseMainDialog::OnBnClickedOk()
{
	Config.KeyConfigs[HOTKEY_MIN_RUN].key = m_MinRunKey.GetKeyCode();
	Config.KeyConfigs[HOTKEY_MIN_RUN].isWin =  static_cast<bool>(m_MinRunWinCheck.GetCheck() != FALSE);
	Config.KeyConfigs[HOTKEY_MIN_RUN].isAlt =  static_cast<bool>(m_MinRunAltCheck.GetCheck() != FALSE);
	Config.KeyConfigs[HOTKEY_MIN_RUN].isCtrl =  static_cast<bool>(m_MinRunCtrlCheck.GetCheck() != FALSE);
	Config.KeyConfigs[HOTKEY_MIN_RUN].inShift =  static_cast<bool>(m_MinRunShiftCheck.GetCheck() != FALSE);

	Config.KeyConfigs[HOTKEY_MIN_RESTORE].key = m_MinRestoreKey.GetKeyCode();
	Config.KeyConfigs[HOTKEY_MIN_RESTORE].isWin =  static_cast<bool>(m_MinRestoreWinCheck.GetCheck() != FALSE);
	Config.KeyConfigs[HOTKEY_MIN_RESTORE].isAlt =  static_cast<bool>(m_MinRestoreAltCheck.GetCheck() != FALSE);
	Config.KeyConfigs[HOTKEY_MIN_RESTORE].isCtrl =  static_cast<bool>(m_MinRestoreCtrlCheck.GetCheck() != FALSE);
	Config.KeyConfigs[HOTKEY_MIN_RESTORE].inShift =  static_cast<bool>(m_MinRestoreShiftCheck.GetCheck() != FALSE);

	Config.KeyConfigs[HOTKEY_MIN_TOGGLE].key = m_MinToggleKey.GetKeyCode();
	Config.KeyConfigs[HOTKEY_MIN_TOGGLE].isWin =  static_cast<bool>(m_MinToggleWinCheck.GetCheck() != FALSE);
	Config.KeyConfigs[HOTKEY_MIN_TOGGLE].isAlt =  static_cast<bool>(m_MinToggleAltCheck.GetCheck() != FALSE);
	Config.KeyConfigs[HOTKEY_MIN_TOGGLE].isCtrl =  static_cast<bool>(m_MinToggleCtrlCheck.GetCheck() != FALSE);
	Config.KeyConfigs[HOTKEY_MIN_TOGGLE].inShift =  static_cast<bool>(m_MinToggleShiftCheck.GetCheck() != FALSE);

	Config.KeyConfigs[HOTKEY_SUMMIT_RUN].key = m_MinRunKey.GetKeyCode();
	Config.KeyConfigs[HOTKEY_SUMMIT_RUN].isWin =  static_cast<bool>(m_SummitRunWinCheck.GetCheck() != FALSE);
	Config.KeyConfigs[HOTKEY_SUMMIT_RUN].isAlt =  static_cast<bool>(m_SummitRunAltCheck.GetCheck() != FALSE);
	Config.KeyConfigs[HOTKEY_SUMMIT_RUN].isCtrl =  static_cast<bool>(m_SummitRunCtrlCheck.GetCheck() != FALSE);
	Config.KeyConfigs[HOTKEY_SUMMIT_RUN].inShift =  static_cast<bool>(m_SummitRunShiftCheck.GetCheck() != FALSE);

	Config.KeyConfigs[HOTKEY_SUMMIT_RESTORE].key = m_SummitRestoreKey.GetKeyCode();
	Config.KeyConfigs[HOTKEY_SUMMIT_RESTORE].isWin =  static_cast<bool>(m_SummitRestoreWinCheck.GetCheck() != FALSE);
	Config.KeyConfigs[HOTKEY_SUMMIT_RESTORE].isAlt =  static_cast<bool>(m_SummitRestoreAltCheck.GetCheck() != FALSE);
	Config.KeyConfigs[HOTKEY_SUMMIT_RESTORE].isCtrl =  static_cast<bool>(m_SummitRestoreCtrlCheck.GetCheck() != FALSE);
	Config.KeyConfigs[HOTKEY_SUMMIT_RESTORE].inShift =  static_cast<bool>(m_SummitRestoreShiftCheck.GetCheck() != FALSE);

	Config.KeyConfigs[HOTKEY_SUMMIT_TOGGLE].key = m_SummitToggleKey.GetKeyCode();
	Config.KeyConfigs[HOTKEY_SUMMIT_TOGGLE].isWin =  static_cast<bool>(m_SummitToggleWinCheck.GetCheck() != FALSE);
	Config.KeyConfigs[HOTKEY_SUMMIT_TOGGLE].isAlt =  static_cast<bool>(m_SummitToggleAltCheck.GetCheck() != FALSE);
	Config.KeyConfigs[HOTKEY_SUMMIT_TOGGLE].isCtrl =  static_cast<bool>(m_SummitToggleCtrlCheck.GetCheck() != FALSE);
	Config.KeyConfigs[HOTKEY_SUMMIT_TOGGLE].inShift =  static_cast<bool>(m_SummitToggleShiftCheck.GetCheck() != FALSE);



	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CDialogEx::OnOK();
}
