
// sdl_CameraDlg.h : ͷ�ļ�
//

#pragma once
#include "Young_CCamera.h"
#include "sdl_Preview.h"




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
		Csdl_Preview m_Csdl_Preview;
		afx_msg void OnBnClickedbtnDrawrect();
		afx_msg void OnBnClickedbtnOpencamera();
};
