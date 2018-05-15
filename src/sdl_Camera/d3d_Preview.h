#pragma once  
#include "d3d9.h"

#pragma  comment(lib,"d3d9.lib")

#define YOUNG                        0

class Cd3d_Preview
{
public:
	Cd3d_Preview(void);
	virtual~Cd3d_Preview(void);

#if YOUNG
	int GetFormt();  
	HRESULT RenderSample(BYTE* pSampleBuffer);  
	HRESULT RenderTOSrceen();  
	HRESULT InitGeometry();  
	HRESULT CreateTexture();  
	HRESULT InitD3D(HWND hWnd);  
	void ClearD3D();  

private:  
	LPDIRECT3D9 m_pD3D;
	LPDIRECT3DDEVICE9 m_pd3dDevice;
	LPDIRECT3DTEXTURE9 m_pTexturesvideo;
	LPDIRECT3DVERTEXBUFFER9 m_pVBvideo;//视频矩形的顶点缓存区接口指针  
	LPDIRECT3DTEXTURE9 m_pTexturesbmp;
	LPDIRECT3DVERTEXBUFFER9 m_pVBbmp;//位图矩形的顶点缓存区接口指针  

	HWND m_hWnd;  
	D3DFORMAT m_Format;  
	//LPD3DXFONT m_p2Dfont;  

	//long d3d_Init(void);
	//long d3d_SetHwnd(HWND hWnd,int width,int height);
#else

	CRITICAL_SECTION  m_critial;
	IDirect3D9 *m_pDirect3D9 ;
	IDirect3DDevice9 *m_pDirect3DDevice ;
	IDirect3DSurface9 *m_pDirect3DSurfaceRender  ;
	RECT m_rtViewport;
	void Cleanup();
	int InitD3D( HWND hwnd, unsigned long lWidth, unsigned long lHeight );
	bool Render(byte*src);
#endif
};
