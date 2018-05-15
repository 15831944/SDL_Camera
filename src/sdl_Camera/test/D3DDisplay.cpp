#include "stdafx.h"  
#include "D3DDisplay.h"  
#include "DEFINE.h"  
#ifdef _DEBUG  
#undef THIS_FILE  
static char THIS_FILE[]=__FILE__;  
#define new DEBUG_NEW  
#endif  
struct CUSTOMVERTEX//顶点缓存  
{       
    float x,y,z,rhw;    //顶点坐标   
    DWORD color;        //顶点颜色  
    float tu,tv;        //纹理坐标  
};   
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)  
//////////////////////////////////////////////////////////////////////  
// Construction/Destruction  
//////////////////////////////////////////////////////////////////////  
CD3DDisplay::CD3DDisplay()  
{  
    m_pd3dDevice = NULL;  
    m_pD3D = NULL;  
    m_p2Dfont = NULL;  
    m_pTexturesvideo = NULL;  
    m_pVBvideo = NULL;  
    m_pTexturesbmp = NULL;  
    m_pVBbmp = NULL;  
    m_hWnd = NULL;  
    m_Format = (D3DFORMAT)0;  
}  
CD3DDisplay::~CD3DDisplay()  
{  
    ClearD3D();   
}  
HRESULT CD3DDisplay::InitD3D(HWND hWnd)//初始化d3d  
{  
    D3DPRESENT_PARAMETERS d3dpp;//显示参数设置  
    if(hWnd == NULL)  
        return -1;  
    m_hWnd = hWnd;  
    //D3DFMT_A8R8G8B8表示一个32位像素，从左开始，8位为ALPHA通道，8位分配给红色，8位分配给绿色，8位分配给蓝色  
    m_Format = D3DFMT_A8R8G8B8;  
    if(NULL == (m_pD3D = Direct3DCreate9( D3D_SDK_VERSION )))//创建d3d对象  
        return -1;  
    ZeroMemory( &d3dpp, sizeof(d3dpp) );//清空显示参数，重新设置参数  
    d3dpp.Windowed = TRUE;//FALSE，表示要渲染全屏，如果为TRUE，表示要渲染窗口  
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;//交换缓冲支持的效果类型，指定表面在交换链中D是如何被交换的  
    //D3DSWAPEFFECT_DISCARD，则后备缓冲区的东西被复制到屏幕上后,后备缓冲区的东西就没有什么用可以丢弃  
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;//后备缓冲的格式D3DFMT_UNKNOWN，这时候它将使用桌面的格式  
    d3dpp.EnableAutoDepthStencil = TRUE;//如果要使用缓冲或模板缓冲则把它设为TRUE  
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;//如果启动了深度缓冲那么这个参数将为深度设定缓冲格式  
    //D3DFMT_16深度缓冲  
    d3dpp.BackBufferWidth = IMAGE_WIDTH;//后备缓冲的宽度和高度  
    d3dpp.BackBufferHeight = IMAGE_HEIGHT;//在全屏模式下这两者的值必需符合显卡所支持的分辨率  
      
    if( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT,  
        D3DDEVTYPE_HAL,   
        m_hWnd,  
        D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,  
        &d3dpp,   
        &m_pd3dDevice ) ) )//创建d3d设备  
        return -1;  
    return 0;  
}  
void CD3DDisplay::ClearD3D()//释放d3d  
{  
    if(m_pd3dDevice != NULL)  
        m_pd3dDevice->Release();  
    if(m_pD3D != NULL)  
        m_pD3D->Release();   
    if(m_p2Dfont != NULL)  
        m_p2Dfont->Release();   
    if(m_pVBvideo != NULL)  
        m_pVBvideo->Release();  
    if(m_pTexturesvideo != NULL)  
        m_pTexturesvideo->Release();  
    if(m_pVBbmp != NULL)  
        m_pVBbmp->Release();   
    if(m_pTexturesbmp != NULL)  
        m_pTexturesbmp->Release();   
    m_hWnd = NULL;  
    m_Format = (D3DFORMAT)0;  
}  
HRESULT CD3DDisplay::CreateTexture()//创建纹理  
{  
    HRESULT hr = 0;  
    D3DSURFACE_DESC ddsd;  
    hr = m_pd3dDevice->CreateTexture( IMAGE_WIDTH, IMAGE_HEIGHT, 1, 0, m_Format,   
        D3DPOOL_MANAGED, &m_pTexturesvideo, NULL);  
    if( FAILED(hr))  
    {  
        return -1;  
    }  
hr = m_pTexturesvideo->GetLevelDesc( 0, &ddsd );//获得加载图片的宽和高及些信息  
    if(FAILED(hr))  
        return -1;  
    if ((ddsd.Format != D3DFMT_A8R8G8B8) && (ddsd.Format != D3DFMT_YUY2))   
        return -1;  
  
    return 0;  
}  
HRESULT CD3DDisplay::InitGeometry()//创建视频几何建立顶点缓存并初始化纹坐标  
{  
    HRESULT hr = 0;  
    CUSTOMVERTEX* pVertices = NULL;  
    RECT rect;  
    //创建视频矩形  
    rect.top = 0;             
    rect.left = 0;  
    rect.right = IMAGE_WIDTH;  
    rect.bottom = IMAGE_HEIGHT;  
    CUSTOMVERTEX vertices[4] =  //顶点缓存  
    {     // x, y, z, rhw, color  
        { (float)rect.left, (float)rect.top,   
            0.0f, 1.0f, D3DCOLOR_XRGB(255,255,255), 0.0f, 0.0f },   
        { (float)rect.right, (float)rect.top,   
        0.0f, 1.0f, D3DCOLOR_XRGB(255,255,255), 1.0f, 0.0f },   
        { (float)rect.left, (float)rect.bottom,   
        0.0f, 1.0f, D3DCOLOR_XRGB(255,255,255), 0.0f, 1.0f },   
        { (float)rect.right, (float)rect.bottom,   
        0.0f, 1.0f, D3DCOLOR_XRGB(255,255,255), 1.0f, 1.0f }  
    };  
    m_pd3dDevice->CreateVertexBuffer(sizeof(vertices), //指定缓冲区的大小  
        0, //指定顶点缓冲区的属性  
        D3DFVF_CUSTOMVERTEX, //开始创建的定点格式  
        D3DPOOL_DEFAULT, //指定顶点缓冲区的内存类型  
        &m_pVBvideo, //视频矩形的顶点缓存区接口指针  
        NULL ); //保留参数  
m_pVBvideo->Lock( 0, sizeof(vertices), (void**)&pVertices, 0 ); //填充顶点缓冲区  
    memcpy( pVertices, vertices, sizeof(vertices));   
    m_pVBvideo->Unlock();      
    return hr;  
}  
HRESULT CD3DDisplay::RenderSample(BYTE *pSampleBuffer)  
{  
    HRESULT hr = 0;  
    BYTE * pTextureBuffer = NULL;  
    D3DLOCKED_RECT d3dlr;  
    LONG  lTexturePitch;       
        hr = m_pTexturesvideo->LockRect( 0, &d3dlr, 0, 0 );  
    if( FAILED(hr))  
    {  
        return -1;  
    }  
    lTexturePitch = d3dlr.Pitch;  
    pTextureBuffer = static_cast<byte *>(d3dlr.pBits);  
    for(int i = 0; i < IMAGE_HEIGHT; i++ )   
    {  
        BYTE *pBmpBufferOld = pSampleBuffer;  
        BYTE *pTxtBufferOld = pTextureBuffer;     
        for (int j = 0; j < IMAGE_WIDTH; j++)   
        {  
            pTextureBuffer[0] = pSampleBuffer[0];  
            pTextureBuffer[1] = pSampleBuffer[1];  
            pTextureBuffer[2] = pSampleBuffer[2];  
            pTextureBuffer[3] = 0xFF;  
            pTextureBuffer += 4;  
            pSampleBuffer  += 3;  
        }  
        pSampleBuffer  = pBmpBufferOld + IMAGE_WIDTH *3;  
        pTextureBuffer = pTxtBufferOld + lTexturePitch;  
    }  
    hr = m_pTexturesvideo->UnlockRect(0);  
    if( FAILED(hr))  
    {  
        return -1;  
    }  
    return 0;  
}  
HRESULT CD3DDisplay::RenderTOSrceen()//传递到屏幕  
{  
    HRESULT hr = 0;  
    hr = m_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0 );  
    if( FAILED(hr))  
    {  
        return -1;  
    }  
    hr = m_pd3dDevice->BeginScene();  
    if( FAILED(hr))  
    {  
        return -1;  
    }  
    //显示视频  
    hr = m_pd3dDevice->SetTexture( 0, m_pTexturesvideo );  
    m_pd3dDevice->SetStreamSource( 0, m_pVBvideo, 0, sizeof(CUSTOMVERTEX));   
    hr = m_pd3dDevice->SetVertexShader( NULL );//设置可编程渲染管道的Shader程序  
    m_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX ); //设置固定渲染管道ì的顶点格式  
    m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );//渲染一组非索引  
    m_pd3dDevice->SetTexture( 0, NULL );   
    m_pd3dDevice->EndScene();  
    m_pd3dDevice->Present( NULL, NULL, NULL, NULL );  
    return hr;  
}  
int CD3DDisplay::GetFormt()  
{  
    return D3D_ARGB32;    
}