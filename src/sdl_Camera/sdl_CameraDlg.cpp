
// sdl_CameraDlg.cpp : 实现文件
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

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// Csdl_CameraDlg 对话框




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


// Csdl_CameraDlg 消息处理程序

BOOL Csdl_CameraDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
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


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void Csdl_CameraDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR Csdl_CameraDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void Csdl_CameraDlg::OnBnClickedbtnDrawrect()
{
	// TODO: 在此添加控件通知处理程序代码
#if SDLorD3D
	m_Csdl_Preview.sdl_DrawLine();
#else

#endif


}


void Csdl_CameraDlg::OnBnClickedbtnOpencamera()
{
	// TODO: 在此添加控件通知处理程序代码
	m_CCamera.OpenCamera(0,HEIGHT,WIDTH);
	m_CCamera.Start();
}


void Csdl_CameraDlg::OnBnClickedbtnZoomin()
{
	// TODO: 在此添加控件通知处理程序代码
#if SDLorD3D
	m_Csdl_Preview.sdl_ZoomIn();
#else

#endif

}


void Csdl_CameraDlg::OnBnClickedbtnZoomout()
{
	// TODO: 在此添加控件通知处理程序代码
#if SDLorD3D
	m_Csdl_Preview.sdl_ZoomOut();
#else

#endif
	
}
