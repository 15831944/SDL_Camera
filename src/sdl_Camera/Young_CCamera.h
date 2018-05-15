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
	//DXS����
private:
	CComPtr<IFilterGraph2> m_pGraph ;  
	//��Ƶ��׽
	CComPtr<ICaptureGraphBuilder2> m_pCapture ;
	//���ڲ��ſ��ƣ���ʼ����ͣ�ȣ�
	CComPtr<IMediaControl> m_pMC;
	//���ڿ�����Ƶ����ʾλ�úʹ�С
	CComPtr<IVideoWindow> m_pVW ;
	//�����¼�����
	CComPtr<IMediaEventEx> m_pME ;
	//Source Filter��Camera��
	CComPtr<IBaseFilter> m_pDeviceFilter;
	//ȡͼ��filter
	CComPtr<IBaseFilter> m_pGrabberFilter ;	
	//��׽filter����m_pGrabberFilter������
	CComPtr<ISampleGrabber> m_pGrabber;
public:
	static int CameraCount(); 
	bool BindFilter(int nCameraIndex, IBaseFilter **pFilter);
    bool OpenCamera(int nCamID, int nWidth =320, int nHeight =240);
    bool InitGraph();
	//����SampleGrabber Filter
	bool CreateSampleGrabber();
	//����SampleGrabber Filter
	static bool CreateSampleGrabber(CComPtr<IBaseFilter>& pGrabberFilter, CComPtr<ISampleGrabber>& pGrabber);
	bool RenderStream();
	bool RenderStream(CComPtr<IBaseFilter>& pGrabberFilter, GUID pinCategory);
	//���÷ֱ��ʺ͸�ʽ  ��ͼ��ʽYOUNG
	bool SetResolutionFormat(int nWidth,int nHeight,GUID subtype =/*MEDIASUBTYPE_RGB24*/MEDIASUBTYPE_YUY2/*MEDIASUBTYPE_MJPG*/,bool defaultType = false,
		GUID pinCategory = PIN_CATEGORY_CAPTURE);
	bool Start();
	// ���÷ֱ���
	//��ҪΪPin�ӿڵ�����һ������
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