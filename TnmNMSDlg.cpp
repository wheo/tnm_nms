
// TnmNMSDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "TnmNMS.h"
#include "TnmNMSDlg.h"
#include "afxdialogex.h"
#include "PopupNetworkDlg.h"

#pragma comment(lib, "IPHLPAPI.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CTnmNMSDlg 대화 상자

COLORREF g_colorTabBG = RGB(220, 220, 220);

CCriticalSection g_csComm;
CCommSNMP *g_pCommSnmp = NULL;
CWaveFileAlarm *g_pAlarm = NULL;

stGroupState *g_pGroupState[MAX_GROUP_COUNT];

TCHAR g_strLevelName[5][50] = { _T(""), _T("disabled"), _T("information"), _T("warninging"), _T("critical") };
TCHAR g_strMsgType[4][50] = { _T(""), _T("begin"), _T("end"), _T("log") };

CTnmNMSDlg::CTnmNMSDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TNMNMS_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pLogDlg = NULL;
	m_pServerDlg = NULL;
}

void CTnmNMSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTnmNMSDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BTN_EXIT, OnBnClickedExit)
	ON_BN_CLICKED(IDC_BTN_MIN, OnBnClickedMin)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CTnmNMSDlg 메시지 처리기

BOOL CTnmNMSDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	WSADATA wsaData;
	if (WSAStartup(0x202, &wsaData) == SOCKET_ERROR) {
		WSACleanup();
	}

	for (int i = 0; i < MAX_GROUP_COUNT; i++) {
		g_pGroupState[i] = new stGroupState;
		memset(g_pGroupState[i], 0, sizeof(stGroupState));
		g_pGroupState[i]->stGroupInfo.nIndex = i;
	}

	CreateGUI();

	GetNetworkInfo();

	_d(L"[main] 연결 가능한 네트웍 어뎁터 개수: %d\n", m_nEthernetCount);

	if (m_nEthernetCount == 0) {
		AfxMessageBox(L"연결할 수 있는 장치가 없습니다. 네트워크를 확인해 주세요");
	}
	else if (m_nEthernetCount == 1) {
		g_pCommSnmp = new CCommSNMP(m_strEthernetIP[0]);
	}
	else {
		CPopupNetworkDlg dlg;
		for (int i = 0; i<m_nEthernetCount; i++) {
			dlg.SetIPInfo(i, m_strEthernetIP[i]);
		}

		if (dlg.DoModal() == IDOK) {
			int nIndex = dlg.GetIPIndex();
			g_pCommSnmp = new CCommSNMP(m_strEthernetIP[nIndex]);
		}
	}

	g_pAlarm = new CWaveFileAlarm();
	g_pAlarm->SetWaveFile(_T("SmokeAlarm.wav"));

	CreateDirectory(L"History", NULL);

	SetTimer(1, 100, NULL);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

