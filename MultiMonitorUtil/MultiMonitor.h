#pragma once

class CMultiMonitorMin;
class CMultiMonitorSummit;

struct MultiMonitorSaveData{
	HWND Handle;
	bool IsIconic;
	bool IsZoomed;
	RECT Rect;
};

class CMultiMonitor
{
public:
	CMultiMonitor(void);
	~CMultiMonitor(void);
	//�ŏ������s
	void CMultiMonitor::MinRun(void);
	//�ŏ�������
	void CMultiMonitor::MinRestore(void);
	//�ŏ����g�O���X�C�b�`
	void CMultiMonitor::MinToggle(void);
	//�T�~�b�g���{
	void CMultiMonitor::SummitRun(void);
	//�T�~�b�g����
	void CMultiMonitor::SummitRestore(void);
	//�T�~�b�g���{�g�O���X�C�b�`
	void CMultiMonitor::SummitToggle(void);

private:
	std::map<HMONITOR,CMultiMonitorMin*>		MinMap;
	std::map<HMONITOR,CMultiMonitorSummit*>		SummitMap;


};

