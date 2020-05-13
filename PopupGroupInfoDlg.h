#pragma once
#include "afxwin.h"


// CPopupGroupInfoDlg 대화 상자입니다.

class CPopupGroupInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPopupGroupInfoDlg)

public:
	CPopupGroupInfoDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPopupGroupInfoDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_POPUP_GROUP_INFO_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
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
