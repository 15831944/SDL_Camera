
// sdl_CameraDlg.h : 头文件
//

#pragma once
#include "Young_CCamera.h"
#include "sdl_Preview.h"




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
		Csdl_Preview m_Csdl_Preview;
		afx_msg void OnBnClickedbtnDrawrect();
		afx_msg void OnBnClickedbtnOpencamera();
};
