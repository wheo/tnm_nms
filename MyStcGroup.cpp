#include "stdafx.h"
#include "MyStcGroup.h"


CMyStcGroup::CMyStcGroup()
{
	m_hBmpMain = NULL;
	m_hMdMain = NULL;
	m_hBmpSub = NULL;
	m_hMdSub = NULL;

	m_pGroupState = NULL;
	
	m_hBrushBGDisconnect = CreateSolidBrush(RGB(171, 171, 171));	
	m_hBrushBGGroup = CreateSolidBrush(RGB(0, 0, 0));
	m_hBrushBG = CreateSolidBrush(RGB(255, 255, 255));
	m_hBrushBGRunning = CreateSolidBrush(RGB(25, 210, 75));
	m_hBrushBGError = CreateSolidBrush(RGB(240, 15, 45));

	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));       // Zero out the structure
										   // Set Font Familly
	_tcscpy(lf.lfFaceName, _T("돋움"));
	lf.lfHeight = 14;// Set Height
	lf.lfWeight = FW_HEAVY;					// Set Weight

	m_hFontS = ::CreateFontIndirect(&lf);

	for (int i = 0; i < MAX_ERROR_MSG_COUNT; i++) {
		m_encError[i] = NULL;
		m_decError[i] = NULL;
	}

	m_nEncErrorMsgCount = 0;
	m_nDecErrorMsgCount = 0;
}

CMyStcGroup::~CMyStcGroup()
{
	Terminate();

	if (m_hMdMain) {
		::DeleteDC(m_hMdMain);
	}
	if (m_hMdSub) {
		::DeleteDC(m_hMdSub);
	}

	if (m_hBmpMain) {
		::DeleteObject(m_hBmpMain);
	}
	if (m_hBmpSub) {
		::DeleteObject(m_hBmpSub);
	}

	if (m_hBrushBGGroup) {
		::DeleteObject(m_hBrushBGGroup);
	}
	if (m_hBrushBGDisconnect) {
		::DeleteObject(m_hBrushBGDisconnect);
	}
	if (m_hBrushBGRunning) {
		::DeleteObject(m_hBrushBGRunning);
	}
	if (m_hBrushBGError) {
		::DeleteObject(m_hBrushBGError);
	}

	if (m_hFontS) {
		::DeleteObject(m_hFontS);
	}

	ClearAlram();
}

BEGIN_MESSAGE_MAP(CMyStcGroup, CStatic)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()

void CMyStcGroup::CreateAlram(int nServerType, stWarningMsg *pAlramMsg)
{
	if (!m_pGroupState) {
		return;
	}

	m_csErrorMsg.Lock();
	if (pAlramMsg->nMsgType == WARNING_TYPE_BEGIN) {
		if (pAlramMsg->iLevelIndex == WARNING_LEVEL_CRITICAL) {
			AddErrorMsg(nServerType, pAlramMsg);
		}
	} else if (pAlramMsg->nMsgType == WARNING_TYPE_END) { 
		DeleteErrorMsg(nServerType, pAlramMsg);
	}	
	m_csErrorMsg.Unlock();
}

