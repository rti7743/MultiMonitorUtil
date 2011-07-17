
// MultiMonitorUtilDlg.h : �w�b�_�[ �t�@�C��
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

//�ݒ�t�@�C���̃L�[�̕���.
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

// CMultiMonitorUtilDlg �_�C�A���O
class CMultiMonitorUtilDlg : public CDialogEx
{
// �R���X�g���N�V����
public:
	CMultiMonitorUtilDlg(CWnd* pParent = NULL);	// �W���R���X�g���N�^�[

// �_�C�A���O �f�[�^
	enum { IDD = IDD_MULTIMONITORUTIL_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �T�|�[�g
private:
	//�ݒ��L���ɂ���
	void CMultiMonitorUtilDlg::SetHotKey();
	//�ݒ��ۑ�����
	void CMultiMonitorUtilDlg::SaveConfig();
	//�ݒ��ǂݍ���
	void CMultiMonitorUtilDlg::LoadConfig();

	//�}���`���j�^�[�R���g���[���̖{��
	CMultiMonitor MonitorUtil;

// ����
protected:
	HICON m_hIcon;
	//�ʒm
	NOTIFYICONDATA NotifyIcon ;
	//�ݒ�t�@�C��
	DataConfig Config;


	//�ʒm�̈�ɒǉ�
	bool CMultiMonitorUtilDlg::AddNotifyIcon();
	//�^�X�N�g���C����A�C�R��������
	bool CMultiMonitorUtilDlg::DelNotifyIcon();
	//�^�X�N�g���C���N���b�N���ꂽ�ꍇ
	afx_msg LRESULT CMultiMonitorUtilDlg::OnTaskTrayClick(WPARAM wParam, LPARAM lParam);
	//�V�F�������񂾎��ɔ��ł��郁�b�Z�[�W
	afx_msg LRESULT CMultiMonitorUtilDlg::OnTaskbarCreated(WPARAM wParam, LPARAM lParam);
	//�ݒ�_�C�A���O���J��
	void CMultiMonitorUtilDlg::OnMenuCommandDialog(); 
	//�I��
	void CMultiMonitorUtilDlg::OnMenuCommandExit() ;
	BOOL CMultiMonitorUtilDlg::Create();
	


	// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();
public:
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
};
