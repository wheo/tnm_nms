#pragma once


// CMyButton

class CMyButton : public CButton
{
	DECLARE_DYNAMIC(CMyButton)

public:
	CMyButton();
	virtual ~CMyButton();

	void LoadImages(UINT uiResource, int nNum);
	bool EnableWindow(bool bEnable = true);

	void SetCheck(int nState) 
	{ 
		m_nState = nState; 
		m_bRedraw = true;
		Invalidate();
	};

	void SetText(CString strText, COLORREF cbText = RGB(250,250,250));

	void SetLongToggle(bool bLongToggle, int nIndex = 0) { m_bLongToggle = bLongToggle;	m_nIndex = nIndex; };

	void SetManual(bool bManual) { m_bManual = bManual; };
	void SetEnable(bool bEnable) { m_bEnable = bEnable; };
	void SetToggle(bool bIsToggle) { m_bIsToggleButton = bIsToggle; };
	void SetSelfToggle(bool bToggle, int nMillSec = 1000);
	
	void SetFont(const CString& strFont, int nPointSize,int nWeight, BOOL bRedraw = TRUE);
	void SetFont(LOGFONT *pLogFont, BOOL bRedraw = TRUE);

protected:
	virtual void PreSubclassWindow();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	DECLARE_MESSAGE_MAP()

	void DrawText(CDC* pDCMem, CRect* pRect, CString csText);
	void DrawBitmap(CDC *pDC, HBITMAP hBmp, CRect rc, BOOL bStretch = false);

	bool IsASingleWord(const CString & csText);

protected:
	
	bool	m_bRedraw;
	bool	m_bLongToggle;

	bool	m_bEnable;
	bool	m_bIsDisabled;

	bool	m_bManual;
	bool	m_bIsToggleButton;
	bool	m_bIsTracking;

	int		m_nIndex;

	int		m_nWidth;
	int		m_nHeight;

	int		m_nToggleCount;

	int		m_nState;
	int		m_nPrevState;

	int		m_nMaxImage;

	int		m_nFontSize;
	DWORD	m_dwTxtFlags;

	CFont	m_font;

	CString m_strText;

	COLORREF m_cbText;

	HBITMAP m_hBmp;

public:
	
//	afx_msg void OnPaint();
};