void CMyStcGroup::AddErrorMsg(int nServerType, stWarningMsg *pAlramMsg)
{
	if (nServerType == SERVER_TYPE_ENCODER) {
		if ((m_nEncErrorMsgCount + 1) >= MAX_ERROR_MSG_COUNT) {
			return;
		}
	}
	else {
		if ((m_nDecErrorMsgCount + 1) >= MAX_ERROR_MSG_COUNT) {
			return;
		}
	}
	
	bool bAdd = false;
	for (int i = 0; i < MAX_ERROR_MSG_COUNT; i++) {		
		if (nServerType == SERVER_TYPE_ENCODER) {
			if (!m_encError[i]) {
				m_encError[i] = new stWarningMsg;
				memcpy(m_encError[i], pAlramMsg, sizeof(stWarningMsg));
				bAdd = true;
				m_nEncErrorMsgCount++;
				_d(L"[stcGroup] Add encoder err msg.. count: %d\n", m_nEncErrorMsgCount);

				m_pGroupState->stEncState.bAlarm = true;
				m_pGroupState->stEncState.bACK = false;
				break;
			}
		} else {
			if (!m_decError[i]) {
				m_decError[i] = new stWarningMsg;
				memcpy(m_decError[i], pAlramMsg, sizeof(stWarningMsg));
				bAdd = true;
				m_nDecErrorMsgCount++;
				_d(L"[stcGroup] Add decoder err msg.. count: %d\n", m_nDecErrorMsgCount);

				m_pGroupState->stDecState.bAlarm = true;
				m_pGroupState->stDecState.bACK = false;
				break;
			}
		}
	}

	if (bAdd) {
		///> 싸이렌 울리고
		if (!g_pAlarm) {
			return;
		}
		g_pAlarm->SetAlarm(true);
	}	
}
void CMyStcGroup::DeleteErrorMsg(int nServerType, stWarningMsg *pAlramMsg)
{
	for (int i = 0; i < MAX_ERROR_MSG_COUNT; i++) {
		if (nServerType == SERVER_TYPE_ENCODER) {
			if (!m_encError[i]) {
				continue;
			}

			if (CompareWarningMsg(m_encError[i], pAlramMsg)) {
				SAFE_DELETE(m_encError[i]);
				m_nEncErrorMsgCount--;

				if (m_nEncErrorMsgCount < 0) {
					m_nEncErrorMsgCount = 0;
				}

				_d(L"[stcGroup] Delete encoder err msg.. count: %d\n", m_nEncErrorMsgCount);
			}				
		}
		else {
			if (!m_decError[i]) {
				continue;
			}

			if (CompareWarningMsg(m_decError[i], pAlramMsg)) {
				SAFE_DELETE(m_decError[i]);
				m_nDecErrorMsgCount--;

				if (m_nDecErrorMsgCount < 0) {
					m_nDecErrorMsgCount = 0;
				}

				_d(L"[stcGroup] Delete decoder err msg.. count: %d\n", m_nDecErrorMsgCount);
			}				
		}
	}
}
void CMyStcGroup::ClearAlram()
{
	m_csErrorMsg.Lock();
	for (int i = 0; i < MAX_ERROR_MSG_COUNT; i++) {
		SAFE_DELETE(m_encError[i]);
		SAFE_DELETE(m_decError[i]);
	}
	m_nEncErrorMsgCount = 0;
	m_nDecErrorMsgCount = 0;
	m_csErrorMsg.Unlock();
}
void CMyStcGroup::ClearAlram(int nServer)
{
	m_csErrorMsg.Lock();
	if (nServer == SERVER_TYPE_ENCODER) {
		for (int i = 0; i < MAX_ERROR_MSG_COUNT; i++) {
			SAFE_DELETE(m_encError[i]);			
		}
		m_nEncErrorMsgCount = 0;		
	}
	else {
		for (int i = 0; i < MAX_ERROR_MSG_COUNT; i++) {			
			SAFE_DELETE(m_decError[i]);
		}		
		m_nDecErrorMsgCount = 0;
	}	
	m_csErrorMsg.Unlock();
}
void CMyStcGroup::SetACK(int nServer)
{
	m_csErrorMsg.Lock();
	if (nServer == SERVER_TYPE_ENCODER) {
		m_pGroupState->stEncState.bACK = true;		
	}
	else {
		m_pGroupState->stDecState.bACK = true;
	}
	m_csErrorMsg.Unlock();
}

void CMyStcGroup::SetGroupState(int nIndex, stGroupState *pState)
{
	Terminate();

	m_nIndex = nIndex;
	m_pGroupState = pState;	

	Start();
}

