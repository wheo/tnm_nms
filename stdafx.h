
// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������ 
// ��� �ִ� ���� �����Դϴ�.

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // �Ϻ� CString �����ڴ� ��������� ����˴ϴ�.

// MFC�� ���� �κа� ���� ������ ��� �޽����� ���� ����⸦ �����մϴ�.
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC �ٽ� �� ǥ�� ���� ����Դϴ�.
#include <afxext.h>         // MFC Ȯ���Դϴ�.


#include <afxdisp.h>        // MFC �ڵ�ȭ Ŭ�����Դϴ�.



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 ���� ��Ʈ�ѿ� ���� MFC �����Դϴ�.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // Windows ���� ��Ʈ�ѿ� ���� MFC �����Դϴ�.
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC�� ���� �� ��Ʈ�� ���� ����









#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

#define ENABLE_TRAP_MSG		0

#define MAX_ALARM_SEC			30

#define MAX_GROUP_COUNT			100

#define WM_TNM_LONG_PRESSED WM_USER+1
#define WM_TNM_SUB_LOG_SIZE_CHANGE	WM_USER+2
#define WM_TNM_SERVERLIST_SAVE		WM_USER+3
#define WM_TNM_SERVERLIST_LOAD		WM_USER+4
#define WM_TNM_SERVER_CTRL_GROUP_ADD	WM_USER+6
#define WM_TNM_SERVER_CTRL_GROUP_DELETE	WM_USER+7
#define WM_TNM_SERVER_CTRL_GROUP_MODIFY	WM_USER+8


#define WARNING_LEVEL_DISABLE	1
#define WARNING_LEVEL_INFORMATION	2
#define WARNING_LEVEL_WARNING	3
#define WARNING_LEVEL_CRITICAL	4

#define WARNING_TYPE_BEGIN		1
#define WARNING_TYPE_END		2
#define WARNING_TYPE_LOG		3

#define SERVER_TYPE_ENCODER		0
#define SERVER_TYPE_DECODER		1

#define SERVER_STATE_DISABLE	0
#define SERVER_STATE_RUNNING	1
#define SERVER_STATE_ERROR		2

#define LOG_OFFSET_INDEX	0
#define LOG_OFFSET_GROUP	1
#define LOG_OFFSET_SERVER	2
#define LOG_OFFSET_LEVEL	3
#define LOG_OFFSET_DESCRIPTION	4
#define LOG_OFFSET_TIME1		5
#define LOG_OFFSET_TIME2		6
#define LOG_OFFSET_LEVEL_INDEX	7


#define	MAX_TRAP_MSG_COUNT	50

#define TRAP_DATA_TYPE_STRING	0
#define TRAP_DATA_TYPE_INT		1

#define TRAP_ENC_CHANGE			0
#define TRAP_CAP_CHANGE			1
#define TRAP_NET_DOWN			2
#define TRAP_OVERLOAD			3
#define TRAP_MSG_INFO			4


#include "TnmMisc.h"
#include "MyButton.h"

#include "WaveFileAlarm.h"
#include "CommSNMP.h"


struct stServerInfo
{
	TCHAR strServerName[200];
	TCHAR strServerIP[50];
};

struct stServerState
{
	int nState;
	bool bAlarm;	//> �˶�����
	bool bACK;	//> ����ڰ� �˶� ���� �� ȯ�ο���. üũ�� ���������θ� ǥ��

	bool bgAlarm;
	DWORD dwAlramStart;
};

struct stGroupInfo
{
	TCHAR strGroupName[200];

	int nIndex;

	stServerInfo stEncInfo;	
	stServerInfo stDecInfo;	
};

struct stGroupState
{	
	bool bCreated;	
	stGroupInfo	stGroupInfo;

	stServerState stEncState;
	stServerState stDecState;
};

struct stWarningMsg
{
	stGroupInfo groupInfo;
	stServerInfo serverInfo;

	TCHAR strWarningLevel[50];
	TCHAR strWarningDescription[800];
	TCHAR strWarningTime1[100];
	TCHAR strWarningTime2[100];
	TCHAR strWarningType[50];

	int nMsgType; //> �߻�, ��������. 1: ���� (WARNING_TYPE_BEGIN), 2: ���� (WARNING_TYPE_END)
	int iLevelIndex;	//> 1: WARNING_LEVEL_DISABLE, 2: WARNING_LEVEL_INFORMATION, 3: WARNING_LEVEL_WARNING, 4: WARNING_LEVEL_CRITICAL
};

extern COLORREF g_colorTabBG;
extern stGroupState *g_pGroupState[MAX_GROUP_COUNT];
extern CCommSNMP *g_pCommSnmp;
extern CCriticalSection g_csComm;
extern TCHAR g_strLevelName[5][50];
extern TCHAR g_strMsgType[4][50];
extern CWaveFileAlarm *g_pAlarm;


bool CompareWarningMsg(stWarningMsg *pSrc, stWarningMsg *pDst);