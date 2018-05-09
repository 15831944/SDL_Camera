#pragma once
#include "../../lib/include/SDL.h "
#include "../../lib/include/SDL_thread.h"


#pragma comment(lib,"../../lib/lib/x86/SDL2.lib")
#pragma comment(lib,"../../lib/lib/x86/SDL2main.lib")
#pragma comment(lib,"../../lib/lib/x86/SDL2test.lib")

class Csdl_Preview
{
public:
	Csdl_Preview(void);
	~Csdl_Preview(void);
public:
	SDL_Window  *pWindow;
	SDL_Renderer * pRender;
	SDL_Texture * pTexture ;
	SDL_Rect dstRect;
	SDL_Event event;
public:
	long sdl_Init(void);
	long sdl_Unit(void);
	long SetHwnd(HWND hWnd,int width,int height);
	long sdl_PreviewImage(BYTE * buf, int width,int height);
	long sdl_DrawLine();
	void sdl_ZoomIn();
	void sdl_ZoomOut();
};