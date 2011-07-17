#pragma once

#include "KeyEdit.h"
#include "MultiMonitorUtilDlg.h"

// CNiseMainDialog ダイアログ

class CNiseMainDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CNiseMainDialog)

public:
	CNiseMainDialog(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CNiseMainDialog();

// ダイアログ データ
	enum { IDD = IDD_NISE_MAIN_DIALOG };




protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	//設定データ
	DataConfig Config;

	virtual BOOL OnInitDialog();
private:
	virtual BOOL DestroyWindow();
	afx_msg void OnBnClickedOk();


	CKeyEdit m_MinRunKey;
	CButton m_MinRunAltCheck;
	CButton m_MinRunCtrlCheck;
	CButton m_MinRunShiftCheck;
	CButton m_MinRunWinCheck;

	CKeyEdit m_MinRestoreKey;
	CButton m_MinRestoreAltCheck;
	CButton m_MinRestoreCtrlCheck;
	CButton m_MinRestoreShiftCheck;
	CButton m_MinRestoreWinCheck;

	CKeyEdit m_MinToggleKey;
	CButton m_MinToggleAltCheck;
	CButton m_MinToggleCtrlCheck;
	CButton m_MinToggleShiftCheck;
	CButton m_MinToggleWinCheck;


	CKeyEdit m_SummitRunKey;
	CButton m_SummitRunAltCheck;
	CButton m_SummitRunCtrlCheck;
	CButton m_SummitRunShiftCheck;
	CButton m_SummitRunWinCheck;

	CKeyEdit m_SummitRestoreKey;
	CButton m_SummitRestoreAltCheck;
	CButton m_SummitRestoreCtrlCheck;
	CButton m_SummitRestoreShiftCheck;
	CButton m_SummitRestoreWinCheck;

	CKeyEdit m_SummitToggleKey;
	CButton m_SummitToggleAltCheck;
	CButton m_SummitToggleCtrlCheck;
	CButton m_SummitToggleShiftCheck;
	CButton m_SummitToggleWinCheck;

};