void CMyStcGroup::SetGroupInfo(stGroupInfo *pInfo)
{
	ClearAlram();

	m_csData.Lock();	
	if (pInfo) {
		memcpy(&m_pGroupState->stGroupInfo, pInfo, sizeof(stGroupInfo));
		m_pGroupState->bCreated = true;				
	}
	else {
		memset(m_pGroupState, 0, sizeof(stGroupState));
		m_pGroupState->bCreated = false;		
	}

	m_pGroupState->stGroupInfo.nIndex = m_nIndex;
	m_pGroupState->stDecState.bAlarm = false;
	m_pGroupState->stDecState.bACK = false;
	m_pGroupState->stDecState.bgAlarm = false;
	m_pGroupState->stEncState.bAlarm = false;
	m_pGroupState->stEncState.bgAlarm = false;
	m_pGroupState->stEncState.bACK = false;

	m_csData.Unlock();
}

///> 서버정보 수신		
void CMyStcGroup::GetGroupState()
{
	if (!m_pGroupState) {
		return;
	}
	if (m_pGroupState->bCreated == false) {
		return;
	}

	//> 그룹정보만 받아와서
	stGroupInfo stInfo;
	m_csData.Lock();
	memcpy(&stInfo, &m_pGroupState->stGroupInfo, sizeof(stGroupInfo));
	m_csData.Unlock();

	//> 상태정보 수신하고 
	int nEncState = SERVER_STATE_DISABLE;
	int nDecState = SERVER_STATE_DISABLE;
	g_csComm.Lock();
	if (!m_bExit) {
		nEncState = CheckEncoderState(stInfo.stEncInfo.strServerIP);
	}
	if (!m_bExit) {
		nDecState = CheckDecoderState(stInfo.stDecInfo.strServerIP);
	}
	g_csComm.Unlock();
		
	//> 상태정보 갱신
	m_csData.Lock();
	m_pGroupState->stEncState.nState = nEncState;
	m_pGroupState->stDecState.nState = nDecState;	

	if (m_nEncErrorMsgCount > 0) {
		m_pGroupState->stEncState.bAlarm = true;
	}
	else {
		m_pGroupState->stEncState.bAlarm = false;
	}

	if (m_nDecErrorMsgCount > 0) {
		m_pGroupState->stDecState.bAlarm = true;
	}
	else {
		m_pGroupState->stDecState.bAlarm = false;
	}
	m_csData.Unlock();
}

