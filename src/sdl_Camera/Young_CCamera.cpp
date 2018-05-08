#include "stdafx.h"
#include "Young_CCamera.h"
#include "Young_CharConcvert.h"
#pragma comment(lib,"Strmiids.lib") 

bool CCamera::BindFilter(int nCameraIndex, IBaseFilter **pFilter)
{
    if (nCameraIndex < 0)
        return false;

    // enumerate all video capture devices
    CComPtr<ICreateDevEnum> pCreateDevEnum;
    HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER,
        IID_ICreateDevEnum, (void**)&pCreateDevEnum);
    if (hr != NOERROR)
    {
        return false;
    }

    CComPtr<IEnumMoniker> pEm;      // This will access the actual devices
    // 为指定的Filter注册类型目录创建一个枚举器,并获得 IEnumMoniker接口 （Video Capture Sources    ）
    // 可以访问捕捉设备的列表了
    hr = pCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEm, 0);
    if (hr != NOERROR) 
    {
        return false;
    }

    pEm->Reset();       // Go to the start of the enumerated list
    ULONG cFetched;
    IMoniker *pM;
    int index =0;
    while(hr = pEm->Next(1, &pM, &cFetched), hr==S_OK, index <= nCameraIndex)
    {
        IPropertyBag *pBag;
        // BindToStorage之后就可以访问设备标识的属性集了。
        // Binds to the storage for the specified object. Unlike the IMoniker::BindToObject method, 
        //      this method does not activate the object identified by the moniker.
        hr = pM->BindToStorage(0, 0, IID_IPropertyBag, (void**)&pBag);
        if(SUCCEEDED(hr)) 
        {
            VARIANT var;
            var.vt = VT_BSTR;
            hr = pBag->Read(L"FriendlyName", &var, NULL);
            if (hr == NOERROR) 
            {
                if (index == nCameraIndex)
                {
                    // BindToObject将某个设备标识绑定到一个DirectShow Filter，
                    //     然后调用IFilterGraph::AddFilter加入到Filter Graph中，这个设备就可以参与工作了
                    // 调用IMoniker::BindToObject建立一个和选择的device联合的filter，
                    //      并且装载filter的属性(CLSID,FriendlyName, and DevicePath)。
                    // Binds to the specified object. The binding process involves finding the object, 
                    //      putting it into the running state if necessary, 
                    //      and providing the caller with a pointer to a specified interface on the identified object.
                    pM->BindToObject(0, 0, IID_IBaseFilter, (void**)pFilter);
                }
                SysFreeString(var.bstrVal);
            }
            pBag->Release();
        }
        pM->Release();
        index++;
    }

	pEm = NULL;
    pCreateDevEnum = NULL;
    return true;
}

//创建 Graph
bool CCamera::InitGraph()
{
	//创建IGraphBuilder
	HRESULT hr = CoCreateInstance (CLSID_FilterGraph, NULL, CLSCTX_INPROC,
		IID_IGraphBuilder, (void **) &m_pGraph);
	if (FAILED(hr))
	{
		return false;
	}

	// Create the capture graph builder
	hr = CoCreateInstance (CLSID_CaptureGraphBuilder2 , NULL, CLSCTX_INPROC,
		IID_ICaptureGraphBuilder2, (void **) &m_pCapture);
	if (FAILED(hr))
	{
		return false;
	}

	// Attach the filter graph to the capture graph
	hr = m_pCapture->SetFiltergraph(m_pGraph);
	if (FAILED(hr))
	{
		return false;
	}

	// Obtain interfaces for media control and Video Window
	hr = m_pGraph->QueryInterface(IID_IMediaControl,(LPVOID *) &m_pMC);
	if (FAILED(hr))
	{
		return false;
	}

	hr = m_pGraph->QueryInterface(IID_IVideoWindow, (LPVOID *) &m_pVW);
	if (FAILED(hr))
	{
		return false;
	}

	hr = m_pGraph->QueryInterface(IID_IMediaEvent, (LPVOID *) &m_pME);
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}
//创建 SampleGrabber
bool CCamera::CreateSampleGrabber()
{
	//添加取图的filter
	if(!CreateSampleGrabber(m_pGrabberFilter,m_pGrabber))
	{
		return false;
	}

	//参数 1 BufferCB
	//参数 0 SampleCB
	HRESULT hr = m_pGrabber->SetCallback(&m_SampleGrabberCB, 1); //回调
	if (FAILED(hr)) 
	{
		return false;
	}

	return true;
}


