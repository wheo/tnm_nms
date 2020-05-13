#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CSubServerListDlg ��ȭ �����Դϴ�.

class CSubServerListDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSubServerListDlg)

public:
	CSubServerListDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CSubServerListDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SUB_SERVER_LIST };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
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
