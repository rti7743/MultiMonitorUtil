
// MultiMonitorUtil.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです。
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CMultiMonitorUtilApp:
// このクラスの実装については、MultiMonitorUtil.cpp を参照してください。
//

class CMultiMonitorUtilApp : public CWinApp
{
public:
	CMultiMonitorUtilApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CMultiMonitorUtilApp theApp;