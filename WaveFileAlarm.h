#pragma once
class CWaveFileAlarm : public CTnmThread
{
public:
	CWaveFileAlarm();
	~CWaveFileAlarm();

	void SetWaveFile(CString strFilePath);

	void SetAlarm(bool bIsAlarm);

	bool IsAlarm() {
		return m_bAlarm;
	};

protected:
	void Run();

protected:
	bool m_bAlarm;
	CString m_strFilePath;
	int m_nCountdown;	///> 알람 시간
};

