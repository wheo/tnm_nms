#include "stdafx.h"
#include "TnmMisc.h"

BOOL MByteToUnicode(LPCSTR multiByteStr, LPWSTR unicodeStr, DWORD size) {
	// Get the required size of the buffer that receives the Unicode string. 
	DWORD minSize;
	minSize = MultiByteToWideChar(CP_ACP, 0, multiByteStr, -1, NULL, 0);

	if (size < minSize) {
		return FALSE;
	}

	// Convert string from multi-byte to Unicode.
	MultiByteToWideChar(CP_ACP, 0, multiByteStr, -1, unicodeStr, minSize);
	return TRUE;
}

BOOL MByteToUnicode(CString multiByteStr, LPWSTR unicodeStr, DWORD size) {
	// Get the required size of the buffer that receives the multiByte string. 
	DWORD minSize;
	minSize = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(LPCTSTR)multiByteStr, -1, NULL, 0);

	if (size < minSize) {
		return FALSE;
	}

	// Convert string from multi-byte to Unicode.
	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(LPCTSTR)multiByteStr, -1, unicodeStr, minSize);
	return TRUE;
}

BOOL UnicodeToMByte(LPCWSTR unicodeStr, LPSTR multiByteStr, DWORD size) {
	// Get the required size of the buffer that receives the multiByte string. 
	DWORD minSize;
	minSize = WideCharToMultiByte(CP_OEMCP, NULL, unicodeStr, -1, NULL, 0, NULL, FALSE);
	if (size < minSize) {
		return FALSE;
	}
	// Convert string from Unicode to multi-byte.
	WideCharToMultiByte(CP_OEMCP, NULL, unicodeStr, -1, multiByteStr, size, NULL, FALSE);
	return TRUE;
}

char* UnicodeToUTF8(wchar_t uc, char* UTF8)
{
	if (uc <= 0x7f)
	{
		UTF8[0] = (char)uc;
		UTF8[1] = (char) '\0';
	}
	else if (uc <= 0x7ff)
	{
		UTF8[0] = (char)0xc0 + uc / (wchar_t)pow(2, 6);
		UTF8[1] = (char)0x80 + uc % (wchar_t)pow(2, 6);
		UTF8[2] = (char) '\0';
	}
	else if (uc <= 0xffff)
	{
		UTF8[0] = (char)0xe0 + uc / (wchar_t)pow(2, 12);
		UTF8[1] = (char)0x80 + uc / (wchar_t)pow(2, 6) % (wchar_t)pow(2, 6);
		UTF8[2] = (char)0x80 + uc % (wchar_t)pow(2, 6);
		UTF8[3] = (char) '\0';
	}

	return UTF8;
}

void UnicodeToUTF8(LPCWSTR unicodeStr, LPSTR utf8Str, DWORD size) {
	int i = 0;
	char TempUTF8[5000];
	for (i = 0; unicodeStr[i]; i++) {
		UnicodeToUTF8(unicodeStr[i], TempUTF8);
		strcat(utf8Str, TempUTF8);
	}
//	return strlen(utf8Str);
}

void UTF8TOWide(char* strSrc, TCHAR* strDst)
{
	/// UTF-8 => 유니코드	
	//	char strUTF8[2048] = { 0, };
	char strUTF8[5000] = { 0, };
	sprintf(strUTF8, "%s", strSrc);

	int nLen = MultiByteToWideChar(CP_UTF8, 0, strUTF8, strlen(strUTF8), NULL, NULL);
	MultiByteToWideChar(CP_UTF8, 0, strUTF8, strlen(strUTF8), strDst, nLen);
}

void GetNameOnly(TCHAR *pd, const TCHAR *ps)
{
#if 1
	int pos = 0;
	int a, b;

	for (int i = _tcslen(ps); i >= 0; i--) {
		if (ps[i] == '.') {
			b = i;
		}
		if (ps[i] == '\\') {
			a = i + 1;
			break;
		}
	}

	for (int i = a; i<b; i++) {
		pd[pos++] = ps[i];
	}
	pd[pos] = 0;
#else
	for (int i = strlen(ps); i >= 0; i--) {
		if (ps[i] == '\\') {
			sprintf(pd, "%s", &ps[i + 1]);
			break;
		}
	}
#endif
}