BOOL CTnmNMSDlg::DestroyWindow()
{
	KillTimer(1);

	if (m_pLogDlg) {
		m_pLogDlg->DestroyWindow();
	}
	SAFE_DELETE(m_pLogDlg);

	if (m_pServerDlg) {
		m_pServerDlg->DestroyWindow();
	}
	SAFE_DELETE(m_pServerDlg);

	for (int i = 0; i < MAX_GROUP_COUNT; i++) {
		SAFE_DELETE(g_pGroupState[i]);
	}

	SAFE_DELETE(g_pCommSnmp);

	SAFE_DELETE(g_pAlarm);

	WSACleanup();

	return CDialogEx::DestroyWindow();
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CTnmNMSDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CTnmNMSDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTnmNMSDlg::GetNetworkInfo()
{
	DWORD Err;

	PFIXED_INFO pFixedInfo;
	DWORD FixedInfoSize = 0;

	PIP_ADAPTER_INFO pAdapterInfo, pAdapt;
	DWORD AdapterInfoSize;
	PIP_ADDR_STRING pAddrStr;
	CString strResult, strTemp;

	//
	// Get the main IP configuration information for this machine using a FIXED_INFO structure
	//
	if ((Err = GetNetworkParams(NULL, &FixedInfoSize)) != 0) {
		if (Err != ERROR_BUFFER_OVERFLOW) {
			AfxMessageBox(L"GetNetworkParams sizing failed with error\n");
			return;
		}
	}

	// Allocate memory from sizing information
	if ((pFixedInfo = (PFIXED_INFO)GlobalAlloc(GPTR, FixedInfoSize)) == NULL) {
		AfxMessageBox(L"Memory allocation error\n");
		return;
	}

	if ((Err = GetNetworkParams(pFixedInfo, &FixedInfoSize)) == 0) {
		strTemp.Format(L"\tHost Name . . . . . . . . . : \t%s\n", pFixedInfo->HostName);
		strResult = strTemp;

		strTemp.Format(L"\tDNS Servers . . . . . . . . : \t%s\n", pFixedInfo->DnsServerList.IpAddress.String);
		strResult += strTemp;

		pAddrStr = pFixedInfo->DnsServerList.Next;

		while (pAddrStr) {
			strTemp.Format(L"\t\t\t\t%s\n", pAddrStr->IpAddress.String);
			strResult += strTemp;
			pAddrStr = pAddrStr->Next;
		}

		strTemp.Format(L"\tNode Type . . . . . . . . . : ");
		strResult += strTemp;

		switch (pFixedInfo->NodeType) {
		case 1:
			strTemp.Format(L"%s\n", L"Broadcast");
			break;
		case 2:
			strTemp.Format(L"%s\n", L"Peer to peer");
			break;
		case 4:
			strTemp.Format(L"%s\n", L"Mixed");
			break;
		case 8:
			strTemp.Format(L"%s\n", L"Hybrid");
			break;
		default:
			strTemp.Format(L"\n");
		}

		strResult += strTemp;

		strTemp.Format(L"\tNetBIOS Scope ID. . . . . . : %s\n", pFixedInfo->ScopeId);
		strResult += strTemp;

		strTemp.Format(L"\tIP Routing Enabled. . . . . : %s\n", (pFixedInfo->EnableRouting ? "yes" : "no"));
		strResult += strTemp;

		strTemp.Format(L"\tWINS Proxy Enabled. . . . . : %s\n", (pFixedInfo->EnableProxy ? "yes" : "no"));
		strResult += strTemp;

		strTemp.Format(L"\tNetBIOS Resolution Uses DNS : %s\n", (pFixedInfo->EnableDns ? "yes" : "no"));
		strResult += strTemp;
	}
	else {
		strTemp.Format(L"GetNetworkParams failed with error %d\n", Err);
		strResult += strTemp;
		return;
	}

	//
	// Enumerate all of the adapter specific information using the IP_ADAPTER_INFO structure.
	// Note:  IP_ADAPTER_INFO contains a linked list of adapter entries.
	//
	AdapterInfoSize = 0;
	if ((Err = GetAdaptersInfo(NULL, &AdapterInfoSize)) != 0) {
		if (Err != ERROR_BUFFER_OVERFLOW) {
			AfxMessageBox(L"GetAdaptersInfo sizing failed with error %d\n", Err);
			return;
		}
	}

	// Allocate memory from sizing information
	if ((pAdapterInfo = (PIP_ADAPTER_INFO)GlobalAlloc(GPTR, AdapterInfoSize)) == NULL) {
		AfxMessageBox(L"Memory allocation error\n");
		return;
	}

	// Get actual adapter information
	if ((Err = GetAdaptersInfo(pAdapterInfo, &AdapterInfoSize)) != 0) {
		AfxMessageBox(L"GetAdaptersInfo failed with error %d\n", Err);
		return;
	}

	pAdapt = pAdapterInfo;

	int iCount = 0;
	while (pAdapt) {
		switch (pAdapt->Type) {
		case MIB_IF_TYPE_ETHERNET:
			strTemp.Format(L"\nEthernet adapter ");
			break;
		case MIB_IF_TYPE_TOKENRING:
			strTemp.Format(L"\nToken Ring adapter ");
			break;
		case MIB_IF_TYPE_FDDI:
			strTemp.Format(L"\nFDDI adapter ");
			break;
		case MIB_IF_TYPE_PPP:
			strTemp.Format(L"\nPPP adapter ");
			break;
		case MIB_IF_TYPE_LOOPBACK:
			strTemp.Format(L"\nLoopback adapter ");
			break;
		case MIB_IF_TYPE_SLIP:
			strTemp.Format(L"\nSlip adapter ");
			break;
		case MIB_IF_TYPE_OTHER:
		default:
			strTemp.Format(L"\nOther adapter ");
		}

		strResult += strTemp;
		strTemp.Format(L"%s:\n\n", pAdapt->AdapterName);
		strResult += strTemp;

		strTemp.Format(L"\tDescription . . . . . . . . : %s\n", pAdapt->Description);
		strResult += strTemp;

		strTemp.Format(L"\tPhysical Address. . . . . . : ");
		strResult += strTemp;

		for (UINT i = 0; i<pAdapt->AddressLength; i++) {
			if (i == (pAdapt->AddressLength - 1)) {
				strTemp.Format(L"%.2X\n", (int)pAdapt->Address[i]);
			}
			else {
				strTemp.Format(L"%.2X-", (int)pAdapt->Address[i]);
			}
			strResult += strTemp;
		}

		strTemp.Format(L"\tDHCP Enabled. . . . . . . . : %s\n", (pAdapt->DhcpEnabled ? "yes" : "no"));
		strResult += strTemp;

		pAddrStr = &(pAdapt->IpAddressList);

		while (pAddrStr) {
			strTemp.Format(L"\tIP Address. . . . . . . . . : %s\n", pAddrStr->IpAddress.String);
			strResult += strTemp;

			//		if (pAdapt->Type == MIB_IF_TYPE_ETHERNET) 
			{
				wchar_t strUnicodeAddr[150];
				CString strIpAddr;
				MByteToUnicode(pAddrStr->IpAddress.String, strUnicodeAddr, 150);
				strIpAddr.Format(L"%s", strUnicodeAddr);
				if (strIpAddr.IsEmpty() == false) {
					if (_tcscmp(strUnicodeAddr, L"0.0.0.0") != 0) {
						m_strEthernetIP[iCount].Format(L"%s", strIpAddr);
						iCount++;
					}
				}
			}

			strTemp.Format(L"\tSubnet Mask . . . . . . . . : %s\n", pAddrStr->IpMask.String);
			strResult += strTemp;
			pAddrStr = pAddrStr->Next;
		}

		strTemp.Format(L"\tDefault Gateway . . . . . . : %s\n", pAdapt->GatewayList.IpAddress.String);
		strResult += strTemp;

		pAddrStr = pAdapt->GatewayList.Next;

		while (pAddrStr) {
			strTemp.Format(L"%52s\n", pAddrStr->IpAddress.String);
			strResult += strTemp;
			pAddrStr = pAddrStr->Next;
		}

		strTemp.Format(L"\tDHCP Server . . . . . . . . : %s\n", pAdapt->DhcpServer.IpAddress.String);
		strResult += strTemp;

		strTemp.Format(L"\tPrimary WINS Server . . . . : %s\n", pAdapt->PrimaryWinsServer.IpAddress.String);
		strResult += strTemp;

		strTemp.Format(L"\tSecondary WINS Server . . . : %s\n", pAdapt->SecondaryWinsServer.IpAddress.String);
		strResult += strTemp;

		struct tm *newtime;

		// Display coordinated universal time - GMT 
		newtime = gmtime(&pAdapt->LeaseObtained);
		strTemp.Format(L"\tLease Obtained. . . . . . . : %s", asctime(newtime));
		strResult += strTemp;

		newtime = gmtime(&pAdapt->LeaseExpires);
		strTemp.Format(L"\tLease Expires . . . . . . . : %s", asctime(newtime));
		strResult += strTemp;

		pAdapt = pAdapt->Next;
	}

	m_nEthernetCount = iCount;
#if 0
	//AfxMessageBox(strResult);
	_d(L"[main] %s\n", strResult);
#endif

}

void CTnmNMSDlg::CreateGUI()
{
	CRect rc;

	MoveWindow(0, 0, 1920, 1080);

	GetClientRect(&rc);

	m_cBtnExit.Create(L"", WS_VISIBLE | WS_CHILD, CRect(rc.right - 50, rc.top + 3, rc.right - 3, rc.top + 24), this, IDC_BTN_EXIT);
	m_cBtnExit.LoadImages(IDB_BTN_EXIT, 3);

	m_cBtnMin.Create(L"", WS_VISIBLE | WS_CHILD, CRect(rc.right - 80, rc.top + 3, rc.right - 51, rc.top + 24), this, IDC_BTN_MIN);
	m_cBtnMin.LoadImages(IDB_BTN_MIN, 3);

	///> 서버 화면
	m_pServerDlg = new CSubServerMgrDlg(this);
	m_pServerDlg->Create(IDD_SUB_SERVER_MGR_DLG, this);	
	m_pServerDlg->MoveWindow(0, 30, 1920, 760);
//	m_pServerDlg->MoveWindow(0, 30, 1920, 765);
	m_pServerDlg->ShowWindow(SW_SHOW);

	///> 로그 화면
	m_pLogDlg = new CSubLogDlg(this);
	m_pLogDlg->Create(IDD_SUB_LOG_DLG, this);
	m_pLogDlg->MoveWindow(0, 790, 1920, 290);
	m_pLogDlg->ShowWindow(SW_SHOW);
}

BOOL CTnmNMSDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rc, rcText;
	GetClientRect(&rc);

	//	CBrush guideBrush(RGB(150, 10, 10));
	CTnmMemDC dc(pDC, rc);

	rc.top = 0;
	rc.bottom = rc.top + 30;
	dc.FillSolidRect(&rc, RGB(50, 50, 50));
	//	dc.FillSolidRect(&rc, RGB(5, 30, 160));

	rc.left = 10;
	rc.right = rc.left + 100;
	rc.top = 7;
	rc.bottom = rc.top + 20;
	dc.SetTextColor(RGB(255, 255, 255));
	dc.DrawText(L"TNM NMS", &rc, DT_VCENTER);

	return false;

	return CDialogEx::OnEraseBkgnd(pDC);
}


