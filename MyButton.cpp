// MyButton.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MyButton.h"

// CMyButton

IMPLEMENT_DYNAMIC(CMyButton, CButton)

CMyButton::CMyButton()
{
	m_nState = 0;
	m_nPrevState = -1;

	m_bEnable = true;
	m_bManual = false;

	m_bIsTracking = false;
	m_bIsToggleButton = false;

	m_bIsDisabled = false;
	m_bRedraw = false;

	m_strText = "";
	m_dwTxtFlags = SS_CENTER;
}

CMyButton::~CMyButton()
{
}


BEGIN_MESSAGE_MAP(CMyButton, CButton)
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
//	ON_WM_PAINT()
END_MESSAGE_MAP()



// CMyButton 메시지 처리기입니다.

void CMyButton::PreSubclassWindow()
{
	ModifyStyle(0, BS_OWNERDRAW);

	CButton::PreSubclassWindow();
}

void CMyButton::SetText(CString strText, COLORREF cbText)
{
	m_strText = strText;
	m_cbText = cbText;

	m_bRedraw = true;

	Invalidate(); 
}

void CMyButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CRect rcItem (lpDrawItemStruct->rcItem);

	if (m_nPrevState != m_nState || m_bRedraw) {
		DrawBitmap(pDC, m_hBmp, rcItem);
		m_nPrevState = m_nState;
		m_bRedraw = true;
	}

	if (!m_strText.IsEmpty() && m_bRedraw) {
		CTnmMemDC memDC(pDC, rcItem, true);
		
		CFont *pOldFont = memDC.SelectObject(&m_font);
		COLORREF rgbText = memDC.SetTextColor(m_cbText);

		DrawText(&memDC, &rcItem, m_strText);

		memDC.SelectObject(pOldFont);
		memDC.SetTextColor(rgbText);

		m_bRedraw = false;
	}

}

void CMyButton::DrawBitmap(CDC *pDC, HBITMAP hBmp, CRect rc, BOOL bStretch)
{
	CDC srcdc;
	CDC memdc;
	CDC maskdc;

	if (!hBmp) return;
	
	memdc.CreateCompatibleDC(pDC);
	memdc.SelectObject(hBmp);

	// Select bitmap into memory DC.
	if (memdc.m_hDC && pDC)
	{
		ASSERT(hBmp);
		HBITMAP* pBmpOld = (HBITMAP*) ::SelectObject(memdc.m_hDC, hBmp);

		if (!bStretch)
		{
			pDC->BitBlt(rc.left, rc.top, rc.Width(), rc.Height(), &memdc, m_nWidth*m_nState, 0, SRCCOPY);
		}
		else
		{	
			BITMAP bmp; 
			CBitmap TempBmp;
			CBitmap *pTempBmp;
			pTempBmp = TempBmp.FromHandle(hBmp);
			pTempBmp->GetBitmap(&bmp);
			pDC->StretchBlt(rc.left, rc.top, rc.Width(), rc.Height(), &memdc, 0, 0,bmp.bmWidth, bmp.bmHeight, SRCCOPY); 
		}
		DeleteDC(memdc);
	}
}

bool CMyButton::IsASingleWord(const CString & csText)
{
#if _OLD_
	char	cEnd = 0;

	cEnd = csText.GetAt(csText.GetLength() - 1);
	if ( ( csText.Find(" ") == -1 ) || (cEnd == ' ') )
		return TRUE;
	else
		return FALSE;
#else
	return false;
#endif
}

