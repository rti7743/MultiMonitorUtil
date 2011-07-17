// KeyEdit.cpp : 実装ファイル
//

#include "stdafx.h"
#include "MultiMonitorUtil.h"
#include "KeyEdit.h"


// CKeyEdit

IMPLEMENT_DYNAMIC(CKeyEdit, CEdit)

CKeyEdit::CKeyEdit()
{
	this->KeyCode = 0;
}

CKeyEdit::~CKeyEdit()
{
}



BEGIN_MESSAGE_MAP(CKeyEdit, CEdit)
END_MESSAGE_MAP()




void CKeyEdit::SetKeyCode(UINT nChar)
{
	const TCHAR * p = NULL;
	switch(nChar)
	{
	case VK_CANCEL:	p = L"CANCEL"; break;
	case VK_BACK:	p = L"BACK"; break;
	case VK_TAB:	p = L"TAB"; break;
	case VK_CLEAR:	p = L"CLEAR"; break;
	case VK_RETURN:	p = L"RETURN"; break;
	case VK_MENU:	p = L"MENU"; break;
	case VK_PAUSE:	p = L"PAUSE"; break;

	case VK_SPACE:	p = L"SPACE"; break;
	case VK_PRIOR:	p = L"PRIOR"; break;
	case VK_NEXT:	p = L"NEXT"; break;
	case VK_END:	p = L"END"; break;
	case VK_HOME:	p = L"HOME"; break;
	case VK_LEFT:	p = L"LEFT"; break;
	case VK_UP:		p = L"UP"; break;
	case VK_RIGHT:	p = L"RIGHT"; break;
	case VK_DOWN:	p = L"DOWN"; break;

	case VK_SELECT:	p = L"SELECT"; break;
	case VK_PRINT:	p = L"PRINT"; break;
	case VK_EXECUTE:	p = L"EXECUTE"; break;
	case VK_SNAPSHOT:	p = L"SNAPSHOT"; break;
	case VK_INSERT:	p = L"INSERT"; break;
	case VK_HELP:	p = L"HELP"; break;
	case VK_SLEEP:	p = L"SLEEP"; break;

	case VK_NUMPAD0:	p = L"NUMPAD0"; break;
	case VK_NUMPAD1:	p = L"NUMPAD1"; break;
	case VK_NUMPAD2:	p = L"NUMPAD2"; break;
	case VK_NUMPAD3:	p = L"NUMPAD3"; break;
	case VK_NUMPAD4:	p = L"NUMPAD4"; break;
	case VK_NUMPAD5:	p = L"NUMPAD5"; break;
	case VK_NUMPAD6:	p = L"NUMPAD6"; break;
	case VK_NUMPAD7:	p = L"NUMPAD7"; break;
	case VK_NUMPAD8:	p = L"NUMPAD8"; break;
	case VK_NUMPAD9:	p = L"NUMPAD9"; break;
	case VK_MULTIPLY:	p = L"MULTIPLY"; break;
	case VK_ADD:	p = L"ADD"; break;
	case VK_SEPARATOR:	p = L"SEPARATOR"; break;
	case VK_SUBTRACT:	p = L"SUBTRACT"; break;
	case VK_DECIMAL:	p = L"DECIMAL"; break;
	case VK_DIVIDE:	p = L"DIVIDE"; break;

	case VK_F1:		p = L"F1"; break;
	case VK_F2:		p = L"F2"; break;
	case VK_F3:		p = L"F3"; break;
	case VK_F4:		p = L"F4"; break;
	case VK_F5:		p = L"F5"; break;
	case VK_F6:		p = L"F6"; break;
	case VK_F7:		p = L"F7"; break;
	case VK_F8:		p = L"F8"; break;
	case VK_F9:		p = L"F9"; break;
	case VK_F10:	p = L"F10"; break;
	case VK_F11:	p = L"F11"; break;
	case VK_F12:	p = L"F12"; break;
	case VK_F13:	p = L"F13"; break;
	case VK_F14:	p = L"F14"; break;
	case VK_F15:	p = L"F15"; break;
	case VK_F16:	p = L"F16"; break;
	case VK_F17:	p = L"F17"; break;
	case VK_F18:	p = L"F18"; break;
	case VK_F19:	p = L"F19"; break;
	case VK_F20:	p = L"F20"; break;
	case VK_F21:	p = L"F21"; break;
	case VK_F22:	p = L"F22"; break;
	case VK_F23:	p = L"F23"; break;
	case VK_F24:	p = L"F24"; break;
	case VK_NUMLOCK:	p = L"NUMLOCK"; break;
	case VK_SCROLL:	p = L"SCROLL"; break;
	case '0':	p = L"0"; break;
	case '1':	p = L"1"; break;
	case '2':	p = L"2"; break;
	case '3':	p = L"3"; break;
	case '4':	p = L"4"; break;
	case '5':	p = L"5"; break;
	case '6':	p = L"6"; break;
	case '7':	p = L"7"; break;
	case '8':	p = L"8"; break;
	case '9':	p = L"9"; break;
	case 'A':	p = L"A"; break;
	case 'B':	p = L"B"; break;
	case 'C':	p = L"C"; break;
	case 'D':	p = L"D"; break;
	case 'E':	p = L"E"; break;
	case 'F':	p = L"F"; break;
	case 'G':	p = L"G"; break;
	case 'H':	p = L"H"; break;
	case 'I':	p = L"I"; break;
	case 'J':	p = L"J"; break;
	case 'K':	p = L"K"; break;
	case 'L':	p = L"L"; break;
	case 'N':	p = L"N"; break;
	case 'M':	p = L"M"; break;
	case 'O':	p = L"O"; break;
	case 'P':	p = L"P"; break;
	case 'Q':	p = L"Q"; break;
	case 'R':	p = L"R"; break;
	case 'S':	p = L"S"; break;
	case 'T':	p = L"T"; break;
	case 'U':	p = L"U"; break;
	case 'V':	p = L"V"; break;
	case 'W':	p = L"W"; break;
	case 'X':	p = L"X"; break;
	case 'Y':	p = L"Y"; break;
	case 'Z':	p = L"Z"; break;

	case VK_ESCAPE:	p = NULL; break;
	case VK_DELETE:	p = NULL; break;

	default:
		return ;
	}


	if (p == NULL)
	{
		this->KeyCode = 0;
		CWnd::SetWindowText(L"");
	}
	else
	{
		this->KeyCode = nChar;
		CWnd::SetWindowText(p);
	}
}

UINT CKeyEdit::GetKeyCode() const
{
	return this->KeyCode;
}


// ウィンドウプロシージャのオーバーライド
LRESULT CKeyEdit::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch(message)
	{
	case WM_KEYUP:
		SetKeyCode(wParam);
		return 0;
	case WM_KEYDOWN:
	case WM_CHAR:
		return 0;
	case WM_HELP:
		return 0;
	}
	return CWnd::DefWindowProc(message, wParam, lParam);
}
