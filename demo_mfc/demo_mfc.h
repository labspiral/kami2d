// demo_mfc.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// Cdemo_mfcApp:
// �� Ŭ������ ������ ���ؼ��� demo_mfc.cpp�� �����Ͻʽÿ�.
//

class Cdemo_mfcApp : public CWinApp
{
public:
	Cdemo_mfcApp();

// �������Դϴ�.
	public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern Cdemo_mfcApp theApp;