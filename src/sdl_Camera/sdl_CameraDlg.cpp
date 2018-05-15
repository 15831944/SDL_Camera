
// sdl_CameraDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "sdl_Camera.h"
#include "sdl_CameraDlg.h"
#include "afxdialogex.h"

int HEIGHT = 2592;
int WIDTH = 1944;
#define  SDLorD3D 0
#define YUVPLANE WIDTH*HEIGHT*3/2  
#define RGBPLANE WIDTH*HEIGHT*4  

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
Csdl_CameraDlg *m_Csdl_CameraDlg;

//YOUNG callback
static bool CALLBACK cbPreviewImage(byte * src,long width,long height,long size)
{
#if SDLorD3D
	m_Csdl_CameraDlg->m_Csdl_Preview.sdl_PreviewImage(src,width,height);
#else
 //   BYTE*   rgbbuf = new BYTE[RGBPLANE];   
	//m_Csdl_CameraDlg->m_CColourSpaceConvert.YUV2RGB(src,   
	//	src + HEIGHT*WIDTH*5/4,   
	//	src + HEIGHT*WIDTH,  
	//	rgbbuf, WIDTH, HEIGHT);  
	//m_Csdl_CameraDlg->m_Cd3d_Preview.RenderSample(rgbbuf);
 //   m_Csdl_CameraDlg->m_Cd3d_Preview.RenderTOSrceen();   
	//delete []rgbbuf;
	//rgbbuf = NULL;
	m_Csdl_CameraDlg->m_Cd3d_Preview.Render(src);

#endif

	return true;
}
//YOUNG callback

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Csdl_CameraDlg �Ի���




Csdl_CameraDlg::Csdl_CameraDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Csdl_CameraDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Csdl_CameraDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(Csdl_CameraDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_btn_DrawRect, &Csdl_CameraDlg::OnBnClickedbtnDrawrect)
	ON_BN_CLICKED(IDC_btn_OpenCamera, &Csdl_CameraDlg::OnBnClickedbtnOpencamera)
	ON_BN_CLICKED(IDC_btn_Zoomin, &Csdl_CameraDlg::OnBnClickedbtnZoomin)
	ON_BN_CLICKED(IDC_btn_Zoomout, &Csdl_CameraDlg::OnBnClickedbtnZoomout)
END_MESSAGE_MAP()


// Csdl_CameraDlg ��Ϣ�������

BOOL Csdl_CameraDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_Csdl_CameraDlg = this;
	m_CCamera.camRegCallBackPreviewImage(cbPreviewImage);

#if SDLorD3D
	m_Csdl_Preview.sdl_SetHwnd(GetDlgItem(IDC_Preview)->GetSafeHwnd(),HEIGHT,WIDTH);
#else
	//m_Cd3d_Preview.InitD3D(GetDlgItem(IDC_Preview)->GetSafeHwnd());
	//m_Cd3d_Preview.CreateTexture();  
	//m_Cd3d_Preview.InitGeometry();  
	//RECT rect;
	//GetDlgItem(IDC_Preview)->GetWindowRect(&rect);
	m_Cd3d_Preview.InitD3D(GetDlgItem(IDC_Preview)->GetSafeHwnd(),HEIGHT,WIDTH);
#endif


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void Csdl_CameraDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void Csdl_CameraDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR Csdl_CameraDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void Csdl_CameraDlg::OnBnClickedbtnDrawrect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
#if SDLorD3D
	m_Csdl_Preview.sdl_DrawLine();
#else

#endif


}


void Csdl_CameraDlg::OnBnClickedbtnOpencamera()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_CCamera.OpenCamera(0,HEIGHT,WIDTH);
	m_CCamera.Start();
}


void Csdl_CameraDlg::OnBnClickedbtnZoomin()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
#if SDLorD3D
	m_Csdl_Preview.sdl_ZoomIn();
#else

#endif

}


void Csdl_CameraDlg::OnBnClickedbtnZoomout()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
#if SDLorD3D
	m_Csdl_Preview.sdl_ZoomOut();
#else

#endif
	
}
