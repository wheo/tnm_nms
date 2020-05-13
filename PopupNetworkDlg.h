#pragma once
#include "afxwin.h"


// CPopupNetworkDlg 대화 상자입니다.

class CPopupNetworkDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPopupNetworkDlg)

public:
	CPopupNetworkDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPopupNetworkDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_POPUP_NETWORK_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	void SetIPInfo(int nIndex, CString strIP);

	wchar_t *GetIPInfo();
	int GetIPIndex() { return m_nComboIndex; };

protected:
	int m_nComboIndex;
	int m_nIPCount;
	wchar_t m_strIpAddr[100][100];

//	CComboBox m_cComboIP;
public:
	CComboBox m_cComboIP;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
