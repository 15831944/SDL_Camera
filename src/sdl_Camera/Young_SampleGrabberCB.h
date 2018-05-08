#pragma once
#include <Qedit.h>

typedef bool (CALLBACK * callBackPreviewImage)(byte * src,long width,long height,long size);
class CSampleGrabberCB: public ISampleGrabberCB
{
	public:
		//构造
		CSampleGrabberCB(){}
		//析构
		~CSampleGrabberCB(){}
	public:
	    callBackPreviewImage m_pPutImage;
	public:
	    STDMETHODIMP BufferCB(double dblSampleTime, BYTE* pBuffer, long lBufferSize)
		{
			m_pPutImage(pBuffer,640,480,lBufferSize);
			return 1;
		}
		STDMETHODIMP SampleCB(double dblSampleTime, IMediaSample *pSample){return 1;}
		STDMETHODIMP QueryInterface(REFIID riid, void ** ppv){return 1;}
		STDMETHODIMP_(ULONG) AddRef() { return 2; }
		STDMETHODIMP_(ULONG) Release() { return 1; }

		//注册预览回调函数
		bool RegOutCallBack(callBackPreviewImage fun)
		{
			if (fun == m_pPutImage)
			{
				return true;
			}
			m_pPutImage = NULL;
			m_pPutImage = fun;
			return true;
		}
};