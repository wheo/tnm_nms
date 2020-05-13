#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CSubServerListDlg 대화 상자입니다.

class CSubServerListDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSubServerListDlg)

public:
	CSubServerListDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSubServerListDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SUB_SERVER_LIST };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedSubServerListBtnSave();
	afx_msg void OnBnClickedSubServerListBtnLoad();
	afx_msg void OnNMDblclkSubServerListTreeList(NMHDR *pNMHDR, LRESULT *pResult);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

public:
	void RedrawList();

	void AddGroup(int nIndex);
	void DeleteGroup(int nIndex);
	void ModifyGroup(int nIndex);
protected:

	CTreeCtrl m_cTreeServerList;
	HTREEITEM	m_treeRoot;
	
	CButton m_cBtnSave;
	CButton m_cBtnLoad;
	
public:
	
};
