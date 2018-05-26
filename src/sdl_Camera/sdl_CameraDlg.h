
// sdl_CameraDlg.h : ͷ�ļ�
//

#pragma once
#include "d3d_Preview.h"
#include "Young_CCamera.h"
#include "sdl_Preview.h"
//#include "./test/PlayControl.h"
#include "./Convert/ColourSpaceConvert.h"
#define  SDLorD3D 1

// Csdl_CameraDlg �Ի���
class Csdl_CameraDlg : public CDialogEx
{
// ����
public:
	Csdl_CameraDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SDL_CAMERA_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
