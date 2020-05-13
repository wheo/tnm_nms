// SubLogDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TnmNMS.h"
#include "SubLogDlg.h"
#include "afxdialogex.h"
#include "XLEzAutomation.h"
#include "SubLogHistoryDlg.h"


// CSubLogDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSubLogDlg, CDialogEx)

CSubLogDlg::CSubLogDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SUB_LOG_DLG, pParent)
{
	m_nLevelIndex = 0;
}

CSubLogDlg::~CSubLogDlg()
{
}

void CSubLogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SUB_LOG_LIST_LOG, m_cListLog);
	DDX_Control(pDX, IDC_SUB_LOG_COMBO_LEVEL, m_cComboLevel);
	DDX_Control(pDX, IDC_SUB_LOG_BUTTON_SAVE, m_cBtnSave);
	DDX_Control(pDX, IDC_SUB_LOG_BUTTON_LOAD, m_cBtnLoad);
	DDX_Control(pDX, IDC_SUB_LOG_BUTTON_CLEAR, m_cBtnClear);
}


BEGIN_MESSAGE_MAP(CSubLogDlg, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_CONTROL_RANGE(BN_CLICKED, IDC_SUB_LOG_BTN_FULL, IDC_SUB_LOG_BTN_NORMAL, OnBnClickedMenu)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SUB_LOG_LIST_LOG, &CSubLogDlg::OnNMCustomdrawSubLogListLog)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_SUB_LOG_COMBO_LEVEL, &CSubLogDlg::OnCbnSelchangeSubLogComboLevel)
	ON_BN_CLICKED(IDC_SUB_LOG_BUTTON_SAVE, &CSubLogDlg::OnBnClickedSubLogButtonSave)
	ON_BN_CLICKED(IDC_SUB_LOG_BUTTON_LOAD, &CSubLogDlg::OnBnClickedSubLogButtonLoad)
	ON_BN_CLICKED(IDC_SUB_LOG_BUTTON_CLEAR, &CSubLogDlg::OnBnClickedSubLogButtonClear)
END_MESSAGE_MAP()

void CSubLogDlg::OnDestroy()
{
	CDialogEx::OnDestroy();	

	KillTimer(1);

	int nCount = m_listWarning.GetCount();
	if (nCount > 0) {
		CString strFilePath;
		TCHAR strFileName[500];
		CTime curTime = CTime::GetCurrentTime();

		GetModuleFileName(NULL, strFileName, MAX_PATH);

		strFilePath.Format(L"%s", strFileName);
		strFilePath.Format(L"%s\\History\\%04d%02d%02d_%02d%02d%02d.xlsx",
			strFilePath.Left(strFilePath.ReverseFind('\\')), curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(), curTime.GetHour(), curTime.GetMinute(), curTime.GetSecond());

//		SaveFile(strFilePath);
	}

	m_csLog.Lock();
	ClearLogData();
	m_csLog.Unlock();
}

// CSubLogDlg 메시지 처리기입니다.
void CSubLogDlg::OnBnClickedMenu(UINT uID)
{
	switch (uID) {
	case IDC_SUB_LOG_BTN_FULL:
		SizeChange(true);
		break;

	case IDC_SUB_LOG_BTN_NORMAL:
		SizeChange(false);
		break;

	/*case IDC_SUB_LOG_BUTTON_SAVE:
		SaveFile();
		break;

	case IDC_SUB_LOG_BUTTON_LOAD:
		LoadFile();
		break;
		*/
	}
}

void CSubLogDlg::SizeChange(bool bIsFull)
{
	if (bIsFull) {
		m_cBtnFull.ShowWindow(SW_HIDE);
		m_cBtnNormal.ShowWindow(SW_SHOW);
	}
	else {
		m_cBtnNormal.ShowWindow(SW_HIDE);
		m_cBtnFull.ShowWindow(SW_SHOW);
	}

	GetParent()->PostMessage(WM_TNM_SUB_LOG_SIZE_CHANGE, bIsFull);
}