void GetPathOnly(TCHAR *pd, const TCHAR *ps)
{
	_stprintf(pd, L"%s", ps);
	for (int i = _tcslen(ps); i >= 0; i--) {
		if (ps[i] == '\\') {
			pd[i] = 0;
			break;
		}
	}
}

#include <Tlhelp32.h>
void KillProcess(TCHAR *strProcess)
{
	DWORD dwSize = 250;
	HANDLE hSnapShot;
	PROCESSENTRY32 pEntry;
	BOOL bCrrent = FALSE;

	hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
	pEntry.dwSize = sizeof(pEntry);
	Process32First(hSnapShot, &pEntry);
	//
	while (1)
	{
		// 다음번 프로세스의 정보를 가져온다.
		BOOL hRes = Process32Next(hSnapShot, &pEntry);

		if (hRes == FALSE)
			break;

		// find Process
		//if (!strncmp(pEntry.szExeFile, "EXCEL.EXE", 8))  
		//if (!wcsncmp(pEntry.szExeFile, strProcess, 8))   
		if (_tcscmp(pEntry.szExeFile, strProcess) == 0) {
			bCrrent = TRUE;
		}

		if (bCrrent) {
			HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pEntry.th32ProcessID);
			if (hProcess) {
				if (TerminateProcess(hProcess, 0)) {
					unsigned long nCode; //프로세스 종료 상태
					GetExitCodeProcess(hProcess, &nCode);
				}
				CloseHandle(hProcess);
			}
			break;
		}

	}
}

int ConsolePrintF(LPCTSTR tszFormat, ...) {
	int cnt = 0;

	va_list argptr;
	TCHAR buf[1024];

	va_start(argptr, tszFormat);
	cnt = _vsntprintf(buf, sizeof(buf) / sizeof(TCHAR), tszFormat, argptr);
	if (cnt < 0)	// overflow
	{
		cnt = sizeof(buf) / sizeof(TCHAR) - 1;
		buf[cnt] = 0;
	}

	static HANDLE hConsole = NULL;
	if (!hConsole)
	{
		AllocConsole();
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		// enlarge console buffer
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
		if (csbi.dwSize.X < 120)	 csbi.dwSize.X = 120;
		if (csbi.dwSize.Y < 9999) csbi.dwSize.Y = 9999;
		SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), csbi.dwSize);
	}

	DWORD dwNumberOfCharsWritten;
	WriteConsole(hConsole, buf, cnt, &dwNumberOfCharsWritten, NULL);

	va_end(argptr);

	return cnt;
}

CTnmThread::CTnmThread(void) {
	m_bExit = false;
	m_hThread = INVALID_HANDLE_VALUE;

	m_eState = mtReady;
}

CTnmThread::~CTnmThread(void) {
	Terminate();
}

void CTnmThread::Start() {
	UINT uiThreadID = 0;
	m_hThread = INVALID_HANDLE_VALUE;

	m_bExit = false;
	m_hThread = (HANDLE)_beginthreadex(NULL, 0, _thread, (void*)this, 0, &uiThreadID);
}

void CTnmThread::Terminate() {
	m_bExit = true;
	if (m_hThread != INVALID_HANDLE_VALUE) {
		WaitForSingleObject(m_hThread, INFINITE);
	}
	m_hThread = INVALID_HANDLE_VALUE;
}

UINT CTnmThread::_thread(LPVOID lParam) {
	CTnmThread *pthis = reinterpret_cast<CTnmThread*>(lParam);

	pthis->SetState(mtRunning);
	pthis->Run();
	pthis->SetState(mtTerminated);
		
	return 0;
}

bool CTnmThread::IsTerminated() {
	if (m_eState == mtTerminated) {
		return true;
	}

	return false;
}

bool CTnmThread::IsRunning() {
	if (m_eState == mtRunning) {
		return true;
	}

	return false;
}

CTnmDoubleThread::CTnmDoubleThread(void) {
	m_bExit = false;
	m_hThread[0] = m_hThread[1] = INVALID_HANDLE_VALUE;
}

CTnmDoubleThread::~CTnmDoubleThread(void) {
	Terminate();
}

