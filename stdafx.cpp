
// stdafx.cpp : 표준 포함 파일만 들어 있는 소스 파일입니다.
// TnmNMS.pch는 미리 컴파일된 헤더가 됩니다.
// stdafx.obj에는 미리 컴파일된 형식 정보가 포함됩니다.

#include "stdafx.h"


bool CompareWarningMsg(stWarningMsg *pSrc, stWarningMsg *pDst)
{
	if (_tcscmp(pSrc->groupInfo.strGroupName, pDst->groupInfo.strGroupName) != 0) {
		return false;
	}
	
	if (_tcscmp(pSrc->serverInfo.strServerName, pDst->serverInfo.strServerName) != 0) {
		return false;
	}
	
	if (_tcscmp(pSrc->strWarningDescription, pDst->strWarningDescription) != 0) {
		return false;
	}
	
	return true;
}