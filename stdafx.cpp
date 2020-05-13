
// stdafx.cpp : ǥ�� ���� ���ϸ� ��� �ִ� �ҽ� �����Դϴ�.
// TnmNMS.pch�� �̸� �����ϵ� ����� �˴ϴ�.
// stdafx.obj���� �̸� �����ϵ� ���� ������ ���Ե˴ϴ�.

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