void CMyButton::DrawText(CDC* pDCMem, CRect* pRect, CString csText)
{
	DWORD dwStyle = m_dwTxtFlags;
	DWORD dwFlags = 0;

	// Map "Static Styles" to "Text Styles" - WARNING MACROS
#define MAP_STYLE(src, dest) if(dwStyle & (src)) dwFlags |= (dest)
#define NMAP_STYLE(src, dest) if(!(dwStyle & (src))) dwFlags |= (dest)

	MAP_STYLE(	SS_RIGHT,			DT_RIGHT   					);
	MAP_STYLE(	SS_CENTER,			DT_CENTER					);
	MAP_STYLE(	SS_LEFT,			DT_LEFT					    );
	//MAP_STYLE(	SS_CENTERIMAGE,		DT_VCENTER | DT_SINGLELINE	);
	MAP_STYLE(	SS_NOPREFIX,		DT_NOPREFIX					);
	MAP_STYLE(	SS_WORDELLIPSIS,	DT_WORD_ELLIPSIS			);
	MAP_STYLE(	SS_ENDELLIPSIS,		DT_END_ELLIPSIS				);
	MAP_STYLE(	SS_PATHELLIPSIS,	DT_PATH_ELLIPSIS			);

	// TAb expansion
//	if (csText.Find( _T('\t') ) != -1)
//		dwFlags |= DT_EXPANDTABS;

	//csText.Replace(
	// TEXT WRAPPING - Inspired by Chen-Cha Hsu and improved
	CRect		newRC;
	TEXTMETRIC	tag;
	CSize		sz;
	::GetTextExtentPoint32(pDCMem->GetSafeHdc(), csText,csText.GetLength(), &sz);
	CStringArray	asText;
	int				nLine = 0;
	CString			s2;
	int				nX = 0;
	int				nId = 0;
	char			nCR = 0;

	if ( 1 )
	{
		for (int i = 1; i <= csText.GetLength(); i++)
		{
			s2 = csText.Left(i);
			//CARRIAGE RETURN not recognised with SS_CENTERIMAGE - manual handling
			if (csText.GetAt(i-1) == '\r' || csText.GetAt(i-1) == '\n')
			{
				if (nCR == 0)
				{
					nCR = csText.GetAt(i-1);
				}
				else if (nCR != csText.GetAt(i-1))
				{ // "\r\n" or "\n\r"
					s2 = csText.Left(i-2);
					asText.Add(s2);
					csText = csText.Mid(i);
					i = nCR = 0;
				}
			}
			::GetTextExtentPoint32(pDCMem->GetSafeHdc(), s2, s2.GetLength(), &sz);
			if ( sz.cx > pRect->Width() ){// We found how many letters fits
				s2 = csText.Left(i-1);
				if ( IsASingleWord(s2) ){
					asText.Add(s2);
					csText = csText.Mid(i-1);
					i = 0;
				}
				else{ // Do not break a word
					nId = s2.ReverseFind(' ');
					s2 = s2.Left(nId);
					asText.Add(s2);
					csText = csText.Mid(nId + 1);
					i = 0;
				}
			}
		}
		if ( ! csText.IsEmpty() )
			asText.Add(csText);
	}
	else{// Standard CStatic behaviour
		asText.SetAtGrow(0, csText);
	}
	
	nLine = asText.GetSize();
	pDCMem->GetTextMetrics(&tag);
	newRC = *pRect;

	LONG nDiff = pRect->bottom - pRect->top - tag.tmHeight * nLine;
	if (1)//dwStyle & SS_CENTERIMAGE)
		pRect->top = nDiff/2;
#if 0
	for (int j = 0; j < nLine; j++)
	{
		newRC.top = pRect->top + tag.tmHeight * j;

		int width = lstrlen(asText[j])*((m_nFontSize + 1)/2);
		int left = newRC.Width()/2 - width/2;

		if (left < 0)
			left = newRC.left + 4;

		pDCMem->DrawText(asText[j], &newRC, dwFlags);
	}
#else
	GetClientRect(&newRC);
	newRC.OffsetRect(0, 4);
	newRC.DeflateRect(6, 2);

	pDCMem->DrawText(asText[0], &newRC, dwFlags);

#endif
}