//创建SampleGrabber Filter
bool CCamera::CreateSampleGrabber(CComPtr<IBaseFilter> &pGrabberFilter, CComPtr<ISampleGrabber> &pGrabber)
{
	//添加取图的SampleGrabber Filter
	//SAFE_RELEASE(m_pGrabberFilter);
	HRESULT hr = CoCreateInstance (CLSID_SampleGrabber, NULL, CLSCTX_INPROC_SERVER,
		IID_IBaseFilter, (void **) &pGrabberFilter);
	if (FAILED(hr))
	{
		return false;
	}
	if(FAILED(pGrabberFilter->QueryInterface(IID_ISampleGrabber, (void**)&pGrabber)))
	{
		return false;
	}

	//带缓冲
	hr = pGrabber->SetBufferSamples(true);	
	if (FAILED(hr)) 
	{
		return false;
	}
	//获取到一个BMP以后，不允许停止，而是继续采集；
	//如果为param设为true，当拍摄完一张后，会停止预览
	hr = pGrabber->SetOneShot(0);

	if (FAILED(hr)) 
	{
		return false;
	}

	//设置RGB24
	AM_MEDIA_TYPE mt;
	ZeroMemory(&mt, sizeof(AM_MEDIA_TYPE));
	mt.majortype = MEDIATYPE_Video;
	mt.subtype = MEDIASUBTYPE_YUY2;//出图格式
	hr = pGrabber->SetMediaType(&mt);
	if (!SUCCEEDED(hr))
	{
		return false;
	}
	MYFREEMEDIATYPE(mt);

	return true;
}
//获得设备列表
//名称
//数量
int CCamera::CameraCount()
{
	//std::vector<Device> vectorMDevice;
   CoInitialize(NULL);
    int nCount =0;//设备数量

    // enumerate all video capture devices
    CComPtr<ICreateDevEnum> pCreateDevEnum;
    HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER,
        IID_ICreateDevEnum, (void**)&pCreateDevEnum);

    CComPtr<IEnumMoniker> pEm;
    // 使用接口方法ICreateDevEnum::CreateClassEnumerator为指定的Filter注册类型目录创建一个枚举器,并获得 IEnumMoniker接口;
    hr = pCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEm, 0);
    if (hr != NOERROR) 
    {
        return nCount;
    }

    pEm->Reset();
    ULONG cFetched;
    IMoniker *pM;
    while(hr = pEm->Next(1, &pM, &cFetched), hr==S_OK)
    {
        nCount++;
    }

    pCreateDevEnum = NULL;
    pEm = NULL;
    return nCount;
}
// 设置分辨率
//需要为Pin接口单独做一个函数
HRESULT CCamera::SetFormat(ICaptureGraphBuilder2 *pBuilder, IBaseFilter *pCap, 
	long lWidth, long lHeight, unsigned short iColorBit, __int64 iRate,
	GUID subtype,bool defaultType,
	GUID pinCategory)
{
	if (pBuilder == NULL)
	{
		return S_FALSE;
	}
	HRESULT hr;
	VIDEOINFOHEADER*   phead;   
	IAMStreamConfig*   iconfig = NULL;   


	if(iconfig)iconfig->Release();
	hr = pBuilder->FindInterface(&pinCategory,   &MEDIATYPE_Interleaved, pCap,IID_IAMStreamConfig,(void**)&iconfig);   

	if(FAILED(hr))     
	{   if(iconfig)iconfig->Release();
	hr = pBuilder ->FindInterface(&pinCategory,   
		&MEDIATYPE_Video,
		pCap,   
		IID_IAMStreamConfig,(void**)&iconfig);   
	}   

	if(FAILED(hr))   
	{
		if(iconfig)iconfig->Release();
		return hr;   
	}

	AM_MEDIA_TYPE*   pmt = NULL;   
	if(iconfig->GetFormat(&pmt) != S_OK)
	{	if(iconfig)iconfig->Release();
	return   E_UNEXPECTED;   
	}

	if(pmt->formattype == FORMAT_VideoInfo)   
	{   
		if (!defaultType)
		{
			pmt->subtype = subtype;
		}
		phead = (VIDEOINFOHEADER*)pmt -> pbFormat;   
		phead -> bmiHeader.biWidth = lWidth;   
		phead -> bmiHeader.biHeight = lHeight;   

		if((hr = iconfig -> SetFormat(pmt)) != S_OK)  
		{
			if(iconfig)iconfig->Release();
			return E_UNEXPECTED;   
		}
	} 

	MYFREEMEDIATYPE(*pmt);

	iconfig -> Release();   
	iconfig = NULL;  

	return   S_OK;
}
//设置分辨率
bool CCamera::SetResolutionFormat(int nWidth,int nHeight,GUID subtype,bool defaultType,GUID pinCategory)
{
	HRESULT hr;
	hr = SetFormat(m_pCapture, m_pDeviceFilter, nWidth, nHeight, 24, 15,subtype,defaultType,pinCategory);	
	if (FAILED(hr))
	{
		return false;
	} 
	return true;
}

