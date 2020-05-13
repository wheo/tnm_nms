#pragma once
#include "afxwin.h"


// CPopupNetworkDlg ��ȭ �����Դϴ�.

class CPopupNetworkDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPopupNetworkDlg)

public:
	CPopupNetworkDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPopupNetworkDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_POPUP_NETWORK_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
