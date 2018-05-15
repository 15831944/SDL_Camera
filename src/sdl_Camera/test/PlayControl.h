#if !defined(AFX_PLAYCONTROL_H__C3D14174_0157_4809_B573_DEA1F90DADB6__INCLUDED_)  
#define AFX_PLAYCONTROL_H__C3D14174_0157_4809_B573_DEA1F90DADB6__INCLUDED_  
  
#if _MSC_VER > 1000  
#pragma once  
#endif // _MSC_VER > 1000  
#include "D3DDisplay.h"  
#include "DEFINE.h"  
#include <stdio.h>  
#define WM_MANAGE WM_USER + 1001  
 class CPlayControl :public CWinThread  
{  
    DECLARE_DYNCREATE(CPlayControl)  
public:  
    CPlayControl();  
    virtual ~CPlayControl();  
virtual BOOL InitInstance();  
    virtual int ExitInstance();  
    int Init(HWND hwnd, char *filename);  
    void Play();  
    void Stop();  
    void Pause();  
private:  
    CD3DDisplay m_D3D;  
    BYTE *yuvbuf;  
    BYTE *rgbbuf;  
    FILE *f_in;//输入的文件句柄  
    int m_State;  
protected:  
      
  
    void OnManage(WPARAM wParam, LPARAM lParam);  
    DECLARE_MESSAGE_MAP()  
          
};  
#endif   