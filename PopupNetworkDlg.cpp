// PopupNetworkDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TnmNMS.h"
#include "PopupNetworkDlg.h"
#include "afxdialogex.h"


// CPopupNetworkDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPopupNetworkDlg, CDialogEx)

CPopupNetworkDlg::CPopupNetworkDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_POPUP_NETWORK_DIALOG, pParent)
{

}

CPopupNetworkDlg::~CPopupNetworkDlg()
{
}

void CPopupNetworkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SUB_NETWORK_COMBO_LIST, m_cComboIP);
}


BEGIN_MESSAGE_MAP(CPopupNetworkDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CPopupNetworkDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CPopupNetworkDlg 메시지 처리기입니다.
void CPopupNetworkDlg::SetIPInfo(int nIndex, CString strIP)
{
	_stprintf(m_strIpAddr[m_nIPCount], L"%s", strIP);
	m_nIPCount++;
}

wchar_t *CPopupNetworkDlg::GetIPInfo()
{
	return m_strIpAddr[m_nComboIndex];
}

void CPopupNetworkDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
}


BOOL CPopupNetworkDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	for (int i = 0; i<m_nIPCount; i++) {
		m_cComboIP.AddString(m_strIpAddr[i]);
	}

	m_cComboIP.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BOOL CPopupNetworkDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN) {
		switch (pMsg->wParam) {
		case VK_ESCAPE:
			return TRUE;
			break;

		case VK_RETURN:
			return TRUE;
			break;

		case VK_SPACE:
			return TRUE;
			break;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
