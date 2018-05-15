#pragma once


#include <atlbase.h>
#include "qedit.h"
#include "dshow.h"
#include <windows.h>


#include <string>
using namespace std ;
#include "Young_SampleGrabberCB.h"


#define MYFREEMEDIATYPE(mt)    {if ((mt).cbFormat != 0)        \
{CoTaskMemFree((PVOID)(mt).pbFormat);    \
	(mt).cbFormat = 0;                        \
	(mt).pbFormat = NULL;                    \
}                                            \
	if ((mt).pUnk != NULL)                        \
{                                            \
	(mt).pUnk->Release();                    \
	(mt).pUnk = NULL;                        \
}} 


class CCamera
{
public:
    CCamera(void){}
    ~CCamera(void){}
private:
	CSampleGrabberCB m_SampleGrabberCB;
	//DXS变量
private:
	CComPtr<IFilterGraph2> m_pGraph ;  
	//视频捕捉
	CComPtr<ICaptureGraphBuilder2> m_pCapture ;
	//用于播放控制（开始、暂停等）
	CComPtr<IMediaControl> m_pMC;
	//用于控制视频的显示位置和大小
	CComPtr<IVideoWindow> m_pVW ;
	//用于事件交互
	CComPtr<IMediaEventEx> m_pME ;
	//Source Filter（Camera）
	CComPtr<IBaseFilter> m_pDeviceFilter;
	//取图的filter
	CComPtr<IBaseFilter> m_pGrabberFilter ;	
	//捕捉filter（被m_pGrabberFilter创建）
	CComPtr<ISampleGrabber> m_pGrabber;
public:
	static int CameraCount(); 
	bool BindFilter(int nCameraIndex, IBaseFilter **pFilter);
    bool OpenCamera(int nCamID, int nWidth =320, int nHeight =240);
    bool InitGraph();
	//创建SampleGrabber Filter
	bool CreateSampleGrabber();
	//创建SampleGrabber Filter
	static bool CreateSampleGrabber(CComPtr<IBaseFilter>& pGrabberFilter, CComPtr<ISampleGrabber>& pGrabber);
	bool RenderStream();
	bool RenderStream(CComPtr<IBaseFilter>& pGrabberFilter, GUID pinCategory);
	//设置分辨率和格式  出图格式YOUNG
	bool SetResolutionFormat(int nWidth,int nHeight,GUID subtype =/*MEDIASUBTYPE_RGB24*/MEDIASUBTYPE_YUY2/*MEDIASUBTYPE_MJPG*/,bool defaultType = false,
		GUID pinCategory = PIN_CATEGORY_CAPTURE);
	bool Start();
	// 设置分辨率
	//需要为Pin接口单独做一个函数
	HRESULT SetFormat(ICaptureGraphBuilder2 *pBuilder, IBaseFilter *pCap, 
		long lWidth, long lHeight, unsigned short iColorBit, __int64 iRate,
		GUID subtype,bool defaultType,
		GUID pinCategory);

	 bool camRegCallBackPreviewImage(callBackPreviewImage fun)
	{
		m_SampleGrabberCB.RegOutCallBack(fun);
		return 1;
	}
};