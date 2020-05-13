// SubServerListDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TnmNMS.h"
#include "SubServerListDlg.h"
#include "afxdialogex.h"


// CSubServerListDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSubServerListDlg, CDialogEx)

CSubServerListDlg::CSubServerListDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SUB_SERVER_LIST, pParent)
{

}

CSubServerListDlg::~CSubServerListDlg()
{
}

void CSubServerListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SUB_SERVER_LIST_TREE_LIST, m_cTreeServerList);
	DDX_Control(pDX, IDC_SUB_SERVER_LIST_BTN_SAVE, m_cBtnSave);
	DDX_Control(pDX, IDC_SUB_SERVER_LIST_BTN_LOAD, m_cBtnLoad);
}


BEGIN_MESSAGE_MAP(CSubServerListDlg, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_SUB_SERVER_LIST_BTN_SAVE, &CSubServerListDlg::OnBnClickedSubServerListBtnSave)
	ON_BN_CLICKED(IDC_SUB_SERVER_LIST_BTN_LOAD, &CSubServerListDlg::OnBnClickedSubServerListBtnLoad)
	ON_NOTIFY(NM_DBLCLK, IDC_SUB_SERVER_LIST_TREE_LIST, &CSubServerListDlg::OnNMDblclkSubServerListTreeList)
END_MESSAGE_MAP()


// CSubServerListDlg 메시지 처리기입니다.


BOOL CSubServerListDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return CDialogEx::OnEraseBkgnd(pDC);
}

void CSubServerListDlg::RedrawList()
{
	m_cTreeServerList.DeleteAllItems();

	m_treeRoot = m_cTreeServerList.InsertItem(L"Group", TVI_ROOT, TVI_LAST);

	for (int i = 0; i < MAX_GROUP_COUNT; i++) {
		if (g_pGroupState[i]) {
			if (g_pGroupState[i]->bCreated) {
				CString strData;

				stGroupInfo *pInfo = &g_pGroupState[i]->stGroupInfo;

				HTREEITEM group = m_cTreeServerList.InsertItem(pInfo->strGroupName, m_treeRoot, TVI_LAST);
				
			//	strData.Format(L"%s_%s", pInfo->stEncInfo.strServerIP, pInfo->stEncInfo.strServerName);
				strData.Format(L"%s", pInfo->stEncInfo.strServerName);
				m_cTreeServerList.InsertItem(strData, group, TVI_LAST);

			//	strData.Format(L"%s_%s", pInfo->stDecInfo.strServerIP, pInfo->stDecInfo.strServerName);
				strData.Format(L"%s", pInfo->stDecInfo.strServerName);
				m_cTreeServerList.InsertItem(strData, group, TVI_LAST);

				m_cTreeServerList.SetItemData(group, pInfo->nIndex);
			}
		}		
	}

	m_cTreeServerList.Expand(m_treeRoot, TVE_EXPAND);
}

