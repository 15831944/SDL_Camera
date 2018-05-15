#include "stdafx.h"
#include "sdl_Preview.h"

Csdl_Preview::Csdl_Preview(void)
{
	pWindow = NULL;
}

Csdl_Preview::~Csdl_Preview(void)
{

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

long Csdl_Preview::sdl_SetHwnd(HWND hWnd,int width,int height)
{
	//初始化
	if(SDL_Init(SDL_INIT_VIDEO)<0)  {  
		//Window could not be created! 
		return -1;  
	}  
	//创建窗口
	pWindow  = SDL_CreateWindowFrom(hWnd);
	if(pWindow ==NULL) {  
		//pWindow could not be created! 
		return -1; 
	}  
	/*int*/ iPreviewWidth = 0;
	/*int */iPreviewHeight = 0;
	SDL_GetWindowSize( pWindow, &iPreviewWidth, &iPreviewHeight );
	dstRect.x = 0;
	dstRect.y = 0;
	dstRect.h = iPreviewHeight;
	dstRect.w = iPreviewWidth ;
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

	SDL_RenderCopy( pRender, pTexture, NULL, &dstRect );  
	SDL_RenderPresent( pRender );  
	SDL_Delay(40);

	return 0;
}
//int REFRESH_EVENT  = 1;
//int BREAK_EVENT = 2;
//int thread_exit= -1;
//int refresh_video(void *opaque)
//{
//	thread_exit = 0;
//	while (thread_exit == 0)
//	{
//		SDL_Event event;
//		event.type = REFRESH_EVENT;
//		SDL_PushEvent(&event);
//		//SDL_Delay(40);
//	}
//	thread_exit = 0;
//	SDL_Event event;
//	event.type = BREAK_EVENT;
//	SDL_PushEvent(&event);
//	return 0;
//}

long Csdl_Preview::sdl_PreviewImageEx()
{
	//SDL_Thread *refresh_thread = SDL_CreateThread(refresh_video,NULL,NULL);
	SDL_Event event;
	while (1)
	{
		SDL_WaitEvent(&event);
		if(event.type == SDL_KEYDOWN){
			
		}
		else if(event.type == SDL_QUIT){

		}
		else if(event.type == SDL_BUTTON_LEFT){

		}
	}

	return 0;
}


long Csdl_Preview::sdl_DrawLine()
{

	SDL_Rect TempRect = {100, 100, 200, 200};
	SDL_SetRenderDrawColor(pRender,0,255,0,255);
	SDL_RenderDrawRect(pRender,&TempRect);
	SDL_RenderPresent( pRender );  
	return 0;
}

void Csdl_Preview::sdl_ZoomIn()
{
	float Ratio = 1.1;
	float yOffset  =  dstRect.h*(Ratio - 1)/2;
	float xOffset = dstRect.w*(Ratio - 1)/2;
	dstRect.x -=yOffset;
	dstRect.y -=xOffset;
	dstRect.h*=Ratio;
	dstRect.w *= Ratio;
}

void Csdl_Preview::sdl_ZoomOut()
{
	float Ratio = 1.1;
	float yOffset  =  dstRect.h*(Ratio - 1)/2;
	float xOffset = dstRect.w*(Ratio - 1)/2;
	dstRect.x +=yOffset;
	dstRect.y +=xOffset;
	dstRect.h /=Ratio;
	dstRect.w /= Ratio;
}