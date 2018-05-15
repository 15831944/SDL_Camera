#include "stdafx.h"  
#include "PlayControl.h"  
#include "ColourSpaceConvert.h"  
#include  <afx.h>  
/////////////////////////////////////////////////////////////////////////////  
// CPlayControl  
IMPLEMENT_DYNCREATE(CPlayControl, CWinThread)  
CPlayControl::CPlayControl()  
{  
    m_State = 0;  
    f_in = NULL;  
    rgbbuf = NULL;   
    yuvbuf = NULL;  
    CreateThread();  
}  
CPlayControl::~CPlayControl()  
{  
    f_in = NULL;  
    rgbbuf = NULL;   
    yuvbuf = NULL;  
}  
BOOL CPlayControl::InitInstance()  
{  
    // TODO:  perform and per-thread initialization here  
    return TRUE;  
}  
int CPlayControl::ExitInstance()  
{  
    // TODO:  perform any per-thread cleanup here  
    return CWinThread::ExitInstance();  
}  
BEGIN_MESSAGE_MAP(CPlayControl, CWinThread)  
    //{{AFX_MSG_MAP(CPlayControl)  
        // NOTE - the ClassWizard will add and remove mapping macros here.  
        ON_THREAD_MESSAGE(WM_MANAGE,OnManage)  
    //}}AFX_MSG_MAP  
END_MESSAGE_MAP()  
/////////////////////////////////////////////////////////////////////////////  
// CPlayControl message handlers  
int CPlayControl::Init(HWND hwnd, char *filename)  
{  
    m_D3D.InitD3D(hwnd);  
    m_D3D.CreateTexture();  
    m_D3D.InitGeometry();  
    if(m_D3D.GetFormt() == D3D_ARGB32)  
    {  
        rgbbuf = new BYTE[RGBPLANE];   
        yuvbuf = new BYTE[YUVPLANE];  
    }  
    else  
    {  
        return -1;  
    }     
    if(!rgbbuf || !yuvbuf)  
    {  
        delete [] rgbbuf;  
        delete [] yuvbuf;  
        rgbbuf = NULL;   
        yuvbuf = NULL;  
        return -1;  
    }  
    f_in=fopen(filename,"rb");  
    if (f_in == NULL)  
    {  
        return -1;  
    }  
    m_State = 1;  
    return 0;  
}  
void CPlayControl::Play()  
{  
    m_State = 1;  
}  
void CPlayControl::Stop()  
{  
    m_State = 0;          
}  
void CPlayControl::Pause()  
{  
    m_State = 2;  
}  
void CPlayControl::OnManage(WPARAM wParam, LPARAM lParam)  
{  
    CColourSpaceConvert m_csc;  
    if(f_in == NULL)  
        return;  
    while(1)  
    {  
        if(m_State == 1)  
        {  
            if(fread(yuvbuf, YUVPLANE, 1, f_in))  
            {  
                m_csc.YUV2RGB(yuvbuf,   
                    yuvbuf + IMAGE_HEIGHT*IMAGE_WIDTH*5/4,   
                    yuvbuf + IMAGE_HEIGHT*IMAGE_WIDTH,  
                    rgbbuf, IMAGE_WIDTH, IMAGE_HEIGHT);  
                m_D3D.RenderSample(rgbbuf);  
                m_D3D.RenderTOSrceen();   
                Sleep(33);  
            }  
            else  
                break;  
        }  
        else if(m_State == 2)  
            continue;  
        else  
            break;  
    }  
    fclose(f_in);  
}  