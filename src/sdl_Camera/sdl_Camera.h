
// sdl_Camera.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// Csdl_CameraApp:
// �йش����ʵ�֣������ sdl_Camera.cpp
//

class Csdl_CameraApp : public CWinApp
{
public:
	Csdl_CameraApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern Csdl_CameraApp theApp;