
// TnmNMSDlg.h : 헤더 파일
//

#pragma once
#include "SubLogDlg.h"
#include "SubServerMgrDlg.h"

// CTnmNMSDlg 대화 상자
class CTnmNMSDlg : public CDialogEx
{
// 생성입니다.
public:
	CTnmNMSDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TNMNMS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
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
	글로벌로 선언되어있는 SNMP클래스로부터 수신받아 보관중인 트랩메시지를 가지고 와서 처리하는 함수*/
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
