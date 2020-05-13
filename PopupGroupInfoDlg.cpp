// PopupGroupInfoDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "TnmNMS.h"
#include "PopupGroupInfoDlg.h"
#include "afxdialogex.h"


// CPopupGroupInfoDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CPopupGroupInfoDlg, CDialogEx)

CPopupGroupInfoDlg::CPopupGroupInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_POPUP_GROUP_INFO_DLG, pParent)
{
	memset(&m_stGroupInfo, 0, sizeof(stGroupInfo));
}

CPopupGroupInfoDlg::~CPopupGroupInfoDlg()
{
}

void CPopupGroupInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_POPUP_GROUP_INFO_EDIT_GROUP_NAME, m_cEditGroupName);
	for (int i = 0; i < 2; i++) {
		DDX_Control(pDX, IDC_POPUP_GROUP_INFO_EDIT_ENCODER1+i, m_cEditEncoder[i]);
		DDX_Control(pDX, IDC_POPUP_GROUP_INFO_EDIT_DECODER1+i, m_cEditDecoder[i]);
	}
}


BEGIN_MESSAGE_MAP(CPopupGroupInfoDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CPopupGroupInfoDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CPopupGroupInfoDlg �޽��� ó�����Դϴ�.


BOOL CPopupGroupInfoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_cEditGroupName.SetWindowTextW(m_stGroupInfo.strGroupName);

	m_cEditEncoder[0].SetWindowTextW(m_stGroupInfo.stEncInfo.strServerName);
	m_cEditEncoder[1].SetWindowTextW(m_stGroupInfo.stEncInfo.strServerIP);

	m_cEditDecoder[0].SetWindowTextW(m_stGroupInfo.stDecInfo.strServerName);
	m_cEditDecoder[1].SetWindowTextW(m_stGroupInfo.stDecInfo.strServerIP);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CPopupGroupInfoDlg::SetGroupInfo(stGroupInfo *pInfo)
{
	memcpy(&m_stGroupInfo, pInfo, sizeof(stGroupInfo));
}

void CPopupGroupInfoDlg::GetGroupInfo(stGroupInfo *pInfo)
{
	memcpy(pInfo, &m_stGroupInfo, sizeof(stGroupInfo));
}

BOOL CPopupGroupInfoDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	return CDialogEx::PreTranslateMessage(pMsg);
}


LRESULT CPopupGroupInfoDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	return CDialogEx::WindowProc(message, wParam, lParam);
}


void CPopupGroupInfoDlg::OnBnClickedOk()
{
	CString strData;

	m_cEditGroupName.GetWindowText(strData);
	if (strData.IsEmpty()) {
		AfxMessageBox(L"�׷� �̸��� �Է��� �ּ���");
		return;
	}
	_stprintf(m_stGroupInfo.strGroupName, L"%s", strData);

	m_cEditEncoder[0].GetWindowTextW(strData);
	if (strData.IsEmpty()) {
		AfxMessageBox(L"���ڴ� �̸��� �Է��� �ּ���");
		return;
	}
	_stprintf(m_stGroupInfo.stEncInfo.strServerName, L"%s", strData);
	m_cEditEncoder[1].GetWindowTextW(strData);
	if (strData.IsEmpty()) {
		AfxMessageBox(L"���ڴ� IP�� �Է��� �ּ���");
		return;
	}
	_stprintf(m_stGroupInfo.stEncInfo.strServerIP, L"%s", strData);

	m_cEditDecoder[0].GetWindowTextW(strData);
	if (strData.IsEmpty()) {
		AfxMessageBox(L"���ڴ� �̸��� �Է��� �ּ���");
		return;
	}
	_stprintf(m_stGroupInfo.stDecInfo.strServerName, L"%s", strData);
	m_cEditDecoder[1].GetWindowTextW(strData);
	if (strData.IsEmpty()) {
		AfxMessageBox(L"���ڴ� IP�� �Է��� �ּ���");
		return;
	}
	_stprintf(m_stGroupInfo.stDecInfo.strServerIP, L"%s", strData);

	CDialogEx::OnOK();
}
