#include "stdafx.h"
#include "WaveFileAlarm.h"

#include <mmsystem.h>
#pragma comment(lib, "winmm")

CWaveFileAlarm::CWaveFileAlarm()
{
	m_strFilePath = _T("");
	m_bAlarm = false;
	m_nCountdown = 0;
	Start();
}


CWaveFileAlarm::~CWaveFileAlarm()
{
	Terminate();
}

void CWaveFileAlarm::SetWaveFile(CString strFilePath)
{
	m_strFilePath.Format(_T("%s"), strFilePath);
}

void CWaveFileAlarm::SetAlarm(bool bIsAlarm)
{
	if (bIsAlarm) {
		if (m_bAlarm == false) {
			m_bAlarm = true;
			sndPlaySound(m_strFilePath, SND_ASYNC | SND_LOOP);				
		}
		
		m_nCountdown = MAX_ALARM_SEC;

	//	_d(L"[Wavealarm] Start Alarm..\n");
	}
	else {
		m_bAlarm = false;		
		sndPlaySound(NULL, SND_ASYNC);
		m_nCountdown = 0;

	//	_d(L"[Wavealarm] End Alarm..\n");
	}
}

void CWaveFileAlarm::Run()
{
	while (!m_bExit) {
		if (m_bAlarm) {
			m_nCountdown--;
			if (m_nCountdown <= 0) {
				SetAlarm(false);
			}
		}

		Sleep(1000);
	}
}