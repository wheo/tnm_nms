#pragma once
#include "afxwin.h"

#define	MAX_ERROR_MSG_COUNT	5000

struct stErrorMsg
{
	int	nIsEnable;
	stWarningMsg	warningMsg;
};

class CMyStcGroup :
	public CStatic, public CTnmDoubleThread
{
public:
	CMyStcGroup();
	~CMyStcGroup();

	void SetGroupState(int nIndex, stGroupState *pState);
	
	void SetGroupInfo(stGroupInfo *pInfo);	
	void GetGroupState();

	void CreateAlram(int nServerType, stWarningMsg *pAlramMsg);	
	void ClearAlram(int nServer);
	void SetACK(int nServer);

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();

private:
	void ClearAlram();

protected:
	void Run1();
	void Run2();

	void DrawGroup();

	void MyDrawText(CString strText, CRect rcText, HFONT hFont, COLORREF txtColor, UINT uiFormat);

	int CheckEncoderState(TCHAR *strServerIP);
	int CheckDecoderState(TCHAR *strServerIP);

	void AddErrorMsg(int nServerType, stWarningMsg *pAlramMsg);
	void DeleteErrorMsg(int nServerType, stWarningMsg *pAlramMsg);

protected:
	int m_nIndex;

	CCriticalSection m_csData;
	CCriticalSection m_csErrorMsg;

	stGroupState	*m_pGroupState;

	stWarningMsg		*m_encError[MAX_ERROR_MSG_COUNT];
	stWarningMsg		*m_decError[MAX_ERROR_MSG_COUNT];

	int m_nEncErrorMsgCount;
	int m_nDecErrorMsgCount;

	CRect m_rcClient; ///> Ŭ���̾�Ʈ�� ũ��

	HDC		m_hMdMain; ///> ���� �޸� DC - ��Ʈ��ũ���� �޸� DC
	HBITMAP m_hBmpMain; ///> �޸� DC�� ���� �޸�

	HDC		m_hMdSub; ///> �������� �׷��� �޸� DC - ��ü ��ǥ ũ���� �޸� DC 
	HBITMAP m_hBmpSub;

	HBRUSH m_hBrushBGDisconnect;
	HBRUSH m_hBrushBGRunning;
	HBRUSH m_hBrushBGError;
	HBRUSH m_hBrushBGGroup;
	HBRUSH m_hBrushBG;

	HFONT m_hFontS; ///> �ؽ�Ʈ�� ��Ʈ	
public:
	
};

