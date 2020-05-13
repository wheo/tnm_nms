// PopupGroupInfoDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TnmNMS.h"
#include "PopupGroupInfoDlg.h"
#include "afxdialogex.h"


// CPopupGroupInfoDlg 대화 상자입니다.

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


// CPopupGroupInfoDlg 메시지 처리기입니다.


BOOL CPopupGroupInfoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_cEditGroupName.SetWindowTextW(m_stGroupInfo.strGroupName);

	m_cEditEncoder[0].SetWindowTextW(m_stGroupInfo.stEncInfo.strServerName);
	m_cEditEncoder[1].SetWindowTextW(m_stGroupInfo.stEncInfo.strServerIP);

	m_cEditDecoder[0].SetWindowTextW(m_stGroupInfo.stDecInfo.strServerName);
	m_cEditDecoder[1].SetWindowTextW(m_stGroupInfo.stDecInfo.strServerIP);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
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
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialogEx::PreTranslateMessage(pMsg);
}


LRESULT CPopupGroupInfoDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialogEx::WindowProc(message, wParam, lParam);
}


void CPopupGroupInfoDlg::OnBnClickedOk()
{
	CString strData;

	m_cEditGroupName.GetWindowText(strData);
	if (strData.IsEmpty()) {
		AfxMessageBox(L"그룹 이름을 입력해 주세요");
		return;
	}
	_stprintf(m_stGroupInfo.strGroupName, L"%s", strData);

	m_cEditEncoder[0].GetWindowTextW(strData);
	if (strData.IsEmpty()) {
		AfxMessageBox(L"인코더 이름을 입력해 주세요");
		return;
	}
	_stprintf(m_stGroupInfo.stEncInfo.strServerName, L"%s", strData);
	m_cEditEncoder[1].GetWindowTextW(strData);
	if (strData.IsEmpty()) {
		AfxMessageBox(L"인코더 IP를 입력해 주세요");
		return;
	}
	_stprintf(m_stGroupInfo.stEncInfo.strServerIP, L"%s", strData);

	m_cEditDecoder[0].GetWindowTextW(strData);
	if (strData.IsEmpty()) {
		AfxMessageBox(L"디코더 이름을 입력해 주세요");
		return;
	}
	_stprintf(m_stGroupInfo.stDecInfo.strServerName, L"%s", strData);
	m_cEditDecoder[1].GetWindowTextW(strData);
	if (strData.IsEmpty()) {
		AfxMessageBox(L"디코더 IP를 입력해 주세요");
		return;
	}
	_stprintf(m_stGroupInfo.stDecInfo.strServerIP, L"%s", strData);

	CDialogEx::OnOK();
}
