
// MultiMonitorUtilDlg.h : ヘッダー ファイル
//

#pragma once

#include "MultiMonitor.h"

enum
{
	 HOTKEY_MIN_RUN
	,HOTKEY_MIN_RESTORE
	,HOTKEY_MIN_TOGGLE
	,HOTKEY_SUMMIT_RUN
	,HOTKEY_SUMMIT_RESTORE
	,HOTKEY_SUMMIT_TOGGLE
};

const int HOTKEY_START = HOTKEY_MIN_RUN;
const int HOTKEY_END = HOTKEY_SUMMIT_TOGGLE + 1;

//設定ファイルのキーの部分.
struct DataConfig
{
	struct KeyConfigOne
	{
		UINT key  ;
		bool isWin ;
		bool isAlt ;
		bool isCtrl ;
		bool inShift ;
	} KeyConfigs[HOTKEY_END];
};

// CMultiMonitorUtilDlg ダイアログ
class CMultiMonitorUtilDlg : public CDialogEx
{
// コンストラクション
public:
	CMultiMonitorUtilDlg(CWnd* pParent = NULL);	// 標準コンストラクター

// ダイアログ データ
	enum { IDD = IDD_MULTIMONITORUTIL_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート
private:
	//設定を有効にする
	void CMultiMonitorUtilDlg::SetHotKey();
	//設定を保存する
	void CMultiMonitorUtilDlg::SaveConfig();
	//設定を読み込む
	void CMultiMonitorUtilDlg::LoadConfig();

	//マルチモニターコントロールの本体
	CMultiMonitor MonitorUtil;

// 実装
protected:
	HICON m_hIcon;
	//通知
	NOTIFYICONDATA NotifyIcon ;
	//設定ファイル
	DataConfig Config;


	//通知領域に追加
	bool CMultiMonitorUtilDlg::AddNotifyIcon();
	//タスクトレイからアイコンを消す
	bool CMultiMonitorUtilDlg::DelNotifyIcon();
	//タスクトレイがクリックされた場合
	afx_msg LRESULT CMultiMonitorUtilDlg::OnTaskTrayClick(WPARAM wParam, LPARAM lParam);
	//シェルが死んだ時に飛んでくるメッセージ
	afx_msg LRESULT CMultiMonitorUtilDlg::OnTaskbarCreated(WPARAM wParam, LPARAM lParam);
	//設定ダイアログを開く
	void CMultiMonitorUtilDlg::OnMenuCommandDialog(); 
	//終了
	void CMultiMonitorUtilDlg::OnMenuCommandExit() ;
	BOOL CMultiMonitorUtilDlg::Create();
	


	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();
public:
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
};
