// SubLogHistoryDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "TnmNMS.h"
#include "SubLogHistoryDlg.h"
#include "afxdialogex.h"


// CSubLogHistoryDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CSubLogHistoryDlg, CDialogEx)

CSubLogHistoryDlg::CSubLogHistoryDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SUB_LOG_HISTORY_DIALOG, pParent)
{

}

CSubLogHistoryDlg::~CSubLogHistoryDlg()
{
}

void CSubLogHistoryDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	ClearLogData();
}

void CSubLogHistoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SUB_LOG_HISTORY_LIST, m_cListLog);
	DDX_Control(pDX, IDC_SUB_LOG_HISTORY_COMBO_LEVEL, m_cComboLevel);
}


BEGIN_MESSAGE_MAP(CSubLogHistoryDlg, CDialogEx)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SUB_LOG_HISTORY_LIST, &CSubLogHistoryDlg::OnNMCustomdrawSubLogHistoryList)
	ON_CBN_SELCHANGE(IDC_SUB_LOG_HISTORY_COMBO_LEVEL, &CSubLogHistoryDlg::OnCbnSelchangeSubLogHistoryComboLevel)
END_MESSAGE_MAP()


// CSubLogHistoryDlg �޽��� ó�����Դϴ�.
void CSubLogHistoryDlg::SetFilePath(CString strFilePath)
{
	m_strFilePath.Format(_T("%s"), strFilePath);
}

void CSubLogHistoryDlg::ClearLogData()
{
	POSITION pos = m_listLog.GetHeadPosition();
	while (pos) {
		stWarningMsg *pMsg = (stWarningMsg*)m_listLog.GetNext(pos);
		SAFE_DELETE(pMsg);
	}

	m_listLog.RemoveAll();
}

void CSubLogHistoryDlg::OpenFile()
{
	FILE *fp = _tfopen(m_strFilePath, L"rb");
	if (fp) {
		while (1) {
			stWarningMsg *pMsg = new stWarningMsg;

			int iRead = fread(pMsg, 1, sizeof(stWarningMsg), fp);
			if (iRead <= 0) {
				delete pMsg;
				break;
			}

			m_listLog.AddTail(pMsg);
		}
		fclose(fp);
	}
	
	ReDrawLog();
}

void CSubLogHistoryDlg::AddListItem(int nIndex, stWarningMsg *pMsg)
{
	CString strData;
	strData.Format(_T("%d"), nIndex + 1);
	m_cListLog.InsertItem(nIndex, strData);
	m_cListLog.SetItemText(nIndex, LOG_OFFSET_GROUP, pMsg->groupInfo.strGroupName);
	strData.Format(L"%s_%s", pMsg->serverInfo.strServerIP, pMsg->serverInfo.strServerName);
	m_cListLog.SetItemText(nIndex, LOG_OFFSET_SERVER, strData);
	m_cListLog.SetItemText(nIndex, LOG_OFFSET_DESCRIPTION, pMsg->strWarningDescription);
	m_cListLog.SetItemText(nIndex, LOG_OFFSET_LEVEL, pMsg->strWarningLevel);
	m_cListLog.SetItemText(nIndex, LOG_OFFSET_TIME1, pMsg->strWarningTime1);
	m_cListLog.SetItemText(nIndex, LOG_OFFSET_TIME2, pMsg->strWarningTime2);
	strData.Format(L"%d", pMsg->iLevelIndex);
	m_cListLog.SetItemText(nIndex, LOG_OFFSET_LEVEL_INDEX, strData);
}
void CSubLogHistoryDlg::ReDrawLog()
{
	int nIndex = 0;

	m_cListLog.DeleteAllItems();

	POSITION pos = m_listLog.GetHeadPosition();
	while (pos) {
		stWarningMsg *pMsg = (stWarningMsg*)m_listLog.GetNext(pos);
		if (!pMsg) {
			continue;
		}

	//	//> �������ǿ� ��ġ�ϴ� ��� ����
		if (CheckCondition(pMsg)) {
			AddListItem(nIndex, pMsg);
			nIndex++;
		}
	}
}
bool CSubLogHistoryDlg::CheckCondition(stWarningMsg *pMsg)
{
	bool bAdd = false;
	
	CString strData;
	TCHAR strGroupName[250];
	TCHAR strLevel[100];
	int nSTime, nETime, nIndex;
	

	//> 2. level
	nIndex = m_cComboLevel.GetCurSel();
	if (nIndex == 0) {
		bAdd = true;
	}
	else {		
		m_cComboLevel.GetWindowText(strLevel, 100);
		if (_tcscmp(pMsg->strWarningLevel, strLevel) != 0) {
			bAdd = false;
		}
		else {
			bAdd = true;
		}
	}
	return bAdd;
}

void CSubLogHistoryDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	CRect rc, rcCtrl;
	GetClientRect(&rc);

	rcCtrl.left = 38;
	rcCtrl.top = 22;
	rcCtrl.right = rcCtrl.left + 150;
	rcCtrl.bottom = rcCtrl.top + 200;	
	if (m_cComboLevel) {
		m_cComboLevel.MoveWindow(&rcCtrl);
	}

	if (m_cListLog) {
		m_cListLog.MoveWindow(rc.left + 3, rc.top + 70, rc.Width() - 6, rc.Height() - 75);
	}
}