BOOL CSubLogDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rc, rcText;
	GetClientRect(&rc);

	//	CBrush guideBrush(RGB(150, 10, 10));
	CTnmMemDC dc(pDC, rc);

	dc.FillSolidRect(&rc, g_colorTabBG);
	//	dc.FrameRect(&rc, &guideBrush);
	dc.MoveTo(rc.left, rc.top);
	dc.LineTo(rc.right, rc.top);
			
	rc.left = 15;
	rc.right = rc.left + 100;
	rc.top = 25;
	rc.bottom = rc.top + 20;
	dc.SetTextColor(RGB(10, 10, 10));
	dc.DrawText(L"정렬", &rc, DT_VCENTER);

	rc.OffsetRect(350, 0);	
	dc.DrawText(L"파일", &rc, DT_VCENTER);

	rc.OffsetRect(350, 0);
	dc.DrawText(L"리스트정리", &rc, DT_VCENTER);

	return false;

	return CDialogEx::OnEraseBkgnd(pDC);
}

BOOL CSubLogDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CRect rc;
	GetClientRect(&rc);

	m_cBtnFull.Create(_T(""), WS_VISIBLE | WS_CHILD, CRect(rc.right - 49, rc.top + 3, rc.right - 3, 36), this, IDC_SUB_LOG_BTN_FULL);
	m_cBtnFull.LoadImages(IDB_SUB_LOG_BTN_FULL, 3);

	m_cBtnNormal.Create(_T(""), WS_VISIBLE | WS_CHILD, CRect(rc.right - 49, rc.top + 3, rc.right - 3, 36), this, IDC_SUB_LOG_BTN_NORMAL);
	m_cBtnNormal.LoadImages(IDB_SUB_LOG_BTN_NORMAL, 3);
	m_cBtnNormal.ShowWindow(SW_HIDE);

	m_cListLog.SetExtendedStyle(m_cListLog.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_cListLog.InsertColumn(LOG_OFFSET_INDEX, _T("#"), LVCFMT_LEFT, 0);
	m_cListLog.InsertColumn(LOG_OFFSET_GROUP, _T("Group"), LVCFMT_LEFT, 200);
	m_cListLog.InsertColumn(LOG_OFFSET_SERVER, _T("Server"), LVCFMT_LEFT, 200);
	m_cListLog.InsertColumn(LOG_OFFSET_LEVEL, _T("LEVEL"), LVCFMT_LEFT, 150);
	m_cListLog.InsertColumn(LOG_OFFSET_DESCRIPTION, _T("Description"), LVCFMT_LEFT, 700);
	m_cListLog.InsertColumn(LOG_OFFSET_TIME1, _T("Start Time"), LVCFMT_LEFT, 200);
	m_cListLog.InsertColumn(LOG_OFFSET_TIME2, _T("End Time"), LVCFMT_LEFT, 200);
	m_cListLog.InsertColumn(LOG_OFFSET_LEVEL_INDEX, _T("Level"), LVCFMT_LEFT, 0);

	m_cComboLevel.AddString(_T("All Level"));
	for (int i = 1; i < 5; i++) {
		m_cComboLevel.AddString(g_strLevelName[i]);
	}
	m_cComboLevel.SetCurSel(0);

	m_cBtnLoad.ShowWindow(SW_HIDE);

	m_i64FileTime = GetTickCount();
	SetTimer(1, 1000, NULL);
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CSubLogDlg::PreTranslateMessage(MSG* pMsg)
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


void CSubLogDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	CRect rc, rcCtrl;
	GetClientRect(&rc);

	rcCtrl.left = 55;
	rcCtrl.top = 22;
	rcCtrl.right = rcCtrl.left + 150;
	rcCtrl.bottom = rcCtrl.top + 100;	
	if (m_cComboLevel) {
		m_cComboLevel.MoveWindow(&rcCtrl);
	}
	
	rcCtrl.CopyRect(&rc);
	rcCtrl.left = 410;
	rcCtrl.right = rcCtrl.left + 90;
	rcCtrl.top = 22;
	rcCtrl.bottom = rcCtrl.top + 25;

	if (m_cBtnSave) {
		m_cBtnSave.MoveWindow(&rcCtrl);
	}

	rcCtrl.OffsetRect(100, 0);
	/*if (m_cBtnLoad) {
		m_cBtnLoad.MoveWindow(&rcCtrl);
	}*/

	rcCtrl.OffsetRect(310, 0);
	if (m_cBtnClear) {
		m_cBtnClear.MoveWindow(&rcCtrl);
	}

	if (m_cBtnFull) {
		m_cBtnFull.MoveWindow(rc.right - 49, rc.top + 3, 46, 33);
	}
	if (m_cBtnNormal) {
		m_cBtnNormal.MoveWindow(rc.right - 49, rc.top + 3, 46, 33);
	}

	if (m_cListLog) {
		m_cListLog.MoveWindow(rc.left + 3, rc.top + 70, rc.Width() - 8, rc.Height() - 75);
	}
}

