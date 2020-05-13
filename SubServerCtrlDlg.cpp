// SubServerCtrlDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TnmNMS.h"
#include "SubServerCtrlDlg.h"
#include "afxdialogex.h"

#include "PopupGroupInfoDlg.h"

#define SERVER_WIDTH	312
#define SERVER_HEIGHT	29


// CSubServerCtrlDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSubServerCtrlDlg, CDialogEx)

CSubServerCtrlDlg::CSubServerCtrlDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SUB_SERVER_CTRL, pParent)
{
	m_bExit = false;
}

CSubServerCtrlDlg::~CSubServerCtrlDlg()
{
	
}

void CSubServerCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	for (int i = 0; i < MAX_GROUP_COUNT; i++) {
		DDX_Control(pDX, IDC_SUB_SERVER_CTRL_STC_GROUP1+i, m_cStcGroup[i]);
	}
}


BEGIN_MESSAGE_MAP(CSubServerCtrlDlg, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_GROUP_GROUPADD, &CSubServerCtrlDlg::OnGroupGroupadd)
	ON_COMMAND(ID_GROUP_GROUPDELETE, &CSubServerCtrlDlg::OnGroupGroupdelete)
	ON_COMMAND(ID_GROUP_GROUPMODIFY, &CSubServerCtrlDlg::OnGroupGroupmodify)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_GROUP_SERVERCLEAR, &CSubServerCtrlDlg::OnGroupServerclear)
	ON_COMMAND(ID_GROUP_ERRORACK, &CSubServerCtrlDlg::OnGroupErrorack)
END_MESSAGE_MAP()


// CSubServerCtrlDlg 메시지 처리기입니다.


BOOL CSubServerCtrlDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rc, rcText;
	GetClientRect(&rc);

	//	CBrush guideBrush(RGB(150, 10, 10));
	CTnmMemDC dc(pDC, rc);

	dc.FillSolidRect(&rc, RGB(130, 130, 130));
	//	dc.FrameRect(&rc, &guideBrush);
	dc.MoveTo(rc.left, rc.top);
	dc.LineTo(rc.right, rc.top);
		

	return CDialogEx::OnEraseBkgnd(pDC);
}

void CSubServerCtrlDlg::CreateAlram(int nGroupIndex, int nServerType, stWarningMsg *pAlramMsg)
{
	if (m_cStcGroup[nGroupIndex]) {
		m_cStcGroup[nGroupIndex].CreateAlram(nServerType, pAlramMsg);
	}
}

