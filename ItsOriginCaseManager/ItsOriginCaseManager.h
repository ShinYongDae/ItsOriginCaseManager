
// ItsOriginCaseManager.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CItsOriginCaseManagerApp:
// �� Ŭ������ ������ ���ؼ��� ItsOriginCaseManager.cpp�� �����Ͻʽÿ�.
//

class CItsOriginCaseManagerApp : public CWinApp
{
public:
	CItsOriginCaseManagerApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CItsOriginCaseManagerApp theApp;