bool CCamera::OpenCamera(int nCamID, int nWidth /*=320*/, int nHeight /*=240*/)
{
	HRESULT hrInitCom = CoInitialize(NULL);    
	//////////////////////////////////////////////////////////////////
	//Step 1 初始化GrapBuilder
	//////////////////////////////////////////////////////////////////
	//初始化Grap
	if(!this->InitGraph())
	{
		if(!hrInitCom)CoUninitialize();
		return false;
	}
	//////////////////////////////////////////////////////////////////
	//Step 2 创建DeviceFilter
	//////////////////////////////////////////////////////////////////
	// Bind Device Filter.  We know the device because the id was passed in                      
	//获取Filter所有的Pin以及所有的Resolution
	if(!this->BindFilter(0, &m_pDeviceFilter))
	{
		if(!hrInitCom)CoUninitialize();
		return false;
	}

	//GUID guidSubtype = MEDIASUBTYPE_YUY2;

	//////////////////////////////////////////////////////////////////
	//Step 3 创建SampleGrabber Filter
	//////////////////////////////////////////////////////////////////
	//创建SampleGrabber Filter
	if(!this->CreateSampleGrabber())
	{
		if(!hrInitCom)CoUninitialize();
		return false;
	}
	//////////////////////////////////////////////////////////////////
	//Step 4 设置分辨率
	//////////////////////////////////////////////////////////////////
	//获取可以使用的分辨率和类型，有可能用户使用的数据有误
	if(!this->SetResolutionFormat(nWidth,nHeight))
	{
		if(!hrInitCom)CoUninitialize();
		return false;
	}

	if(!hrInitCom)CoUninitialize();
    return true;
}
//开始视频
bool CCamera::Start()
{	
	//添加Filter
	m_pGraph->AddFilter(m_pDeviceFilter, NULL); 
	m_pGraph->AddFilter(m_pGrabberFilter, NULL); 

	if(!this->RenderStream())
	{	
		return false;
	}
	//this->m_bConnected = true;
	//m_SampleGrabberCB.CreateThread();
	OAFilterState pfs,pfs2;
	HRESULT hr = m_pMC->GetState(100,&pfs); //pfs = 0；表示所有Filter处于停止状态
	HRESULT hRet =  m_pMC->Run();
	HRESULT hr2 = m_pMC->GetState(100,&pfs2);//pfs = 2；表示所有Filter处于运行状态	


	if(SUCCEEDED(hRet) || pfs2 == 2)
	{
		return true;
	}

	return false;
}
//RenderSteam
bool CCamera::RenderStream()
{
	if(false == RenderStream(m_pGrabberFilter,PIN_CATEGORY_PREVIEW))
	{
		return false;
	}

	return true;
}
//针对Preview模式RenderStrem
bool CCamera::RenderStream(CComPtr<IBaseFilter>& pGrabberFilter, GUID pinCategory)
{
	HRESULT hr = m_pCapture->RenderStream(
		&pinCategory,
		&MEDIATYPE_Video,
		m_pDeviceFilter, 
		0,
		pGrabberFilter
		);

	if (!SUCCEEDED(hr))
	{
		return false;
	}
	return true;
}