bool CTnmDoubleThread::IsRunning()
{
	if ((m_hThread[0] == INVALID_HANDLE_VALUE) && (m_hThread[1] == INVALID_HANDLE_VALUE)) {
		return false;
	}
	else {
		return true;
	}
}

void CTnmDoubleThread::Start() {
	UINT uiThreadID;

	m_bExit = false;

	m_hThread[0] = (HANDLE)_beginthreadex(NULL, 0, _thread1, (void*)this, 0, &uiThreadID);
	m_hThread[1] = (HANDLE)_beginthreadex(NULL, 0, _thread2, (void*)this, 0, &uiThreadID);
}

void CTnmDoubleThread::Terminate() {
	m_bExit = true;
	if (m_hThread[0] != INVALID_HANDLE_VALUE && m_hThread[1] != INVALID_HANDLE_VALUE) {
		WaitForMultipleObjects(2, m_hThread, true, INFINITE);
	}
}

UINT CTnmDoubleThread::_thread1(LPVOID lParam) {
	CTnmDoubleThread *pthis = reinterpret_cast<CTnmDoubleThread*>(lParam);

	pthis->Run1();
	return 0;
}

UINT CTnmDoubleThread::_thread2(LPVOID lParam) {
	CTnmDoubleThread *pthis = reinterpret_cast<CTnmDoubleThread*>(lParam);

	pthis->Run2();
	return 0;
}

CTnmMemDC::CTnmMemDC(CDC *pDC, bool bIsInvDel) {
	Create(pDC, NULL, bIsInvDel);
}

CTnmMemDC::CTnmMemDC(CDC *pDC, CRect &rc, bool bIsInvDel) {
	Create(pDC, &rc, bIsInvDel);
}

CTnmMemDC::~CTnmMemDC() {
	if (m_bIsMemDC) {
		if (m_bIsInvDel) {
			m_pDC->BitBlt(m_rc.left, m_rc.top, m_rc.Width(), m_rc.Height(), this, m_rc.left, m_rc.top, SRCCOPY);
		}
		SelectObject(m_hOldBmp);
		DeleteObject(m_hBmp);
		DeleteDC();
	} else {
		m_hDC = NULL;
		m_hAttribDC = NULL;
	}
}

void CTnmMemDC::Create(CDC *pDC, const CRect *pRect, bool bIsInvDel) {
	m_pDC = pDC;
	m_hOldBmp = NULL;
	m_bIsMemDC = !pDC->IsPrinting(); // DC가 인쇄를 위하여 사용되는지 검사하는 함수
	m_bIsInvDel = bIsInvDel;

	if (m_bIsMemDC) {
		CreateCompatibleDC(pDC);
		if (NULL == pRect) {
			pDC->GetClipBox(&m_rc);
		} else {
			m_rc = *pRect;
		}

		m_hBmp = CreateCompatibleBitmap(pDC->GetSafeHdc(), m_rc.Width(), m_rc.Height());
		m_hOldBmp = SelectObject(m_hBmp);

		SetWindowOrg(m_rc.left, m_rc.top);

		// Fill background, 배경을 전달 받은 매개변수 DC 의 배경색으로 채운다.
		FillSolidRect(m_rc, pDC->GetBkColor());
	} else {
		m_bPrinting = pDC->m_bPrinting;
		m_hDC = pDC->m_hDC;
		m_hAttribDC = pDC->m_hAttribDC;
	}
}

CTnmMemDC *CTnmMemDC::operator->() {
	return this;
}

CTnmMemDC::operator CTnmMemDC*() {
	return this;
}

BOOL CTnmMemDC::Draw(CDC *pDC) {
	return pDC->BitBlt(0, 0, m_rc.Width(), m_rc.Height(), this, m_rc.left, m_rc.top, SRCCOPY);
}

