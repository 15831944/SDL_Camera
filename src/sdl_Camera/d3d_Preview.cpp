#include "stdafx.h"
#include "d3d_Preview.h"

#define IMAGE_WIDTH            2592 //分辨率  
#define IMAGE_HEIGHT           1944//分辨率  

//set '1' to choose a type of file to play
//Read BGRA data
#define LOAD_BGRA    0
//Read YUV420P data
#define LOAD_YUV420P 0
//Read YUV422 data
#define LOAD_YUV422   1
//Width, Height
const int screen_w=548,screen_h=315;
const int pixel_w=2592,pixel_h=1944;

Cd3d_Preview::Cd3d_Preview(void)
{
	m_pDirect3D9= NULL;
	m_pDirect3DDevice= NULL;
	m_pDirect3DSurfaceRender= NULL;
}

Cd3d_Preview::~Cd3d_Preview(void)
{
	Cleanup();
}

void Cd3d_Preview::Cleanup()
{
	EnterCriticalSection(&m_critial);
	if(m_pDirect3DSurfaceRender)
		m_pDirect3DSurfaceRender->Release();
	if(m_pDirect3DDevice)
		m_pDirect3DDevice->Release();
	if(m_pDirect3D9)
		m_pDirect3D9->Release();
	LeaveCriticalSection(&m_critial);
}


int Cd3d_Preview::InitD3D( HWND hwnd, unsigned long lWidth, unsigned long lHeight )
{
	HRESULT lRet;
	InitializeCriticalSection(&m_critial);
	Cleanup();

	m_pDirect3D9 = Direct3DCreate9( D3D_SDK_VERSION );
	if( m_pDirect3D9 == NULL )
		return -1;

	D3DPRESENT_PARAMETERS d3dpp; 
	ZeroMemory( &d3dpp, sizeof(d3dpp) );
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

	GetClientRect(hwnd,&m_rtViewport);

	lRet=m_pDirect3D9->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,hwnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, &m_pDirect3DDevice );
	if(FAILED(lRet))
		return -1;

#if LOAD_BGRA
	lRet=m_pDirect3DDevice->CreateOffscreenPlainSurface(
		lWidth,lHeight,
		D3DFMT_X8R8G8B8,
		D3DPOOL_DEFAULT,
		&m_pDirect3DSurfaceRender,
		NULL);
#elif LOAD_YUV420P
	lRet=m_pDirect3DDevice->CreateOffscreenPlainSurface(
		lWidth,lHeight,
		(D3DFORMAT)MAKEFOURCC('Y', 'V', '1', '2'),
		D3DPOOL_DEFAULT,
		&m_pDirect3DSurfaceRender,
		NULL);
#elif LOAD_YUV422
	lRet=m_pDirect3DDevice->CreateOffscreenPlainSurface(
		lWidth,lHeight,
		(D3DFORMAT)MAKEFOURCC('Y', 'U', 'Y', '2'),
		D3DPOOL_DEFAULT,
		&m_pDirect3DSurfaceRender,
		NULL);
#endif


	if(FAILED(lRet))
		return -1;

	return 0;
}


bool Cd3d_Preview::Render(byte *src)
{
	HRESULT lRet;

	if(m_pDirect3DSurfaceRender == NULL)
		return -1;
	D3DLOCKED_RECT d3d_rect;
	lRet=m_pDirect3DSurfaceRender->LockRect(&d3d_rect,NULL,D3DLOCK_DONOTWAIT);
	if(FAILED(lRet))
		return -1;

	byte *pSrc = src;
	byte * pDest = (BYTE *)d3d_rect.pBits;
	int stride = d3d_rect.Pitch;
	unsigned long i = 0;

	//Copy Data
#if LOAD_BGRA
	int pixel_w_size=pixel_w*4;
	for(i=0; i< pixel_h; i++){
		memcpy( pDest, pSrc, pixel_w_size );
		pDest += stride;
		pSrc += pixel_w_size;
	}
#elif LOAD_YUV420P
	for(i = 0;i < pixel_h;i ++){
		memcpy(pDest + i * stride,pSrc + i * pixel_w*2, pixel_w*2);
	}
	for(i = 0;i < pixel_h/2;i ++){
		memcpy(pDest + stride * pixel_h + i * stride / 2,pSrc + pixel_w * pixel_h + pixel_w * pixel_h / 4 + i * pixel_w / 2, pixel_w / 2);
	}
	for(i = 0;i < pixel_h/2;i ++){
		memcpy(pDest + stride * pixel_h + stride * pixel_h / 4 + i * stride / 2,pSrc + pixel_w * pixel_h + i * pixel_w / 2, pixel_w / 2);
	}
#elif LOAD_YUV422
	for(i = 0;i < pixel_h;i ++){
		memcpy(pDest + i * stride,pSrc + i * pixel_w*2, pixel_w*2);
	}
#endif

	lRet=m_pDirect3DSurfaceRender->UnlockRect();
	if(FAILED(lRet))
		return -1;

	if (m_pDirect3DDevice == NULL)
		return -1;

	m_pDirect3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,0), 1.0f, 0 );
	m_pDirect3DDevice->BeginScene();
	IDirect3DSurface9 * pBackBuffer = NULL;

	m_pDirect3DDevice->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&pBackBuffer);
	m_pDirect3DDevice->StretchRect(m_pDirect3DSurfaceRender,NULL,pBackBuffer,&m_rtViewport,D3DTEXF_LINEAR);
	m_pDirect3DDevice->EndScene();
	m_pDirect3DDevice->Present( NULL, NULL, NULL, NULL );
	pBackBuffer->Release();

	return true;
}

