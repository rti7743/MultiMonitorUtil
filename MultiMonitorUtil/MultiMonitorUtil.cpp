
// MultiMonitorUtil.cpp : アプリケーションのクラス動作を定義します。
//

#include "stdafx.h"
#include "MultiMonitorUtil.h"
#include "MultiMonitorUtilDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMultiMonitorUtilApp

BEGIN_MESSAGE_MAP(CMultiMonitorUtilApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CMultiMonitorUtilApp コンストラクション

CMultiMonitorUtilApp::CMultiMonitorUtilApp()
{
	// 再起動マネージャーをサポートします
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: この位置に構築用コードを追加してください。
	// ここに InitInstance 中の重要な初期化処理をすべて記述してください。
}


// 唯一の CMultiMonitorUtilApp オブジェクトです。

CMultiMonitorUtilApp theApp;


// CMultiMonitorUtilApp 初期化

BOOL CMultiMonitorUtilApp::InitInstance()
{
	// アプリケーション マニフェストが visual スタイルを有効にするために、
	// ComCtl32.dll Version 6 以降の使用を指定する場合は、
	// Windows XP に InitCommonControlsEx() が必要です。さもなければ、ウィンドウ作成はすべて失敗します。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// アプリケーションで使用するすべてのコモン コントロール クラスを含めるには、
	// これを設定します。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	// ダイアログにシェル ツリー ビューまたはシェル リスト ビュー コントロールが
	// 含まれている場合にシェル マネージャーを作成します。
	CShellManager *pShellManager = new CShellManager;

	//多重起動のチェック
	if ( CMultiMonitorUtilDlg::checkAlreadyRunning() )
	{
		//重複起動しているので無視
		return FALSE;
	}

	// 標準初期化
	// これらの機能を使わずに最終的な実行可能ファイルの
	// サイズを縮小したい場合は、以下から不要な初期化
	// ルーチンを削除してください。
	// 設定が格納されているレジストリ キーを変更します。
	// TODO: 会社名または組織名などの適切な文字列に
	// この文字列を変更してください。
	//SetRegistryKey(_T("アプリケーション ウィザードで生成されたローカル アプリケーション"));
	// INIファイルのパス名を変更する
	delete [] m_pszProfileName;
	m_pszProfileName = new TCHAR[MAX_PATH];
	::GetCurrentDirectory(MAX_PATH, (LPTSTR)m_pszProfileName);
	lstrcat((LPTSTR)m_pszProfileName, L"\\setting.ini");



	CMultiMonitorUtilDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: ダイアログが <OK> で消された時のコードを
		//  記述してください。
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: ダイアログが <キャンセル> で消された時のコードを
		//  記述してください。
	}

	// 上で作成されたシェル マネージャーを削除します。
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	
//	delete [] m_pszProfileName;
//	m_pszProfileName = NULL;

	// ダイアログは閉じられました。アプリケーションのメッセージ ポンプを開始しないで
	//  アプリケーションを終了するために FALSE を返してください。
	return FALSE;
}

