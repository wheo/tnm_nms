
// TnmNMSDlg.h : ��� ����
//

#pragma once
#include "SubLogDlg.h"
#include "SubServerMgrDlg.h"

// CTnmNMSDlg ��ȭ ����
class CTnmNMSDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CTnmNMSDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TNMNMS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedExit();
	afx_msg void OnBnClickedMin();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL DestroyWindow();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

protected:
	void CreateGUI();

	void GetNetworkInfo();

	/*
	�۷ι��� ����Ǿ��ִ� SNMPŬ�����κ��� ���Ź޾� �������� Ʈ���޽����� ������ �ͼ� ó���ϴ� �Լ�*/
	void GetWarningMsg();

protected:
	CSubLogDlg *m_pLogDlg;
	CSubServerMgrDlg	*m_pServerDlg;

	int m_nEthernetCount;
	CString m_strEthernetIP[100];

	CMyButton	m_cBtnExit;
	CMyButton	m_cBtnMin;

public:
	
	
	
};
