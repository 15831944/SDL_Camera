#include "stdafx.h"  
#include "D3DDisplay.h"  
#include "DEFINE.h"  
#ifdef _DEBUG  
#undef THIS_FILE  
static char THIS_FILE[]=__FILE__;  
#define new DEBUG_NEW  
#endif  
struct CUSTOMVERTEX//���㻺��  
{       
    float x,y,z,rhw;    //��������   
    DWORD color;        //������ɫ  
    float tu,tv;        //��������  
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
HRESULT CD3DDisplay::InitD3D(HWND hWnd)//��ʼ��d3d  
{  
    D3DPRESENT_PARAMETERS d3dpp;//��ʾ��������  
    if(hWnd == NULL)  
        return -1;  
    m_hWnd = hWnd;  
    //D3DFMT_A8R8G8B8��ʾһ��32λ���أ�����ʼ��8λΪALPHAͨ����8λ�������ɫ��8λ�������ɫ��8λ�������ɫ  
    m_Format = D3DFMT_A8R8G8B8;  
    if(NULL == (m_pD3D = Direct3DCreate9( D3D_SDK_VERSION )))//����d3d����  
        return -1;  
    ZeroMemory( &d3dpp, sizeof(d3dpp) );//�����ʾ�������������ò���  
    d3dpp.Windowed = TRUE;//FALSE����ʾҪ��Ⱦȫ�������ΪTRUE����ʾҪ��Ⱦ����  
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;//��������֧�ֵ�Ч�����ͣ�ָ�������ڽ�������D����α�������  
    //D3DSWAPEFFECT_DISCARD����󱸻������Ķ��������Ƶ���Ļ�Ϻ�,�󱸻������Ķ�����û��ʲô�ÿ��Զ���  
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;//�󱸻���ĸ�ʽD3DFMT_UNKNOWN����ʱ������ʹ������ĸ�ʽ  
    d3dpp.EnableAutoDepthStencil = TRUE;//���Ҫʹ�û����ģ�建���������ΪTRUE  
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;//�����������Ȼ�����ô���������Ϊ����趨�����ʽ  
    //D3DFMT_16��Ȼ���  
    d3dpp.BackBufferWidth = IMAGE_WIDTH;//�󱸻���Ŀ�Ⱥ͸߶�  
    d3dpp.BackBufferHeight = IMAGE_HEIGHT;//��ȫ��ģʽ�������ߵ�ֵ��������Կ���֧�ֵķֱ���  
      
    if( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT,  
        D3DDEVTYPE_HAL,   
        m_hWnd,  
        D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,  
        &d3dpp,   
        &m_pd3dDevice ) ) )//����d3d�豸  
        return -1;  
    return 0;  
}  
void CD3DDisplay::ClearD3D()//�ͷ�d3d  
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
HRESULT CD3DDisplay::CreateTexture()//��������  
{  
    HRESULT hr = 0;  
    D3DSURFACE_DESC ddsd;  
    hr = m_pd3dDevice->CreateTexture( IMAGE_WIDTH, IMAGE_HEIGHT, 1, 0, m_Format,   
        D3DPOOL_MANAGED, &m_pTexturesvideo, NULL);  
    if( FAILED(hr))  
    {  
        return -1;  
    }  
hr = m_pTexturesvideo->GetLevelDesc( 0, &ddsd );//��ü���ͼƬ�Ŀ�͸߼�Щ��Ϣ  
    if(FAILED(hr))  
        return -1;  
    if ((ddsd.Format != D3DFMT_A8R8G8B8) && (ddsd.Format != D3DFMT_YUY2))   
        return -1;  
  
    return 0;  
}  
HRESULT CD3DDisplay::InitGeometry()//������Ƶ���ν������㻺�沢��ʼ��������  
{  
    HRESULT hr = 0;  
    CUSTOMVERTEX* pVertices = NULL;  
    RECT rect;  
    //������Ƶ����  
    rect.top = 0;             
    rect.left = 0;  
    rect.right = IMAGE_WIDTH;  
    rect.bottom = IMAGE_HEIGHT;  
    CUSTOMVERTEX vertices[4] =  //���㻺��  
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
    m_pd3dDevice->CreateVertexBuffer(sizeof(vertices), //ָ���������Ĵ�С  
        0, //ָ�����㻺����������  
        D3DFVF_CUSTOMVERTEX, //��ʼ�����Ķ����ʽ  
        D3DPOOL_DEFAULT, //ָ�����㻺�������ڴ�����  
        &m_pVBvideo, //��Ƶ���εĶ��㻺�����ӿ�ָ��  
        NULL ); //��������  
m_pVBvideo->Lock( 0, sizeof(vertices), (void**)&pVertices, 0 ); //��䶥�㻺����  
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
HRESULT CD3DDisplay::RenderTOSrceen()//���ݵ���Ļ  
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
    //��ʾ��Ƶ  
    hr = m_pd3dDevice->SetTexture( 0, m_pTexturesvideo );  
    m_pd3dDevice->SetStreamSource( 0, m_pVBvideo, 0, sizeof(CUSTOMVERTEX));   
    hr = m_pd3dDevice->SetVertexShader( NULL );//���ÿɱ����Ⱦ�ܵ���Shader����  
    m_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX ); //���ù̶���Ⱦ�ܵ����Ķ����ʽ  
    m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );//��Ⱦһ�������  
    m_pd3dDevice->SetTexture( 0, NULL );   
    m_pd3dDevice->EndScene();  
    m_pd3dDevice->Present( NULL, NULL, NULL, NULL );  
    return hr;  
}  
int CD3DDisplay::GetFormt()  
{  
    return D3D_ARGB32;    
}