int CMyStcGroup::CheckEncoderState(TCHAR *strServerIP)
{
	int nState = SERVER_STATE_DISABLE;

	if (!g_pCommSnmp) {
		return nState;
	}

	//> 오디오 입력 체크
	int nRecv = g_pCommSnmp->GetEncoderAudioSignal(strServerIP);
	if (nRecv == -1) {
		nState = SERVER_STATE_DISABLE;
		return nState;
	}
	else if (nRecv != 2) {
		//> pcm이 아닌경우 error
		nState = SERVER_STATE_ERROR;
		return nState;
	}
	//> 비디오 입력 체크
	nRecv = g_pCommSnmp->GetEncoderVideoInfo(strServerIP);
	if (nRecv == -1) {
		nState = SERVER_STATE_DISABLE;
		return nState;
	} 
	else if ((nRecv == 1) || (nRecv == 2)) {
		//> unabailable, none
		nState = SERVER_STATE_ERROR;
		return nState;
	}
	//> 인터넷 링크 다운
#if _DEBUG
	//> 테스트용 장비에 인터넷선 2개는 빠져있음.
#else
	nRecv = g_pCommSnmp->GetEncoderSystemEthenetLinkDown(strServerIP);	
	if (nRecv == -1) {
		nState = SERVER_STATE_DISABLE;
		return nState;
	}
	else if (nRecv == 2) {		
		nState = SERVER_STATE_ERROR;
		return nState;
	}
#endif
	//> Fan failure
	nRecv = g_pCommSnmp->GetEncoderFanFailure(strServerIP);
	if (nRecv == -1) {
		nState = SERVER_STATE_DISABLE;
		return nState;
	}
	else if (nRecv == 1) {
		nState = SERVER_STATE_ERROR;
		return nState;
	}

	//> overheat
	nRecv = g_pCommSnmp->GetEncoderOverHeat(strServerIP);
	if (nRecv == -1) {
		nState = SERVER_STATE_DISABLE;
		return nState;
	}
	else if (nRecv >= 55) {
		nState = SERVER_STATE_ERROR;
		return nState;
	}

	//> power supply unit
#if _DEBUG
	//> 테스트용 장비에 1개만 사용.
#else
	nRecv = g_pCommSnmp->GetEncoderPSU(strServerIP);
	if (nRecv == -1) {
		nState = SERVER_STATE_DISABLE;
		return nState;
	}
	else if (nRecv == 1) {
		nState = SERVER_STATE_ERROR;
		return nState;
	}
#endif

	nState = SERVER_STATE_RUNNING;
	
	return nState;
}
int CMyStcGroup::CheckDecoderState(TCHAR *strServerIP)
{
	int nState = SERVER_STATE_DISABLE;

	if (!g_pCommSnmp) {
		return nState;
	}

	int nRecv = 0;
	//> 입력상태 체크
	nRecv = g_pCommSnmp->GetDecoderInputState(strServerIP);
	if (nRecv == -1) {
		nState = SERVER_STATE_DISABLE;
		return nState;
	}
	else if (nRecv == 2) {
		nState = SERVER_STATE_ERROR;
		return nState;
	}

	//> 입력 네트워크 상태
#if _DEBUG
	//> 테스트 장비에서 일부 네트웍 사용안함
#else
	nRecv = g_pCommSnmp->GetDecoderEthenetState(strServerIP);
	if (nRecv == -1) {
		nState = SERVER_STATE_DISABLE;
		return nState;
	}
	else if (nRecv == 2) {
		nState = SERVER_STATE_ERROR;
		return nState;
	}
#endif
	//> fan state
	nRecv = g_pCommSnmp->GetDecoderFanState(strServerIP);
	if (nRecv == -1) {
		nState = SERVER_STATE_DISABLE;
		return nState;
	}
	else if (nRecv == 1) {
		nState = SERVER_STATE_ERROR;
		return nState;
	}

	//> 온도
	nRecv = g_pCommSnmp->GetDecoderOverHeat(strServerIP);
	if (nRecv == -1) {
		nState = SERVER_STATE_DISABLE;
		return nState;
	}
	else if (nRecv >= 65) {
		nState = SERVER_STATE_ERROR;
		return nState;
	}

#if 0
	//> 입력 비트레이트 체크
	nRecv = g_pCommSnmp->GetDecoderInputBitrate(strServerIP);
	if (nRecv == -1) {
		nState = SERVER_STATE_DISABLE;
		return nState;
	}
	/*else if (nRecv == 0) {
		nState = SERVER_STATE_ERROR;
		return nState;
	}*/
#endif
	nState = SERVER_STATE_RUNNING;

	return nState;
}