void CMyButton::SetFont(const CString& strFont,int nPointSize, int nWeight,
					   BOOL bRedraw /*= TRUE*/)
{
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));       // Zero out the structure.

	_tcscpy(lf.lfFaceName,strFont);			// Set Font Familly
	lf.lfHeight = nPointSize;// Set Height
	lf.lfWeight = nWeight;					// Set Weight

	SetFont(&lf, bRedraw);

	//m_csFontFamilly = strFont;
	//m_nFontSizeVar = m_nFontSize = nPointSize;
	m_nFontSize = nPointSize;
}

void CMyButton::SetFont(LOGFONT *pLogFont, BOOL bRedraw /*= TRUE*/)
{
	ASSERT(pLogFont);
	if (!pLogFont)
		return;

	if (m_font.GetSafeHandle())
		m_font.DeleteObject();

	LOGFONT lf = *pLogFont;

	m_font.CreateFontIndirect(&lf);

	if (bRedraw)
		RedrawWindow();
}

BOOL CMyButton::OnEraseBkgnd(CDC* pDC) 
{
	return false; // don't draw background
}

void CMyButton::SetSelfToggle(bool bToggle, int nMillSec)
{
	if (bToggle)
		SetTimer(1, nMillSec, NULL);
	else
		KillTimer(1);
}

void CMyButton::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{
		m_nState = 1 - m_nState;
		Invalidate();
	}

	if (nIDEvent == 2)
	{
		m_nToggleCount++;
		if (m_nToggleCount >= 6)
		{
			KillTimer(2);
			GetParent()->SendMessage(WM_TNM_LONG_PRESSED, m_nIndex, NULL);
		}
	}

	CButton::OnTimer(nIDEvent);
}

void CMyButton::LoadImages(UINT uiResource, int nNum)
{
	m_hBmp = LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(uiResource));

	BITMAP bm;
	GetObject(m_hBmp, sizeof(bm), &bm);

	m_nWidth = bm.bmWidth/nNum;
	m_nHeight = bm.bmHeight;

	m_nMaxImage = nNum;
}

void CMyButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (!m_bEnable || m_bIsDisabled)
		return;

	if (!m_bManual) {
		if (m_nMaxImage >= 3) {
			m_nState = 2;
			Invalidate();
		}
	}

	if (m_bLongToggle) {
		m_nToggleCount = 0;
		SetTimer(2, 200, NULL);
	}

	CButton::OnLButtonDown(nFlags, point);
}

void CMyButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (!m_bEnable || m_bIsDisabled)
		return;

	if (!m_bManual) {
		if (m_nPrevState == 2) {
			m_nState = 1;
		} else {
			m_nState = 0;
		}
		Invalidate();
	}

	if (m_bLongToggle) {
		KillTimer(2);
		if (m_nToggleCount >= 6)
		{
			m_nToggleCount = 0;
			return;
		}
	}

	CButton::OnLButtonUp(nFlags, point);
}

void CMyButton::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_bEnable || m_bIsDisabled)
		return;

	if (!m_bManual) {
		if (!m_bIsTracking) {
			TRACKMOUSEEVENT tme;
			tme.cbSize = sizeof(tme);
			tme.hwndTrack = m_hWnd;
			tme.dwFlags = TME_LEAVE | TME_HOVER;
			tme.dwHoverTime = 1;
			m_bIsTracking = _TrackMouseEvent(&tme);

			if (m_nMaxImage >= 2) {
				m_nState = 1;
				Invalidate();
			}
		}
	}
}

LRESULT CMyButton::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	if (!m_bEnable || m_bIsDisabled)
		return 0;

	if (m_nMaxImage >= 2) {
		m_nState = 0;
		Invalidate();
	}

	m_bIsTracking = false;
	Invalidate();

	return 0;
}

bool CMyButton::EnableWindow(bool bEnable)
{
	bool bPrev = m_bIsDisabled;

	if (bEnable)
		m_bIsDisabled = false;
	else
		m_bIsDisabled = true;

	if (m_bIsDisabled && m_nMaxImage >= 4)
		m_nState = 3;
	else
		m_nState = 0;

	Invalidate();

	return bPrev;
}
