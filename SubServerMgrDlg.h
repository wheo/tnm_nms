#pragma once
#include "SubServerCtrlDlg.h"
#include "SubServerListDlg.h"

// CSubServerMgrDlg ��ȭ �����Դϴ�.

class CSubServerMgrDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSubServerMgrDlg)

public:
	CSubServerMgrDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CSubServerMgrDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SUB_SERVER_MGR_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

public :
	void CreateAlram(int nGroupIndex, int nServerType, stWarningMsg *pAlramMsg);

protected:
	CSubServerCtrlDlg *m_pServerCtrlDlg;
	CSubServerListDlg *m_pServerListDlg;

public:
	
	
};
