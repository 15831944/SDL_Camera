#include "stdafx.h"
#include "sdl_Preview.h"

Csdl_Preview::Csdl_Preview(void)
{
	pWindow = NULL;
}

Csdl_Preview::~Csdl_Preview(void)
{

}

long Csdl_Preview::sdl_Init(void)
{
	//初始化
	if(SDL_Init(SDL_INIT_VIDEO)<0)  {  
		//Window could not be created! 
		return -1;  
	}  
	return 0;
}

long Csdl_Preview::SetHwnd(HWND hWnd,int width,int height)
{
	//创建窗口
	pWindow  = SDL_CreateWindowFrom(hWnd);
	if(pWindow ==NULL) {  
		//pWindow could not be created! 
		return -1; 
	}  
	int iWidth = 0;
	int iHeight = 0;
	SDL_GetWindowSize( pWindow, &iWidth, &iHeight );
	dstRect.x = 0;
	dstRect.y = 0;
	dstRect.h = iHeight;
	dstRect.w = iWidth;
	//创建渲染器，
	pRender = SDL_CreateRenderer( pWindow, -1, 0 );
	//创建纹理
	pTexture = SDL_CreateTexture( pRender,SDL_PIXELFORMAT_YUY2, SDL_TEXTUREACCESS_STREAMING, width, height);

	return 0;
}

long Csdl_Preview::sdl_PreviewImage(BYTE * buf, int width,int height)
{
	SDL_Rect imageRect;
	imageRect.x = 0;
	imageRect.y = 0;
	imageRect.w = width;
	imageRect.h = height;

	 int iPitch = width*SDL_BYTESPERPIXEL(SDL_PIXELFORMAT_YUY2);    


	SDL_UpdateTexture( pTexture,
								   &imageRect,
								   buf,
								   	iPitch) ;  

	SDL_RenderClear( pRender );  
	SDL_RenderCopy( pRender, pTexture, NULL, &dstRect );  
	SDL_RenderPresent( pRender );  
	SDL_Delay(40);


	return 0;
}