bool CSubServerCtrlDlg::SaveServerState()
{
	CString strFilePath;
	TCHAR szFilter[] = _T("Tnm NMS (*.tlist) | *.tlist||");
	CFileDialog dlg(FALSE, _T("tlist"), NULL, OFN_HIDEREADONLY, szFilter);
	if (IDOK == dlg.DoModal()) {
		strFilePath = dlg.GetPathName();

		return SavePreset(strFilePath);			
	}

	return false;
}
bool CSubServerCtrlDlg::SavePreset(CString strFilePath)
{
	FILE *fp = _tfopen(strFilePath, L"wb");
	if (fp) {
		int nBufSize = sizeof(stGroupState);

		for (int i = 0; i < MAX_GROUP_COUNT; i++) {
			if (!g_pGroupState[i]) {
				fclose(fp);
				_d(L"[ServerCtrlDlg] Save Server File failed.. 그룹 정보가 없다. index: %d\n", i);
				return false;
			}

			int nWrite = fwrite(g_pGroupState[i], 1, nBufSize, fp);
			if (nWrite != nBufSize) {
				fclose(fp);
				_d(L"[ServerCtrlDlg] Save Server File failed.. 저장하는 사이즈가 상이하다. index: %d, write: %d, dst: %d\n", i, nWrite, nBufSize);
				return false;
			}
		}

		fclose(fp);
		return true;
	}

	return false;
}
bool CSubServerCtrlDlg::LoadServerState()
{
	CString strFilePath;
	TCHAR szFilter[] = _T("Tnm NMS (*.tlist) | *.tlist||");
	CFileDialog dlg(TRUE, _T("tlist"), NULL, OFN_HIDEREADONLY, szFilter);
	if (IDOK == dlg.DoModal()) {
		strFilePath = dlg.GetPathName();
		return LoadPreset(strFilePath);		
	}

	return false;
}
bool CSubServerCtrlDlg::LoadPreset(CString strFilePath)
{
	FILE *fp = _tfopen(strFilePath, L"rb");
	if (fp) {
		int nBufSize = sizeof(stGroupState);
		stGroupState stInfo;

		for (int i = 0; i < MAX_GROUP_COUNT; i++) {
			int nWrite = fread(&stInfo, 1, nBufSize, fp);
			if (nWrite != nBufSize) {
				fclose(fp);
				_d(L"[ServerCtrlDlg] Load Server File failed.. 불러오는 사이즈가 상이하다. index: %d, write: %d, dst: %d\n", i, nWrite, nBufSize);
				return false;
			}

			if (m_cStcGroup[i]) {
				bool bNull = false;
				CString strData;

				strData.Format(L"%s", stInfo.stGroupInfo.strGroupName);
				if (strData.IsEmpty()) {
					bNull = true;
				}

				strData.Format(L"%s", stInfo.stGroupInfo.stEncInfo.strServerIP);
				if (strData.IsEmpty()) {
					bNull = true;
				}

				strData.Format(L"%s", stInfo.stGroupInfo.stDecInfo.strServerIP);
				if (strData.IsEmpty()) {
					bNull = true;
				}

				if (bNull) {
					m_cStcGroup[i].SetGroupInfo(NULL);
				}
				else {
					m_cStcGroup[i].SetGroupInfo(&stInfo.stGroupInfo);
				}
			}
		}

		fclose(fp);
		return true;
	}

	return false;
}

BOOL CSubServerCtrlDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
		
	for (int i = 0; i < MAX_GROUP_COUNT; i++) {
		m_cStcGroup[i].SetWindowTextW(L"");
		m_cStcGroup[i].SetGroupState(i, g_pGroupState[i]);			
	}
	
	SetTimer(1, 3000, NULL);
	SetTimer(2, 100, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CSubServerCtrlDlg::ClickedGroup(int nIndex, CPoint ptClick)
{
	CMenu menu, *pSubMenu;

	menu.LoadMenu(IDR_MENU_POPUP);

	pSubMenu = menu.GetSubMenu(0);

	if (!m_cStcGroup[nIndex]) {
		return;
	}

	if (!g_pGroupState[nIndex]) {
		return;
	}

	bool bEnable = g_pGroupState[nIndex]->bCreated;
	
	//> 그룹상태 확인
	if (bEnable) {
		pSubMenu->EnableMenuItem(ID_GROUP_GROUPADD, MF_DISABLED);
		pSubMenu->EnableMenuItem(ID_GROUP_GROUPDELETE, MF_ENABLED);
		pSubMenu->EnableMenuItem(ID_GROUP_GROUPMODIFY, MF_ENABLED);
		pSubMenu->EnableMenuItem(ID_GROUP_SERVERCLEAR, MF_ENABLED);
		pSubMenu->EnableMenuItem(ID_GROUP_ERRORACK, MF_ENABLED);
	}
	else {
		pSubMenu->EnableMenuItem(ID_GROUP_GROUPADD, MF_ENABLED);
		pSubMenu->EnableMenuItem(ID_GROUP_GROUPDELETE, MF_DISABLED);
		pSubMenu->EnableMenuItem(ID_GROUP_GROUPMODIFY, MF_DISABLED);
		pSubMenu->EnableMenuItem(ID_GROUP_SERVERCLEAR, MF_DISABLED);
		pSubMenu->EnableMenuItem(ID_GROUP_ERRORACK, MF_DISABLED);
	}

	m_nSelectedIndex = nIndex;

	CRect rcTemp;
	rcTemp.CopyRect(&m_rcGroup[nIndex]);
	rcTemp.right = rcTemp.left + (rcTemp.Width() / 2);
	if (rcTemp.PtInRect(ptClick) == true) {
		m_nSelectedServer = SERVER_TYPE_ENCODER;
	}
	else {
		m_nSelectedServer = SERVER_TYPE_DECODER;
	}

	CPoint pt;
	pt.x = ptClick.x;
	pt.y = ptClick.y;

	ClientToScreen(&pt);

	pSubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, pt.x, pt.y, this);
}