void CSubLogDlg::ReDrawLog()
{
	m_csLog.Lock();
	m_cListLog.DeleteAllItems();
	bool bCheck;
	bCheck = false;

	TCHAR strLevel[100];

	if (m_nLevelIndex == 0) {
	}
	else {
		bCheck = true;
		m_cComboLevel.GetWindowText(strLevel, 100);
	}

	int nIndex = 0;
	POSITION pos = m_listWarning.GetHeadPosition();
	while (pos) {
		bool bAdd = true;
		stWarningMsg *pMsg = (stWarningMsg*)m_listWarning.GetNext(pos);
		if (!pMsg) {
			continue;
		}

		if (bCheck) {
			if (_tcscmp(pMsg->strWarningLevel, strLevel) != 0) {
				bAdd = false;
			}
		}

		if (bAdd) {
			AddListItem(nIndex, pMsg);
			nIndex++;
		}
	}
	m_csLog.Unlock();
}

void CSubLogDlg::AddListItem(int nIndex, stWarningMsg *pMsg)
{
	CString strData;
	strData.Format(_T("%d"), nIndex + 1);
	m_cListLog.InsertItem(nIndex, strData);
	m_cListLog.SetItemText(nIndex, LOG_OFFSET_GROUP, pMsg->groupInfo.strGroupName);

//	strData.Format(L"%s_%s", pMsg->serverInfo.strServerIP, pMsg->serverInfo.strServerName);
	strData.Format(L"%s", pMsg->serverInfo.strServerName);
	m_cListLog.SetItemText(nIndex, LOG_OFFSET_SERVER, strData);

//	strData.Format(L"%s_%s", pMsg->strWarningDescription, pMsg->strWarningType);
	strData.Format(L"%s", pMsg->strWarningDescription);
	m_cListLog.SetItemText(nIndex, LOG_OFFSET_DESCRIPTION, strData);

	m_cListLog.SetItemText(nIndex, LOG_OFFSET_LEVEL, pMsg->strWarningLevel);
	m_cListLog.SetItemText(nIndex, LOG_OFFSET_TIME1, pMsg->strWarningTime1);

	strData.Format(L"%d", pMsg->iLevelIndex);
	m_cListLog.SetItemText(nIndex, LOG_OFFSET_LEVEL_INDEX, strData);
}

void CSubLogDlg::ModifyListItem(stWarningMsg *pMsg)
{
	//> 그룹, 서버, 설명이 같고, 종료시간이 표시 안된 항목 처리.
	int nCount = m_cListLog.GetItemCount();

	CString strData;
	for (int i = 0; i < nCount; i++) {
		strData = m_cListLog.GetItemText(i, LOG_OFFSET_TIME2);
		if (strData.IsEmpty() == false) {
			continue;
		}

		strData = m_cListLog.GetItemText(i, LOG_OFFSET_GROUP);
		if (strData.Compare(pMsg->groupInfo.strGroupName) != 0) {
			continue;
		}

		strData = m_cListLog.GetItemText(i, LOG_OFFSET_SERVER);
		if (strData.Compare(pMsg->serverInfo.strServerName) != 0) {
			continue;
		}

		strData = m_cListLog.GetItemText(i, LOG_OFFSET_DESCRIPTION);
		if (strData.Compare(pMsg->strWarningDescription) != 0) {
			continue;
		}

		m_cListLog.SetItemText(i, LOG_OFFSET_TIME2, pMsg->strWarningTime2);
	}
}


