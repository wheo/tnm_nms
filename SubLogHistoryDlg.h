#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CSubLogHistoryDlg ��ȭ �����Դϴ�.

class CSubLogHistoryDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSubLogHistoryDlg)

public:
	CSubLogHistoryDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CSubLogHistoryDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SUB_LOG_HISTORY_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:	
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnNMCustomdrawSubLogHistoryList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDestroy();

public:
	void SetFilePath(CString strFilePath);

protected:
	void ClearLogData();

	void OpenFile();

	void ReDrawLog();
	void AddListItem(int nIndex, stWarningMsg *pMsg);

	bool CheckCondition(stWarningMsg *pMsg);
protected:
	CString m_strFilePath;
	CListCtrl m_cListLog;
	CComboBox m_cComboLevel;

	CPtrList m_listLog;

	int m_nLevelIndex;
public:
	
	afx_msg void OnCbnSelchangeSubLogHistoryComboLevel();
};
