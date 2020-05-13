#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CSubLogDlg 대화 상자입니다.

class CSubLogDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSubLogDlg)

public:
	CSubLogDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSubLogDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SUB_LOG_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedMenu(UINT uID);	
	afx_msg void OnNMCustomdrawSubLogListLog(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnCbnSelchangeSubLogComboLevel();
	afx_msg void OnBnClickedSubLogButtonSave();
	afx_msg void OnBnClickedSubLogButtonLoad();
	afx_msg void OnBnClickedSubLogButtonClear();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()

public:
	void AddWarningMsg(stWarningMsg *pMsg);

protected:
	void SizeChange(bool bIsFull);		
	
	void AddListItem(int nIndex, stWarningMsg *pMsg);
	void ModifyListItem(stWarningMsg *pMsg);

	void SaveFile(CString strFilePath);
	void SaveFile();
	void LoadFile();

	void ReDrawLog();

	void ClearLogData(bool bIsAll = true);
	void ModifyLogData(stWarningMsg *pMsg);

protected:
	CComboBox m_cComboLevel;
	CMyButton m_cBtnFull;
	CMyButton m_cBtnNormal;

	CButton m_cBtnSave;
	CButton m_cBtnLoad;
	CButton m_cBtnClear;

	int m_nLevelIndex;

	CListCtrl m_cListLog;

	CPtrList m_listWarning;
	CCriticalSection m_csLog;

	__int64 m_i64FileTime;
public:
	
	
	
	
	
	
	
	
	
	
	
};
