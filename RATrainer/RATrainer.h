
// RATrainer.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CRATrainerApp:
// �йش����ʵ�֣������ RATrainer.cpp
//

class CRATrainerApp : public CWinApp
{
public:
	CRATrainerApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CRATrainerApp theApp;