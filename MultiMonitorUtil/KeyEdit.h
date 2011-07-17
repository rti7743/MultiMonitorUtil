#pragma once


// CKeyEdit

class CKeyEdit : public CEdit
{
	DECLARE_DYNAMIC(CKeyEdit)

public:
	CKeyEdit();
	virtual ~CKeyEdit();
	void CKeyEdit::SetKeyCode(UINT nChar);
	UINT CKeyEdit::GetKeyCode() const;

private:
	//キーコード
	UINT KeyCode;

	virtual LRESULT CKeyEdit::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) ;

protected:
	DECLARE_MESSAGE_MAP()
};