void CSubServerListDlg::AddGroup(int nIndex)
{
	if (!g_pGroupState[nIndex]) {
		return;
	}

	if (g_pGroupState[nIndex]->bCreated) {
		CString strData;

		stGroupInfo *pInfo = &g_pGroupState[nIndex]->stGroupInfo;

		HTREEITEM group = m_cTreeServerList.InsertItem(pInfo->strGroupName, m_treeRoot, TVI_LAST);

	//	strData.Format(L"%s_%s", pInfo->stEncInfo.strServerIP, pInfo->stEncInfo.strServerName);
		strData.Format(L"%s", pInfo->stEncInfo.strServerName);
		m_cTreeServerList.InsertItem(strData, group, TVI_LAST);

//		strData.Format(L"%s_%s", pInfo->stDecInfo.strServerIP, pInfo->stDecInfo.strServerName);
		strData.Format(L"%s", pInfo->stDecInfo.strServerName);
		m_cTreeServerList.InsertItem(strData, group, TVI_LAST);

		m_cTreeServerList.SetItemData(group, pInfo->nIndex);

	//	m_cTreeServerList.Invalidate();
		m_cTreeServerList.Expand(m_treeRoot, TVE_EXPAND);
	}
}
void CSubServerListDlg::DeleteGroup(int nIndex)
{
	HTREEITEM group = m_cTreeServerList.GetChildItem(m_treeRoot);

	while (group) {
		int nGroupIndex = m_cTreeServerList.GetItemData(group);
		if (nIndex == nGroupIndex) {
			m_cTreeServerList.DeleteItem(group);
			break;
		}
		group = m_cTreeServerList.GetNextItem(group, TVGN_NEXT);
	}
}
void CSubServerListDlg::ModifyGroup(int nIndex)
{
	if (!g_pGroupState[nIndex]) {
		return;
	}

	CString strData;
	stGroupInfo *pInfo = &g_pGroupState[nIndex]->stGroupInfo;

	HTREEITEM group = m_cTreeServerList.GetChildItem(m_treeRoot);

	while (group) {
		int nGroupIndex = m_cTreeServerList.GetItemData(group);
		if (nIndex == nGroupIndex) {
			m_cTreeServerList.SetItemText(group, pInfo->strGroupName);
			
			HTREEITEM server = m_cTreeServerList.GetChildItem(group);
			if (server) {
//				strData.Format(L"%s_%s", pInfo->stEncInfo.strServerIP, pInfo->stEncInfo.strServerName);
				strData.Format(L"%s", pInfo->stEncInfo.strServerName);
				m_cTreeServerList.SetItemText(server, strData);
			}
			server = m_cTreeServerList.GetNextItem(server, TVGN_NEXT);
			if (server) {
				//strData.Format(L"%s_%s", pInfo->stDecInfo.strServerIP, pInfo->stDecInfo.strServerName);
				strData.Format(L"%s", pInfo->stDecInfo.strServerName);
				m_cTreeServerList.SetItemText(server, strData);
			}
			break;
		}
		group = m_cTreeServerList.GetNextItem(group, TVGN_NEXT);
	}
}

BOOL CSubServerListDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_treeRoot = m_cTreeServerList.InsertItem(L"Group", TVI_ROOT, TVI_LAST);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BOOL CSubServerListDlg::PreTranslateMessage(MSG* pMsg)
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


LRESULT CSubServerListDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialogEx::WindowProc(message, wParam, lParam);
}


void CSubServerListDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	CRect rc, rct;
	GetClientRect(&rc);

	int nWidth = rc.Width();
	int nHeight = rc.Height();

	if (m_cTreeServerList) {
		m_cTreeServerList.MoveWindow(rc.left + 10, rc.top + 10, nWidth - 20, nHeight - 55);
	}

	rct.left = (nWidth / 2) - 85;
	rct.top = nHeight - 35;
	rct.right = rct.left + 80;
	rct.bottom = rct.top + 25;
	if (m_cBtnSave) {				
		m_cBtnSave.MoveWindow(&rct);
	}
	rct.OffsetRect(90, 0);
	if (m_cBtnLoad) {
		m_cBtnLoad.MoveWindow(&rct);
	}
}


void CSubServerListDlg::OnBnClickedSubServerListBtnSave()
{
	GetParent()->PostMessageW(WM_TNM_SERVERLIST_SAVE);
}


void CSubServerListDlg::OnBnClickedSubServerListBtnLoad()
{
	GetParent()->PostMessageW(WM_TNM_SERVERLIST_LOAD);
}


void CSubServerListDlg::OnNMDblclkSubServerListTreeList(NMHDR *pNMHDR, LRESULT *pResult)
{
	HTREEITEM hItem = m_cTreeServerList.GetSelectedItem(); 
	if (hItem) {
		HTREEITEM hParent = m_cTreeServerList.GetParentItem(hItem);
		if (hParent) {
			if (hParent == m_treeRoot) {
				//_d(L"[ServerList] click group");
			}
			else {
		//		_d(L"[ServerList] click server");
				int nIndex = m_cTreeServerList.GetItemData(hParent);
				CString strServerName = m_cTreeServerList.GetItemText(hItem);
				if (g_pGroupState[nIndex] && g_pGroupState[nIndex]->bCreated) {
					CString strURL;
					if (strServerName.Compare(g_pGroupState[nIndex]->stGroupInfo.stEncInfo.strServerName) == 0) {
						strURL.Format(_T("http://%s"), g_pGroupState[nIndex]->stGroupInfo.stEncInfo.strServerIP);
					}
					else {
						strURL.Format(_T("http://%s"), g_pGroupState[nIndex]->stGroupInfo.stDecInfo.strServerIP);
					}
					ShellExecute(NULL, _T("open"), strURL, NULL, NULL, SW_SHOWNORMAL);					
				}				
			}
		}		
	}


	*pResult = 0;
}