BOOL CSubLogHistoryDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	MoveWindow(0, 0, 1920, 1080);

	m_cListLog.SetExtendedStyle(m_cListLog.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_cListLog.InsertColumn(LOG_OFFSET_INDEX, _T("#"), LVCFMT_LEFT, 0);
	m_cListLog.InsertColumn(LOG_OFFSET_GROUP, _T("Group"), LVCFMT_LEFT, 200);
	m_cListLog.InsertColumn(LOG_OFFSET_SERVER, _T("Server"), LVCFMT_LEFT, 200);
	m_cListLog.InsertColumn(LOG_OFFSET_LEVEL, _T("LEVEL"), LVCFMT_LEFT, 150);
	m_cListLog.InsertColumn(LOG_OFFSET_DESCRIPTION, _T("Description"), LVCFMT_LEFT, 700);
	m_cListLog.InsertColumn(LOG_OFFSET_TIME1, _T("Time"), LVCFMT_LEFT, 200);
	m_cListLog.InsertColumn(LOG_OFFSET_TIME2, _T("Time"), LVCFMT_LEFT, 200);
	m_cListLog.InsertColumn(LOG_OFFSET_LEVEL_INDEX, _T("LevleIndex"), LVCFMT_LEFT, 0);

	m_cComboLevel.AddString(_T("All Level"));
	for (int i = 1; i < 5; i++) {
		m_cComboLevel.AddString(g_strLevelName[i]);
	}
	m_cComboLevel.SetCurSel(0);

	m_nLevelIndex = 0;

	OpenFile();
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

BOOL CSubLogHistoryDlg::PreTranslateMessage(MSG* pMsg)
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


LRESULT CSubLogHistoryDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	return CDialogEx::WindowProc(message, wParam, lParam);
}

void CSubLogHistoryDlg::OnNMCustomdrawSubLogHistoryList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	if (pNMCD->dwDrawStage == CDDS_PREPAINT) {
		*pResult = (LRESULT)CDRF_NOTIFYITEMDRAW;
		return; // ���⼭ �Լ��� ���� ������ *pResult ���� �����ȴ�.
	}

	if (pNMCD->dwDrawStage == CDDS_ITEMPREPAINT) {
		// �� �� (row) �� �׷��� ��. ���⼭�� �����ϸ� �� ���� ��� �����ϰ� ������ �ȴ�.
		if (m_cListLog.GetItemData(pNMCD->dwItemSpec) == 0) {//dwItemSpec �� ���� �׷����� row index
			NMLVCUSTOMDRAW *pDraw = (NMLVCUSTOMDRAW*)(pNMHDR);
			CString strText = m_cListLog.GetItemText(pNMCD->dwItemSpec, LOG_OFFSET_LEVEL_INDEX);
			int nLevel = _ttoi(strText);

			//		pDraw->clrTextBk = RGB(240, 240, 240);
			if (nLevel == WARNING_LEVEL_DISABLE) {
				pDraw->clrText = RGB(160, 160, 160);
			}
			else if (nLevel == WARNING_LEVEL_INFORMATION) {
				pDraw->clrText = 0x0;
			}
			else if (nLevel == WARNING_LEVEL_WARNING) {
				pDraw->clrText = RGB(10, 10, 250);
			}
			else if (nLevel == WARNING_LEVEL_CRITICAL) {
				pDraw->clrText = RGB(250, 10, 10);
			}

			*pResult = (LRESULT)CDRF_NEWFONT;//���⼭ ������ sub-item �� ������� �ʴ´�.
											 //	*pResult = (LRESULT)CDRF_NOTIFYSUBITEMDRAW;//sub-item �� �����ϱ� ���ؼ�. 
			return;//���⼭ �ߴ��ؾ� *pResult ���� �����ȴ�.
		}
		else { // When all matrices are already made. 
			NMLVCUSTOMDRAW *pDraw = (NMLVCUSTOMDRAW*)(pNMHDR);
			pDraw->clrText = 0x0;
			pDraw->clrTextBk = RGB(255, 255, 196);
			*pResult = (LRESULT)CDRF_NEWFONT;
			return;
		}
	}
	else if (pNMCD->dwDrawStage == (CDDS_SUBITEM | CDDS_ITEMPREPAINT)) {
		// sub-item �� �׷����� ����. ������ *pResult �� CDRF_NOTIFYSUBITEMDRAW �� �ؼ� �����

		// �� �� �־��� ���̴�.

		/*NMLVCUSTOMDRAW *pDraw = (NMLVCUSTOMDRAW*)(pNMHDR);
		CString text = _lstDir.GetItemText(pNMCD->dwItemSpec, pDraw->iSubItem);
		if (text == _T("x")) {
		pDraw->clrText = 0xff;
		pDraw->clrTextBk = 0xf0fff0;
		}
		else {
		pDraw->clrText = 0x0;
		pDraw->clrTextBk = 0xffffff;
		}*/

		*pResult = (LRESULT)CDRF_NEWFONT; // �̷��� �ؾ� ������ ��� �׷�����.
		return;
	}
	*pResult = 0;
}

void CSubLogHistoryDlg::OnCbnSelchangeSubLogHistoryComboLevel()
{
	int nIndex = m_cComboLevel.GetCurSel();
	if (m_nLevelIndex != nIndex) {
		m_nLevelIndex = nIndex;

		ReDrawLog();
	}
}