BOOL CTnmNMSDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN) {
		switch (pMsg->wParam) {
		case VK_ESCAPE:
			return TRUE;
			break;

		case VK_RETURN:
			return TRUE;
			break;

		case VK_SPACE:
			return TRUE;
			break;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CTnmNMSDlg::OnBnClickedExit()
{
	OnOK();
}

void CTnmNMSDlg::OnBnClickedMin()
{
	ShowWindow(SW_MINIMIZE);
}

void CTnmNMSDlg::GetWarningMsg()
{
	if (!g_pCommSnmp) {
		return;
	}

	stTrapMsg *pMsg = (stTrapMsg*)g_pCommSnmp->GetFirstTrapMsg();

	if (!pMsg) {
		return;
	}

	//> 그룹 정보 찾아서 복사
	bool bFindServer = false;
	int iGroupIndex = 0;
	int nServerType = 0;
	TCHAR strGroupName[150];
	TCHAR strServerName[150];
	CString strData;

	//> 트랩이 발생한 장비 찾기
	for (int i = 0; i < MAX_GROUP_COUNT; i++) {
		if (!g_pGroupState[i]) {
			continue;
		}

		nServerType = pMsg->nServerType;
		if (nServerType == SERVER_TYPE_ENCODER) {
			strData.Format(L"%s", g_pGroupState[i]->stGroupInfo.stEncInfo.strServerIP);
			if (strData.IsEmpty()) {
				continue;
			}

			if (_tcscmp(strData, pMsg->strServerIP) == 0) {
				iGroupIndex = i;

				//> 그룹정보 복사
				_stprintf(strGroupName, L"%s", g_pGroupState[i]->stGroupInfo.strGroupName);
				//> 서버정보 복사
				_stprintf(strServerName, L"%s", g_pGroupState[i]->stGroupInfo.stEncInfo.strServerName);
				bFindServer = true;
			}
		}
		else if (nServerType == SERVER_TYPE_DECODER) {
			strData.Format(L"%s", g_pGroupState[i]->stGroupInfo.stDecInfo.strServerIP);
			if (strData.IsEmpty()) {
				continue;
			}

			if (_tcscmp(strData, pMsg->strServerIP) == 0) {
				iGroupIndex = i;

				_stprintf(strGroupName, L"%s", g_pGroupState[i]->stGroupInfo.strGroupName);
				_stprintf(strServerName, L"%s", g_pGroupState[i]->stGroupInfo.stDecInfo.strServerName);
				bFindServer = true;
			}
		}

		if (bFindServer) {
			break;
		}
	}

	///> 180305 - 추가안된 서버의 트랩메시지 무시
	if (bFindServer == false) {
	}
	else {
		stWarningMsg *pAlramMsg = new stWarningMsg;
		memset(pAlramMsg, 0, sizeof(stWarningMsg));

		//> 그룹명
		_stprintf(pAlramMsg->groupInfo.strGroupName, L"%s", strGroupName);

		//> 서버명
		_stprintf(pAlramMsg->serverInfo.strServerName, L"%s", strServerName);
		_stprintf(pAlramMsg->serverInfo.strServerIP, L"%s", pMsg->strServerIP);
		//memcpy(&pMsg->serverInfo, &m_stTrapMsg.stServerInfo, sizeof(stServerInfo));

		//> 정보					
		_stprintf(pAlramMsg->strWarningDescription, L"%s", pMsg->strDescription);

		//> 레벨 
		pAlramMsg->iLevelIndex = pMsg->nData[2];
		_stprintf(pAlramMsg->strWarningLevel, L"%s", g_strLevelName[pAlramMsg->iLevelIndex]);

		//> 메시지 타입
		pAlramMsg->nMsgType = pMsg->nData[3];
		_stprintf(pAlramMsg->strWarningType, L"%s", g_strMsgType[pAlramMsg->nMsgType]);

		//> 시간정보
		if (pAlramMsg->nMsgType == 1) {
			_stprintf(pAlramMsg->strWarningTime1, L"%s", pMsg->strMsgTime);
		}
		else if (pAlramMsg->nMsgType == 2) {
			_stprintf(pAlramMsg->strWarningTime2, L"%s", pMsg->strMsgTime);
		}

		//> 로그창 처리
		if (m_pLogDlg) {
			m_pLogDlg->AddWarningMsg(pAlramMsg);
		}
				
		if (m_pServerDlg) {
			m_pServerDlg->CreateAlram(iGroupIndex, nServerType, pAlramMsg);
		}					

		delete pAlramMsg;
	}

	delete pMsg;
}

LRESULT CTnmNMSDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_TNM_SUB_LOG_SIZE_CHANGE) {
		bool bIsFull = (bool)wParam;
		if (m_pLogDlg) {
			if (bIsFull) {
				if (m_pServerDlg) {
					m_pServerDlg->ShowWindow(SW_HIDE);
				}
				m_pLogDlg->MoveWindow(0, 30, 1920, 1050);
			}
			else {
				m_pLogDlg->MoveWindow(0, 790, 1920, 290);
				if (m_pServerDlg) {
					m_pServerDlg->ShowWindow(SW_SHOW);
				}
			}
		}
	}	
	
	return CDialogEx::WindowProc(message, wParam, lParam);
}


void CTnmNMSDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1) {
		GetWarningMsg();
	}

	CDialogEx::OnTimer(nIDEvent);
}
