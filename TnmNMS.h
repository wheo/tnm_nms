
// TnmNMS.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CTnmNMSApp:
// �� Ŭ������ ������ ���ؼ��� TnmNMS.cpp�� �����Ͻʽÿ�.
//

class CTnmNMSApp : public CWinApp
{
public:
	CTnmNMSApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CTnmNMSApp theApp;