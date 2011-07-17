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
	//最小化実行
	void CMultiMonitor::MinRun(void);
	//最小化解除
	void CMultiMonitor::MinRestore(void);
	//最小化トグルスイッチ
	void CMultiMonitor::MinToggle(void);
	//サミット実施
	void CMultiMonitor::SummitRun(void);
	//サミット解除
	void CMultiMonitor::SummitRestore(void);
	//サミット実施トグルスイッチ
	void CMultiMonitor::SummitToggle(void);

private:
	std::map<HMONITOR,CMultiMonitorMin*>		MinMap;
	std::map<HMONITOR,CMultiMonitorSummit*>		SummitMap;


};

