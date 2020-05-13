#pragma once

/** some MACROs */
#define SAFE_DELETE(x) if (x) {delete x; }; x = NULL;
#define SAFE_RELEASE(x) if (x) { x->Release();}; x = NULL;
#define SAFE_FCLOSE(x) if (x) { fclose(x); } x = NULL;
#define SAFE_BUFFDEL(x) if (x) { delete [] x; }; x = NULL;

#define rnd(x, digit) (floor((x)*pow(float(10), digit) + 0.5f) / pow(float(10), digit));

/** Debug console */
int ConsolePrintF(LPCTSTR tszFormat, ...);

BOOL MByteToUnicode(LPCSTR multiByteStr, LPWSTR unicodeStr, DWORD size);
BOOL MByteToUnicode(CString multiByteStr, LPWSTR unicodeStr, DWORD size);
BOOL UnicodeToMByte(LPCWSTR unicodeStr, LPSTR multiByteStr, DWORD size);
void UTF8TOWide(char* strSrc, TCHAR* strDst);
char* UnicodeToUTF8(wchar_t uc, char* UTF8);
void UnicodeToUTF8(LPCWSTR unicodeStr, LPSTR utf8Str, DWORD size);

void GetNameOnly(TCHAR *pd, const TCHAR *ps);
void GetPathOnly(TCHAR *pd, const TCHAR *ps);

void KillProcess(TCHAR *strProcess);

#if 0
#define _d(...)	ConsolePrintF(__VA_ARGS__)
#else 
#define _d(...)
#endif

/** Thread class */
enum MT_STATE {
	mtReady, mtRunning, mtTerminated, mtZombie, mtAborted
};

class CTnmThread {
public:
	CTnmThread(void);
	virtual ~CTnmThread(void);

	void Start();
	void Terminate();

	bool IsTerminated();
	bool IsRunning();

	void SetState(MT_STATE st) { m_eState = st; };
	HANDLE GetHandle() { return m_hThread; };

protected:

	virtual void Run() = 0;
	static unsigned __stdcall _thread(LPVOID lParam);

protected:

	bool m_bExit; ///> 쓰레드 종료 플래그
	bool m_bPause;

private:

	HANDLE m_hThread; ///> 쓰레드 핸들
	MT_STATE m_eState; ///> 쓰레드 상태 변수

};

class CTnmDoubleThread {
public:
	CTnmDoubleThread(void);
	virtual ~CTnmDoubleThread(void);

	void Start();
	void Terminate();

	bool IsRunning();

protected:

	virtual void Run1() = 0;
	virtual void Run2() = 0;

	static unsigned __stdcall _thread1(LPVOID lParam);
	static unsigned __stdcall _thread2(LPVOID lParam);

protected:

	bool m_bExit;

	HANDLE m_hThread[2];
};


class CTnmMemDC : public CDC {
	// Constructors
public:
	CTnmMemDC(CDC *pDC, bool bIsInvDel = true);
	CTnmMemDC(CDC *pDC, CRect &rc, bool bIsInvDel = true);
	virtual ~CTnmMemDC();

private:
	CDC			*m_pDC;

	HBITMAP		m_hBmp;
	HGDIOBJ		m_hOldBmp;

	CRect		m_rc;

	bool		m_bIsMemDC;
	bool		m_bIsInvDel;

public:
	BOOL Draw(CDC *pDC);

	CTnmMemDC *operator->();
	operator CTnmMemDC *();

protected:
	void Create(CDC *pDC, const CRect *pRect, bool bIsInvDel);
};

class CTnmBmpDC : public CDC {
private:
	HBITMAP		m_hBmp;
	HGDIOBJ		m_hOldBmp;

	CDC			*m_pDC;
	CRect		m_rc;

public:

	CTnmBmpDC() {
		m_hBmp = NULL;
		m_hOldBmp = NULL;
	}
	CTnmBmpDC(CDC *pDC, UINT uIdResource) : CDC() {
		Create(pDC, uIdResource);
	};
	CTnmBmpDC(CDC *pDC, TCHAR *strFileName) : CDC() {
		Create(pDC, strFileName);
	};

	virtual ~CTnmBmpDC() {
		SelectObject(m_hOldBmp);

		::DeleteObject(m_hBmp);
	};

	void Create(CDC *pDC, UINT uIdResource) {
		BITMAP bmp;

		pDC = m_pDC;

		CreateCompatibleDC(pDC);

		m_hBmp = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(uIdResource));
		m_hOldBmp = SelectObject(m_hBmp);

		GetObject(m_hBmp, sizeof(BITMAP), &bmp);

		m_rc.left = 0;
		m_rc.top = 0;
		m_rc.right = bmp.bmWidth;
		m_rc.bottom = bmp.bmHeight;
	};

	void Create(CDC *pDC, TCHAR *strFileName) {
		BITMAP bmp;

		pDC = m_pDC;

		CreateCompatibleDC(pDC);

		m_hBmp = (HBITMAP)LoadImage(AfxGetInstanceHandle(), strFileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		m_hOldBmp = SelectObject(m_hBmp);

		GetObject(m_hBmp, sizeof(BITMAP), &bmp);

		m_rc.left = 0;
		m_rc.top = 0;
		m_rc.right = bmp.bmWidth;
		m_rc.bottom = bmp.bmHeight;
	};

	// Allow usage as a pointer	
	CTnmBmpDC* operator->() {
		return this;
	}

	// Allow usage as a pointer	
	operator CTnmBmpDC*() {
		return this;
	}

	int GetWidth() {
		return m_rc.Width();
	};
	int GetHeight() {
		return m_rc.Height();
	};
};

extern bool RegWriteString(HKEY hKey, LPCWSTR lpSubKey, LPCWSTR lpValueKey, LPCWSTR lpValue);
extern bool RegWriteDWORD(HKEY hKey, LPCWSTR lpSubKey, LPCWSTR lpValueKey, DWORD dwValue);

extern CString RegReadString(HKEY hKey, LPCWSTR lpSubKey, LPCWSTR lpValueKey);
extern DWORD RegReadDWORD(HKEY hKey, LPCWSTR lpSubKey, LPCWSTR lpValueKey, DWORD &dwValue);

bool BrowseForDirectory(CWnd* pWndOwner, LPCTSTR title, CString& folderName, bool bIsShared = false);

