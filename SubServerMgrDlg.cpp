// SubServerMgrDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TnmNMS.h"
#include "SubServerMgrDlg.h"
#include "afxdialogex.h"


// CSubServerMgrDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSubServerMgrDlg, CDialogEx)

CSubServerMgrDlg::CSubServerMgrDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SUB_SERVER_MGR_DLG, pParent)
{
	m_pServerCtrlDlg = NULL;
	m_pServerListDlg = NULL;
}

CSubServerMgrDlg::~CSubServerMgrDlg()
{
	if (m_pServerCtrlDlg) {
		m_pServerCtrlDlg->SavePreset(L"DefaultPreset.tlist");
	}
	
	SAFE_DELETE(m_pServerCtrlDlg);
	SAFE_DELETE(m_pServerListDlg);
}

void CSubServerMgrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSubServerMgrDlg, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CSubServerMgrDlg 메시지 처리기입니다.
BOOL CSubServerMgrDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_pServerListDlg = new CSubServerListDlg(this);
	m_pServerListDlg->Create(IDD_SUB_SERVER_LIST, this);
	m_pServerListDlg->ShowWindow(SW_SHOW);

	m_pServerCtrlDlg = new CSubServerCtrlDlg(this);
	m_pServerCtrlDlg->Create(IDD_SUB_SERVER_CTRL, this);
	m_pServerCtrlDlg->ShowWindow(SW_SHOW);	

	//> 180603 - 시작시 직전 프리셋 로드
	if (m_pServerCtrlDlg) {
		if (m_pServerCtrlDlg->LoadPreset(L"DefaultPreset.tlist")) {
			m_pServerListDlg->RedrawList();
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CSubServerMgrDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rc, rcText;
	GetClientRect(&rc);

	//	CBrush guideBrush(RGB(150, 10, 10));
	CTnmMemDC dc(pDC, rc);

	dc.FillSolidRect(&rc, g_colorTabBG);
	//	dc.FrameRect(&rc, &guideBrush);
	dc.MoveTo(rc.left, rc.top);
	dc.LineTo(rc.right, rc.top);

	//	_d(_T("[sererdlg] erase bkgnd\n"));
	return false;

	return CDialogEx::OnEraseBkgnd(pDC);
}

void CSubServerMgrDlg::CreateAlram(int nGroupIndex, int nServerType, stWarningMsg *pAlramMsg)
{
	if (m_pServerCtrlDlg) {
		m_pServerCtrlDlg->CreateAlram(nGroupIndex, nServerType, pAlramMsg);
	}
}

BOOL CSubServerMgrDlg::PreTranslateMessage(MSG* pMsg)
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


LRESULT CSubServerMgrDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_TNM_SERVERLIST_SAVE) {
		if (m_pServerCtrlDlg) {
			m_pServerCtrlDlg->SaveServerState();
		}
	}
	else if (message == WM_TNM_SERVERLIST_LOAD) {
		if (m_pServerCtrlDlg) {
			if (m_pServerCtrlDlg->LoadServerState()) {
				m_pServerListDlg->RedrawList();
			}
		}
	}
	else if (message == WM_TNM_SERVER_CTRL_GROUP_ADD) {
		int nIndex = (int)wParam;
		if (m_pServerListDlg) {
			m_pServerListDlg->AddGroup(nIndex);
		}
	}
	else if (message == WM_TNM_SERVER_CTRL_GROUP_DELETE) {
		int nIndex = (int)wParam;
		if (m_pServerListDlg) {
			m_pServerListDlg->DeleteGroup(nIndex);
		}
	}
	else if (message == WM_TNM_SERVER_CTRL_GROUP_MODIFY) {
		int nIndex = (int)wParam;
		if (m_pServerListDlg) {
			m_pServerListDlg->ModifyGroup(nIndex);
		}
	}

	return CDialogEx::WindowProc(message, wParam, lParam);
}

void CSubServerMgrDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	CRect rc;
	GetClientRect(&rc);

	if (m_pServerListDlg) {
		m_pServerListDlg->MoveWindow(rc.left, rc.top, 300, rc.Height());
	}

	if (m_pServerCtrlDlg) {
		m_pServerCtrlDlg->MoveWindow(rc.left+300, rc.top, rc.Width()-300, rc.Height());
	}	
}
