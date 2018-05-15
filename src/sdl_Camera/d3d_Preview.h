#pragma once  
#include "d3d9.h"

#pragma  comment(lib,"d3d9.lib")


class Cd3d_Preview
{
public:
	Cd3d_Preview(void);
	virtual~Cd3d_Preview(void);
public:
	CRITICAL_SECTION  m_critial;
	IDirect3D9 *m_pDirect3D9 ;
	IDirect3DDevice9 *m_pDirect3DDevice ;
	IDirect3DSurface9 *m_pDirect3DSurfaceRender  ;
	RECT m_rtViewport;
	void Cleanup();
	int InitD3D( HWND hwnd, unsigned long lWidth, unsigned long lHeight );
	bool Render(byte*src);
};
