#pragma once
#include "afxwin.h"


// CPopupGroupInfoDlg ��ȭ �����Դϴ�.

class CPopupGroupInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPopupGroupInfoDlg)

public:
	CPopupGroupInfoDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPopupGroupInfoDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_POPUP_GROUP_INFO_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

public:
	void SetGroupInfo(stGroupInfo *pInfo);
	void GetGroupInfo(stGroupInfo *pInfo);

protected:	
	CEdit m_cEditGroupName;
	CEdit m_cEditEncoder[2];
	CEdit m_cEditDecoder[2];

	stGroupInfo	m_stGroupInfo;
public:
	afx_msg void OnBnClickedOk();
};
