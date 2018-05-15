//经典的YUV转换到RGB的函数的头文件  
#if !defined(AFX_COLOURSPACECONVERT_H__92911B35_FC87_4C19_9007_1D735942D76E__INCLUDED_)  
#define AFX_COLOURSPACECONVERT_H__92911B35_FC87_4C19_9007_1D735942D76E__INCLUDED_  
#if _MSC_VER > 1000  
#pragma once  
#endif // _MSC_VER > 1000  
class CColourSpaceConvert    
{  
public:  
    CColourSpaceConvert();  
    virtual ~CColourSpaceConvert();  
void YUV2RGB(unsigned char *src0,unsigned char *src1,unsigned char *src2,unsigned char *dst_ori, int width,int height);  
      
private:  
//YUV===>RGB  
     long int crv_tab[256];  
     long int cbu_tab[256];  
     long int cgu_tab[256];  
     long int cgv_tab[256];  
     long int tab_76309[256];  
     unsigned char clp[1024];  
     void InitConvertTable();  
};  
#endif  