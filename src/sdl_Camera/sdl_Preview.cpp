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

long Csdl_Preview::sdl_Unit(void)
{
	//destory renderer 
	if (pRender)
	{ 
		SDL_DestroyRenderer(pRender);
	}
	// Close and destroy the window 
	SDL_DestroyWindow(pWindow); 
	// Clean up 
	SDL_Quit();

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

long Csdl_Preview::sdl_DrawLine()
{
	while (1)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				//SDL_BlitSurface(empty, NULL, line, NULL);
				//Draw_HLine(line, 0, event.button.y, 480, 0xff00ff);
				//Draw_VLine(line, event.button.x, 0, 800, 0xff00ff);
				//SDL_BlitSurface(line, NULL, screen, NULL);
				//SDL_Flip(screen);
			}
			else if (event.type == SDL_KEYDOWN)
			{
				//quit = 1;
			}
		}
	}
	//SDL_Rect TempRect = {100, 100, 200, 200};
	//SDL_SetRenderDrawColor(pRender,0,255,0,255);
	//SDL_RenderDrawRect(pRender,&TempRect);
	//SDL_RenderPresent( pRender );  
	//SDL_Delay(40);
	return 0;
}

void sdl_ZoomIn()
{

}
void sdl_ZoomOut()
{

}