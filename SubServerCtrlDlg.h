#pragma once
#include "MyStcGroup.h"
#include "afxwin.h"

// CSubServerCtrlDlg 대화 상자입니다.

class CSubServerCtrlDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSubServerCtrlDlg)

public:
	CSubServerCtrlDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSubServerCtrlDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SUB_SERVER_CTRL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnGroupGroupadd();
	afx_msg void OnGroupGroupdelete();
	afx_msg void OnGroupGroupmodify();
	afx_msg void OnGroupServerclear();
	afx_msg void OnGroupErrorack();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

public:
	bool SaveServerState();
	bool LoadServerState();
	bool SavePreset(CString strFilePath);
	bool LoadPreset(CString strFilePath);

	void CreateAlram(int nGroupIndex, int nServerType, stWarningMsg *pAlramMsg);
protected:
	void ClickedGroup(int nIndex, CPoint ptClick);
		
protected:
	bool m_bExit;
	int m_nSelectedIndex;
	int m_nSelectedServer;

	CMyStcGroup	m_cStcGroup[MAX_GROUP_COUNT];
	CRect m_rcGroup[MAX_GROUP_COUNT];

public:	
	
	
	
	
	
	
};