void CMyStcGroup::DrawGroup()
{
	CRect rc;
	CRect rcServer[2];

	int nHeight = (m_rcClient.Height() - 15) / 2;
	int nWidth = (m_rcClient.Width() - 15) / 2;

	//> 상태정보 복사
	stGroupState stInfo;
	memset(&stInfo, 0, sizeof(stGroupState));

	m_csData.Lock();
	if (m_pGroupState) {
		memcpy(&stInfo, m_pGroupState, sizeof(stGroupState));
	}
	m_csData.Unlock();
	
	//> 전체 배경
	rc.CopyRect(&m_rcClient);		
	::FillRect(m_hMdSub, &rc, m_hBrushBG);		
	
	//> 인코더 상태정보
	rc.right = m_rcClient.left + nWidth;
	rcServer[0].CopyRect(&rc);
	if (stInfo.stEncState.nState == SERVER_STATE_ERROR) {
		::FillRect(m_hMdSub, &rc, m_hBrushBGError);
	}
	else if (stInfo.stEncState.nState == SERVER_STATE_RUNNING) {
		::FillRect(m_hMdSub, &rc, m_hBrushBGRunning);
	}
	else {
		::FillRect(m_hMdSub, &rc, m_hBrushBGDisconnect);
	}
	
	//> 인코더 디코더 연결선
	rc.top = m_rcClient.top + nHeight;
	rc.left = m_rcClient.left + nWidth;
	rc.right = rc.left + 15;
	rc.bottom = rc.top + 15;
	::FillRect(m_hMdSub, &rc, m_hBrushBGGroup);

	//> 디코더 상태정보
	rc.CopyRect(&m_rcClient);
	rc.left = m_rcClient.left + nWidth + 15;
	rcServer[1].CopyRect(&rc);
	if (stInfo.stDecState.nState == SERVER_STATE_ERROR) {
		::FillRect(m_hMdSub, &rc, m_hBrushBGError);
	}
	else if (stInfo.stDecState.nState == SERVER_STATE_RUNNING) {
		::FillRect(m_hMdSub, &rc, m_hBrushBGRunning);
	}
	else {
		::FillRect(m_hMdSub, &rc, m_hBrushBGDisconnect);
	}
	
	//> 알람여부
	bool bAlarm = false;
	if (stInfo.stDecState.bAlarm) {		
		bAlarm = true;
	}
	if (stInfo.stEncState.bAlarm) {
		bAlarm = true;
	}

	if (bAlarm) {		
		//> 그룹 전체선
		HPEN newPen, oldPen;
		newPen = ::CreatePen(PS_DASH, 5, RGB(255, 0, 0));

		oldPen = (HPEN)SelectObject(m_hMdSub, newPen);
		//		::FrameRect(m_hMdSub, &m_rcClient, m_hBrushAlarm);
		::MoveToEx(m_hMdSub, m_rcClient.left, m_rcClient.top, NULL);
		::LineTo(m_hMdSub, m_rcClient.right, m_rcClient.top);
		::LineTo(m_hMdSub, m_rcClient.right, m_rcClient.bottom);
		::LineTo(m_hMdSub, m_rcClient.left, m_rcClient.bottom);
		::LineTo(m_hMdSub, m_rcClient.left, m_rcClient.top);
		SelectObject(m_hMdSub, oldPen);
		DeleteObject(newPen);

		//> 개별
		if (stInfo.stEncState.bAlarm) {
			//> 180603 - ack 추가. 운용자 인지여부 추가
			if (stInfo.stEncState.bACK) {
				::FillRect(m_hMdSub, &rcServer[0], m_hBrushBGError);
			} else {
				if (stInfo.stEncState.bgAlarm) {
					m_pGroupState->stEncState.bgAlarm = false;
					::FillRect(m_hMdSub, &rcServer[0], m_hBrushBGError);
				}
				else {
					m_pGroupState->stEncState.bgAlarm = true;
				}
			}
		}

		if (stInfo.stDecState.bAlarm) {
			if (stInfo.stEncState.bACK) {
				::FillRect(m_hMdSub, &rcServer[0], m_hBrushBGError);
			}
			else {
				if (stInfo.stDecState.bgAlarm) {
					m_pGroupState->stDecState.bgAlarm = false;
					::FillRect(m_hMdSub, &rcServer[1], m_hBrushBGError);
				}
				else {
					m_pGroupState->stDecState.bgAlarm = true;
				}
			}
		}
	}

	//> 서버이름
	CString strData;
	COLORREF coText = RGB(22, 22, 22);
	strData.Format(L"%s", stInfo.stGroupInfo.stEncInfo.strServerName);
	if (strData.IsEmpty() == false) {
		MyDrawText(strData, rcServer[0], m_hFontS, coText, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	}

	strData.Format(L"%s", stInfo.stGroupInfo.stDecInfo.strServerName);
	if (strData.IsEmpty() == false) {
		MyDrawText(strData, &rcServer[1], m_hFontS, coText, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	}
}

void CMyStcGroup::MyDrawText(CString strText, CRect rcText, HFONT hFont, COLORREF txtColor, UINT uiFormat)
{
	HGDIOBJ hOldFont = ::SelectObject(m_hMdSub, hFont);

	::SetTextColor(m_hMdSub, txtColor);
	::DrawText(m_hMdSub, strText, strText.GetLength(), &rcText, uiFormat);

	::SelectObject(m_hMdSub, hOldFont);
}

void CMyStcGroup::Run1()
{	
	///> 그려놓은 서브 이미지를 메인 이미지에 복사
	while (!m_bExit) {
		CRect rc;
		GetClientRect(&rc);
		
		///> 그룹정보 전시
		DrawGroup();
				
		::BitBlt(m_hMdMain, 0, 0, rc.Width(), rc.Height(), m_hMdSub, 0, 0, SRCCOPY);		
		::InvalidateRect(m_hWnd, NULL, false);

		for (int i = 0; i < 10; i++) {
			Sleep(10);
			if (m_bExit) {
				break;
			}
		}
	}
}
void CMyStcGroup::Run2()
{
	while (!m_bExit) {
		GetGroupState();

		for (int i = 0; i < 250; i++) {
			Sleep(10);
			if (m_bExit) {
				break;
			}
		}
	}
}

void CMyStcGroup::OnSize(UINT nType, int cx, int cy)
{
	CStatic::OnSize(nType, cx, cy);

	CRect rc;
	GetClientRect(&rc);
}

BOOL CMyStcGroup::OnEraseBkgnd(CDC* pDC)
{
	CRect rc;
	GetClientRect(&rc);

	if (!rc.EqualRect(&m_rcClient)) {
		m_rcClient = rc;

		HDC hDC = ::GetDC(m_hWnd);

		///> main
		if (m_hMdMain) {
			::DeleteDC(m_hMdMain);
		}

		if (m_hBmpMain) {
			::DeleteObject(m_hBmpMain);
		}
		m_hMdMain = ::CreateCompatibleDC(hDC);
		m_hBmpMain = ::CreateCompatibleBitmap(hDC, rc.Width(), rc.Height());

		::SelectObject(m_hMdMain, m_hBmpMain);
		::SetBkMode(m_hMdMain, TRANSPARENT);

		///> sub
		if (m_hMdSub) {
			::DeleteDC(m_hMdSub);
		}

		if (m_hBmpSub) {
			::DeleteObject(m_hBmpSub);
		}
		m_hMdSub = ::CreateCompatibleDC(hDC);
		m_hBmpSub = ::CreateCompatibleBitmap(hDC, rc.Width(), rc.Height());

		::SelectObject(m_hMdSub, m_hBmpSub); // DC 에 BMP를 연결
		::SetBkMode(m_hMdSub, TRANSPARENT); // 텍스트는 TRANSPARENT로

		::ReleaseDC(m_hWnd, hDC);
		
		/*if (IsRunning() == false) {
			Start();
		}*/
	}

	return CStatic::OnEraseBkgnd(pDC);
}


void CMyStcGroup::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 __super::OnPaint()을(를) 호출하지 마십시오.

	CRect rc;
	GetClientRect(&rc);

	::BitBlt(dc.m_hDC, rc.left, rc.top, rc.Width(), rc.Height(), m_hMdMain, 0, 0, SRCCOPY);
}


/*rc.OffsetRect(0, nHeight);
strData.Format(_T("192.168.2.16"));
MyDrawText(strData, rc, m_hFontS, coText, DT_CENTER | DT_SINGLELINE | DT_VCENTER);*/