bool RegWriteString(HKEY hKey, LPCWSTR lpSubKey, LPCWSTR lpValueKey, LPCWSTR lpValue) {
	HKEY hSubKey = NULL;

	if (::RegOpenKeyEx(hKey, lpSubKey, 0, KEY_ALL_ACCESS, &hSubKey) == ERROR_SUCCESS) {
		DWORD cbSize = (DWORD)lstrlenW(lpValue) * sizeof(TCHAR) + 1;
		BYTE *pBuf = new BYTE[cbSize];

		::ZeroMemory(pBuf, cbSize);
		::CopyMemory(pBuf, lpValue, cbSize - 1);
		::RegSetValueEx(hSubKey, lpValueKey, NULL, REG_SZ, pBuf, cbSize);

		::RegCloseKey(hSubKey);

		delete[] pBuf;
		return true;
	}

	return false;
}
bool RegWriteDWORD(HKEY hKey, LPCWSTR lpSubKey, LPCWSTR lpValueKey, DWORD dwValue) {
	HKEY hSubKey = NULL;

	if (::RegOpenKeyEx(hKey, lpSubKey, 0, KEY_ALL_ACCESS, &hSubKey) == ERROR_SUCCESS) {
		::RegSetValueEx(hSubKey, lpValueKey, NULL, REG_DWORD, (LPBYTE)&dwValue, sizeof(DWORD));
		::RegCloseKey(hSubKey);

		return true;
	}

	return false;
}

CString RegReadString(HKEY hKey, LPCWSTR lpSubKey, LPCWSTR lpValueKey) {
	HKEY hSubKey = NULL;
	CString strValue("");

	if (::RegOpenKeyEx(hKey, lpSubKey, 0, KEY_READ, &hSubKey) == ERROR_SUCCESS) {
		DWORD dwSize = 0;
		if (::RegQueryValueEx(hSubKey, lpValueKey, NULL, NULL, NULL, &dwSize) == ERROR_SUCCESS) {
			TCHAR *pBuf = new TCHAR[dwSize + 1];
			memset(pBuf, 0, sizeof(TCHAR)*(dwSize + 1));
			if (::RegQueryValueEx(hSubKey, lpValueKey, NULL, NULL, (LPBYTE)pBuf, &dwSize) == ERROR_SUCCESS) {
				pBuf[dwSize] = _T('\0');
				strValue = CString(pBuf);
			}

			delete[] pBuf;
		}

		::RegCloseKey(hSubKey);
	}

	return strValue;
}

DWORD RegReadDWORD(HKEY hKey, LPCWSTR lpSubKey, LPCWSTR lpValueKey, DWORD &dwValue) {
	HKEY hSubKey = NULL;

	if (::RegOpenKeyEx(hKey, lpSubKey, 0, KEY_READ, &hSubKey) == ERROR_SUCCESS) {
		DWORD dwSize = sizeof(DWORD);
		if (::RegQueryValueEx(hSubKey, lpValueKey, NULL, NULL, (LPBYTE)&dwValue, &dwSize) == ERROR_SUCCESS) {
			ASSERT(dwSize = sizeof(DWORD));
		}

		::RegCloseKey(hSubKey);
	}
	return dwValue;
}

int CALLBACK MyBrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData) {
	switch (uMsg) {
	case BFFM_INITIALIZED:
		::SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
		break;
	}
	return 0;
}

bool BrowseForDirectory(CWnd* pWndOwner, LPCTSTR title, CString& folderName, bool bIsShared) {
	TCHAR path[MAX_PATH];
	LPTSTR pFolderName = folderName.GetBuffer(folderName.GetLength());
	LPITEMIDLIST pidl = NULL;
	LPITEMIDLIST pidlR = NULL;

	BROWSEINFO browse;
	ZeroMemory(&browse, sizeof(BROWSEINFO));

	if (bIsShared) {
		SHGetSpecialFolderLocation(NULL, CSIDL_NETWORK, &pidlR);
	}

	browse.hwndOwner = pWndOwner->GetSafeHwnd();
	browse.pidlRoot = NULL; // pidlR;// pidl;
	browse.pszDisplayName = pFolderName;
	browse.lpszTitle = title;
	browse.ulFlags = BIF_RETURNONLYFSDIRS;
	browse.lpfn = MyBrowseCallbackProc;
	browse.lParam = (LPARAM)pFolderName;

	if (bIsShared) {
		browse.ulFlags |= BIF_NEWDIALOGSTYLE;
	}

	pidl = SHBrowseForFolder(&browse);

	folderName.ReleaseBuffer();
	if (NULL != pidl) {
		if (SHGetPathFromIDList(pidl, path)) {
			folderName = path;

			return true;
		}
	}

	return false;
}