void CSubServerCtrlDlg::OnGroupGroupadd()
{
	CPopupGroupInfoDlg	dlg;
	if (dlg.DoModal() == IDOK) {
		stGroupInfo info;
		dlg.GetGroupInfo(&info);

		if (m_cStcGroup[m_nSelectedIndex]) {
			m_cStcGroup[m_nSelectedIndex].SetGroupInfo(&info);

			GetParent()->PostMessageW(WM_TNM_SERVER_CTRL_GROUP_ADD, m_nSelectedIndex);
		}
//		_d(L"[ServerCtrlDlg] Stop\n");
	}
}


void CSubServerCtrlDlg::OnGroupGroupdelete()
{
	if (!g_pGroupState[m_nSelectedIndex]) {
		return;
	}

	CString strData;
	strData.Format(L"%s 그룹을 삭제하시겠습니까?", g_pGroupState[m_nSelectedIndex]->stGroupInfo.strGroupName);
	if (AfxMessageBox(strData, MB_YESNO) == IDYES) {
		if (m_cStcGroup[m_nSelectedIndex]) {
			m_cStcGroup[m_nSelectedIndex].SetGroupInfo(NULL);

			GetParent()->PostMessageW(WM_TNM_SERVER_CTRL_GROUP_DELETE, m_nSelectedIndex);
		}
//		_d(L"[ServerCtrlDlg] Stop\n");
	}
}


void CSubServerCtrlDlg::OnGroupGroupmodify()
{
	if (!g_pGroupState[m_nSelectedIndex]) {
		return;
	}

	CPopupGroupInfoDlg dlg;
	dlg.SetGroupInfo(&g_pGroupState[m_nSelectedIndex]->stGroupInfo);
	if (dlg.DoModal() == IDOK) {
		stGroupInfo info;
		dlg.GetGroupInfo(&info);

		if (m_cStcGroup[m_nSelectedIndex]) {
			m_cStcGroup[m_nSelectedIndex].SetGroupInfo(&info);

			GetParent()->PostMessageW(WM_TNM_SERVER_CTRL_GROUP_MODIFY, m_nSelectedIndex);
		}
	//	_d(L"[ServerCtrlDlg] Stop\n");
	}
}

void CSubServerCtrlDlg::OnGroupServerclear()
{
	if (m_cStcGroup[m_nSelectedIndex]) {
		m_cStcGroup[m_nSelectedIndex].ClearAlram(m_nSelectedServer);
	}
}

void CSubServerCtrlDlg::OnGroupErrorack()
{
	if (m_cStcGroup[m_nSelectedIndex]) {
		m_cStcGroup[m_nSelectedIndex].SetACK(m_nSelectedServer);
	}
}

BOOL CSubServerCtrlDlg::PreTranslateMessage(MSG* pMsg)
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


LRESULT CSubServerCtrlDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialogEx::WindowProc(message, wParam, lParam);
}

void CSubServerCtrlDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	CRect rc, rct, rcBase;
	GetClientRect(&rc);	

	int nIndex;
	rcBase.left = 5;
	rcBase.top = 6;
	rcBase.right = rcBase.left + SERVER_WIDTH;
	rcBase.bottom = rcBase.top + SERVER_HEIGHT;

	for (int i = 0; i < 20; i++) {
		nIndex = i * 5;

		rct.CopyRect(&rcBase);				

		for (int j = 0; j < 5; j++) {
			CRect rcTemp;
			if (m_cStcGroup[nIndex]) {				
				rcTemp.CopyRect(&rct);				
				m_cStcGroup[nIndex].MoveWindow(&rcTemp);	
				m_rcGroup[nIndex].CopyRect(&rcTemp);
			}
	//		_d(L"[serverctrldlg] test: %d (%d, %d, %d, %d)\n", nIndex, rcTemp.left, rcTemp.top, rcTemp.right, rcTemp.bottom);

			rct.OffsetRect(SERVER_WIDTH + 12, 0);
			nIndex++;
		}

		rcBase.OffsetRect(0, SERVER_HEIGHT);
			
		if ((i % 2) == 0) {
			rcBase.OffsetRect(0, 6);
		}
		else {
			rcBase.OffsetRect(0, 12);
		}		
	}
}

void CSubServerCtrlDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	//> 클릭 유무
	for (int i = 0; i < MAX_GROUP_COUNT; i++) {
		if (m_cStcGroup[i]) {			
			if (m_rcGroup[i].PtInRect(point) == true) {				
				ClickedGroup(i, point);
				break;
			}
		}
	}

	CDialogEx::OnRButtonDown(nFlags, point);
}

void CSubServerCtrlDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	for (int i = 0; i < MAX_GROUP_COUNT; i++) {
		if (m_cStcGroup[i]) {
			if (m_rcGroup[i].PtInRect(point) == true) {				
			
			}
		}
	}	

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CSubServerCtrlDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	for (int i = 0; i < MAX_GROUP_COUNT; i++) {
		if (m_cStcGroup[i]) {
			if (m_rcGroup[i].PtInRect(point) == true) {
				CRect rcTemp;
				rcTemp.CopyRect(&m_rcGroup[i]);

				rcTemp.right = rcTemp.left + (rcTemp.Width() / 2);
				if (g_pGroupState[i] && g_pGroupState[i]->bCreated) {
					CString strURL;
					if (rcTemp.PtInRect(point) == true) {
						strURL.Format(_T("http://%s"), g_pGroupState[i]->stGroupInfo.stEncInfo.strServerIP);
					}
					else {
						strURL.Format(_T("http://%s"), g_pGroupState[i]->stGroupInfo.stDecInfo.strServerIP);
					}
					ShellExecute(NULL, _T("open"), strURL, NULL, NULL, SW_SHOWNORMAL);
				}
				break;
			}
		}
	}	

	CDialogEx::OnLButtonDblClk(nFlags, point);
}

void CSubServerCtrlDlg::OnTimer(UINT_PTR nIDEvent)
{	
	if (nIDEvent == 1) {
#if 0
		for (int i = 0; i < MAX_GROUP_COUNT; i++) {
			if (m_cStcGroup[i] && g_pGroupState[i]) {
				m_cStcGroup[i].GetGroupState();
			}

			if (m_bExit) {
				break;
			}
		}
#endif
	}
	else if (nIDEvent == 2) {
		bool bWarning = false;
		for (int i = 0; i < MAX_GROUP_COUNT; i++) {
			if (m_cStcGroup[i] && g_pGroupState[i]) {
				if (g_pGroupState[i]->bCreated) {
					if (g_pGroupState[i]->stDecState.bAlarm || g_pGroupState[i]->stEncState.bAlarm) {
						bWarning = true;
						break;
					}
				}
			}

			if (m_bExit) {
				break;
			}
		}

		if (bWarning == false) {
			if (g_pAlarm) {
				if (g_pAlarm->IsAlarm()) {
					g_pAlarm->SetAlarm(false);
				}
			}
		}
	}


	CDialogEx::OnTimer(nIDEvent);
}


void CSubServerCtrlDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	m_bExit = true;

	KillTimer(1);
	KillTimer(2);	

	for (int i = 0; i < MAX_GROUP_COUNT; i++) {
		m_cStcGroup[i].Terminate();
	}
}





