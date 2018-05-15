#if !defined(AFX_D3DDISPLAY_H__D2B5461A_AF44_411A_8FEA_8439E0FB5628__INCLUDED_)  
#define AFX_D3DDISPLAY_H__D2B5461A_AF44_411A_8FEA_8439E0FB5628__INCLUDED_  
#if _MSC_VER > 1000  
#pragma once  
#endif // _MSC_VER > 1000  
#include <d3dx9.h>  
class CD3DDisplay    
{  
public:  
    int GetFormt();  
    HRESULT RenderSample(BYTE* pSampleBuffer);  
    HRESULT RenderTOSrceen();  
    HRESULT InitGeometry();  
    HRESULT CreateTexture();  
    HRESULT InitD3D(HWND hWnd);  
    void ClearD3D();  
    CD3DDisplay();  
    virtual ~CD3DDisplay();  
private:  
    LPDIRECT3D9             m_pD3D;   
    LPDIRECT3DDEVICE9       m_pd3dDevice;   
    LPDIRECT3DTEXTURE9      m_pTexturesvideo;   
    LPDIRECT3DVERTEXBUFFER9 m_pVBvideo;     //视频矩形的顶点缓存区接口指针  
    LPDIRECT3DTEXTURE9      m_pTexturesbmp;  
    LPDIRECT3DVERTEXBUFFER9 m_pVBbmp;       //位图矩形的顶点缓存区接口指针  
  
    HWND m_hWnd;  
    D3DFORMAT m_Format;  
    LPD3DXFONT m_p2Dfont;  
};  
#endif  