void CSubLogDlg::OnNMCustomdrawSubLogListLog(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	if (pNMCD->dwDrawStage == CDDS_PREPAINT) {
		*pResult = (LRESULT)CDRF_NOTIFYITEMDRAW;
		return; // 여기서 함수를 빠져 나가야 *pResult 값이 유지된다.
	}

	if (pNMCD->dwDrawStage == CDDS_ITEMPREPAINT) {
		// 한 줄 (row) 가 그려질 때. 여기서만 설정하면 한 줄이 모두 동일하게 설정이 된다.
		if (m_cListLog.GetItemData(pNMCD->dwItemSpec) == 0) {//dwItemSpec 이 현재 그려지는 row index
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

			*pResult = (LRESULT)CDRF_NEWFONT;//여기서 나가면 sub-item 이 변경되지 않는다.
											 //	*pResult = (LRESULT)CDRF_NOTIFYSUBITEMDRAW;//sub-item 을 변경하기 위해서. 
			return;//여기서 중단해야 *pResult 값이 유지된다.
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
		// sub-item 이 그려지는 순간. 위에서 *pResult 에 CDRF_NOTIFYSUBITEMDRAW 를 해서 여기로

		// 올 수 있었던 것이다.

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

		*pResult = (LRESULT)CDRF_NEWFONT; // 이렇게 해야 설정한 대로 그려진다.
		return;
	}

	*pResult = 0;
}

void CSubLogDlg::AddWarningMsg(stWarningMsg *pMsg)
{
	//> 전시조건에 맞는 메시지 리스트 전시
	bool bAdd;
	bAdd = false;	

	int nIndex = m_cComboLevel.GetCurSel();
	if (nIndex == 0) {
		bAdd = true;
	}
	else {
		CString strName;

		m_cComboLevel.GetWindowText(strName);

		if (_tcscmp(pMsg->strWarningLevel, strName) == 0) {
			bAdd = true;
		}
	}

	//> 리스트 컨트롤에 전시
	if (bAdd) {
		if (pMsg->nMsgType == WARNING_TYPE_BEGIN) {
			//> 메시지 추가
			AddListItem(0, pMsg);
		}
		else if (pMsg->nMsgType == WARNING_TYPE_END) {
			//> 종료시간 지정
			ModifyListItem(pMsg);
		}
	}

	//> 메시지 리스트에 보관
	m_csLog.Lock();
	if (pMsg->nMsgType == WARNING_TYPE_BEGIN) {
		stWarningMsg *pAddMsg = new stWarningMsg;
		memcpy(pAddMsg, pMsg, sizeof(stWarningMsg));
		m_listWarning.AddHead(pAddMsg);
	}
	else if (pMsg->nMsgType == WARNING_TYPE_END) {
		ModifyLogData(pMsg);
	}
	m_csLog.Unlock();
}

void CSubLogDlg::OnTimer(UINT_PTR nIDEvent)
{	
	if (nIDEvent == 1) {
		__int64 i64Cur = GetTickCount64();

		__int64 i64Diff = i64Cur - m_i64FileTime;

		//> 1주일.
		int nBase = 7 * 24 * 60 * 60 * 1000;
		if ((i64Diff < 0) || (nBase < i64Diff)) {
			m_i64FileTime = i64Cur;

			int nCount = m_listWarning.GetCount();
			if (nCount > 0) {
				CString strFilePath;
				TCHAR strFileName[500];
				CTime curTime = CTime::GetCurrentTime();

				GetModuleFileName(NULL, strFileName, MAX_PATH);

				strFilePath.Format(L"%s", strFileName);
				strFilePath.Format(L"%s\\History\\%04d%02d%02d_%02d%02d%02d.xlsx",
					strFilePath.Left(strFilePath.ReverseFind('\\')), curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(), curTime.GetHour(), curTime.GetMinute(), curTime.GetSecond());

				SaveFile(strFilePath);
			}

			m_csLog.Lock();
			ClearLogData();
			m_cListLog.DeleteAllItems();
			m_csLog.Unlock();
		}		
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CSubLogDlg::SaveFile(CString strFilePath)
{
	m_csLog.Lock();
#if 1
	CXLEzAutomation XL(FALSE); // FALSE: 처리 과정을 화면에 보이지 않는다

	XL.SetCellValue(1, 1, _T("Group"));
	XL.SetCellValue(2, 1, _T("Server Name"));
	XL.SetCellValue(3, 1, _T("Server IP"));
	XL.SetCellValue(4, 1, _T("LEVEL"));
	XL.SetCellValue(5, 1, _T("Description"));
	XL.SetCellValue(6, 1, _T("Start Time"));
	XL.SetCellValue(7, 1, _T("End Time"));

	///> 데이터 부분 입력
	int nIndex = 2;
	POSITION pos = m_listWarning.GetHeadPosition();
	while (pos) {
		stWarningMsg *pMsg = (stWarningMsg*)m_listWarning.GetNext(pos);
		if (pMsg) {
			CString strData;
			strData.Format(_T("%s"), pMsg->groupInfo.strGroupName);
			if (strData.IsEmpty()) {
				XL.SetCellValue(1, nIndex, _T("N/A"));
			}
			else {
				XL.SetCellValue(1, nIndex, strData);
			}

			XL.SetCellValue(2, nIndex, pMsg->serverInfo.strServerName);			
			XL.SetCellValue(3, nIndex, pMsg->serverInfo.strServerIP);
			
			XL.SetCellValue(4, nIndex, pMsg->strWarningLevel);
			XL.SetCellValue(5, nIndex, pMsg->strWarningDescription);
			XL.SetCellValue(6, nIndex, pMsg->strWarningTime1);			
			XL.SetCellValue(7, nIndex, pMsg->strWarningTime2);
			//strData.Format(_T("%s, %s, %s, %s, %s"),
			//	pMsg->groupInfo.strName, pMsg->serverInfo.strName, pMsg->strWarningLevel, pMsg->strWarningDescription, pMsg->strWarningTime);

			//char *struMsg = new char[5000];
			//memset(struMsg, 0, 5000);
			////	UnicodeToUTF8(strData, struMsg, 5000);
			//UnicodeToMByte(strData, struMsg, 5000);
			////fprintf(fp, "%s\n", struMsg);
			////_ftprintf(fp, _T("%s\n"), strData);
			//fprintf(fp, "%s\n", struMsg);
			//delete[] struMsg;
		}
		nIndex++;
	}

	//  엑셀 파일 저장
	if (XL.SaveFileAs(strFilePath) == false) {
		_d(L"[LogDlg] 로그파일 생성 실패. path: %s\n", strFilePath);
	}
	XL.ReleaseExcel();                             // 엑셀 파일 해제
#else
	FILE *fp = _tfopen(strFilePath, L"wb");
	if (fp) {
		POSITION pos = m_listWarning.GetHeadPosition();
		while (pos) {
			stWarningMsg *pMsg = (stWarningMsg*)m_listWarning.GetNext(pos);
			if (pMsg) {
				int iWrite = fwrite(pMsg, 1, sizeof(stWarningMsg), fp);
				if (iWrite <= 0) {
					break;
				}
			}
		}
		fclose(fp);
	}
#endif
	m_csLog.Unlock();
}

void CSubLogDlg::SaveFile()
{
	int nCount = m_listWarning.GetCount();
	if (nCount <= 0) {
		return;
	}

	CString strFilePath;
	TCHAR szFilter[] = _T("Tnm NMS Log (*.xlsx) | *.xlsx||");
	CFileDialog dlg(FALSE, _T("tlog"), NULL, OFN_HIDEREADONLY, szFilter);
	if (IDOK == dlg.DoModal()) {
		strFilePath = dlg.GetPathName();

		SaveFile(strFilePath);
	}
}

void CSubLogDlg::LoadFile()
{
	CString strFilePath;
	TCHAR szFilter[] = _T("Tnm NMS Log (*.tlog) | *.tlog||");
	CFileDialog dlg(TRUE, _T("tlog"), NULL, OFN_HIDEREADONLY, szFilter);
	if (IDOK == dlg.DoModal()) {
		strFilePath = dlg.GetPathName();
		//	_d(_T("[logdlg] file path: %s\n"), strFilePath);	

		CSubLogHistoryDlg logDlg;
		logDlg.SetFilePath(strFilePath);		
		logDlg.DoModal();
	}
}

void CSubLogDlg::ModifyLogData(stWarningMsg *pMsg)
{
	CString strData;
	POSITION pos = m_listWarning.GetHeadPosition();
	while (pos) {
		stWarningMsg *pSrcMsg = (stWarningMsg*)m_listWarning.GetNext(pos);
		if (!pSrcMsg) {
			continue;
		}

		strData.Format(_T("%s"), pSrcMsg->strWarningTime2);

		if (strData.IsEmpty() == false) {
			continue;
		}

		if (_tcscmp(pSrcMsg->groupInfo.strGroupName, pMsg->groupInfo.strGroupName) != 0) {
			continue;
		}

		if (_tcscmp(pSrcMsg->serverInfo.strServerName, pMsg->serverInfo.strServerName) != 0) {
			continue;
		}

		if (_tcscmp(pSrcMsg->strWarningDescription, pMsg->strWarningDescription) != 0) {
			continue;
		}

		_stprintf(pSrcMsg->strWarningTime2, _T("%s"), pMsg->strWarningTime2);
	}		
}

void CSubLogDlg::ClearLogData(bool bIsAll)
{
	//> 180911 - 해지된 메시지만 삭제하도록 변경
	if (bIsAll) {
		POSITION pos = m_listWarning.GetHeadPosition();
		while (pos) {
			stWarningMsg *pMsg = (stWarningMsg*)m_listWarning.GetNext(pos);
			SAFE_DELETE(pMsg);
		}

		m_listWarning.RemoveAll();
	}
	else {
		POSITION pos = m_listWarning.GetHeadPosition();
		POSITION posOld;
		while (pos) {
			posOld = pos;
			stWarningMsg *pMsg = (stWarningMsg*)m_listWarning.GetNext(pos);
			if (pMsg) {
				CString strTime;
				strTime.Format(_T("%s"), pMsg->strWarningTime2);
				if (strTime.IsEmpty() == false) {					
					SAFE_DELETE(pMsg);
					m_listWarning.RemoveAt(posOld);
				}
			}
		}		
	}
}




void CSubLogDlg::OnCbnSelchangeSubLogComboLevel()
{
	int nIndex = m_cComboLevel.GetCurSel();
	if (m_nLevelIndex != nIndex) {
		m_nLevelIndex = nIndex;

		ReDrawLog();
	}
}


void CSubLogDlg::OnBnClickedSubLogButtonSave()
{
	SaveFile();
}


void CSubLogDlg::OnBnClickedSubLogButtonLoad()
{
	LoadFile();
}


void CSubLogDlg::OnBnClickedSubLogButtonClear()
{
	if (AfxMessageBox(L"로그가 모두 삭제됩니다. 진행하시겠습니까??", MB_YESNO) != IDYES) {
		return;
	}

	//> 180911 - 해지된 메시지만 삭제하도록 변경
	m_csLog.Lock();
	ClearLogData(false);	
	m_csLog.Unlock();
	ReDrawLog();
}
