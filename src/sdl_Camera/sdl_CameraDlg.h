
// sdl_CameraDlg.h : 头文件
//

#pragma once
#include "d3d_Preview.h"
#include "Young_CCamera.h"
#include "sdl_Preview.h"
//#include "./test/PlayControl.h"
#include "./Convert/ColourSpaceConvert.h"
#define  SDLorD3D 1

// Csdl_CameraDlg 对话框
class Csdl_CameraDlg : public CDialogEx
{
// 构造
public:
	Csdl_CameraDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SDL_CAMERA_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
		CCamera m_CCamera;

#if SDLorD3D
		Csdl_Preview m_Csdl_Preview;
#else
		Cd3d_Preview m_Cd3d_Preview;
#endif

		CColourSpaceConvert m_CColourSpaceConvert;
		afx_msg void OnBnClickedbtnDrawrect();
		afx_msg void OnBnClickedbtnOpencamera();
		afx_msg void OnBnClickedbtnZoomin();
		afx_msg void OnBnClickedbtnZoomout();

	CEdit * m_edit;
};
