// PopupNetworkDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "TnmNMS.h"
#include "PopupNetworkDlg.h"
#include "afxdialogex.h"


// CPopupNetworkDlg ��ȭ �����Դϴ�.

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


// CPopupNetworkDlg �޽��� ó�����Դϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
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
