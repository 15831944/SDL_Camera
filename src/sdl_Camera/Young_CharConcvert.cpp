#include"stdafx.h"  
#include "Young_CharConcvert.h"

CConvert::CConvert()
{

}
CConvert::~CConvert()
{


}

 LPSTR CConvert::WideChar2MBCS( const CString& strCS )
{
	const UINT wLen = strCS.GetLength() + 1;
	UINT aLen = WideCharToMultiByte(CP_ACP,0,strCS,wLen,NULL,0,NULL,NULL);
	LPSTR lpa = new char[aLen];
	WideCharToMultiByte(CP_ACP,0,strCS,wLen,lpa,aLen,NULL,NULL);
	return lpa;
}

std::string CConvert::WideChar2StdStr(const CString& strcs)
{
	LPSTR l = WideChar2MBCS(strcs);
	std::string stdStr(l);
	delete [] l;
	return stdStr;
}

LPOLESTR CConvert::MBCS2WideChar( LPCSTR lpa )
{
	size_t aLen = strlen(lpa) + 1;
	int wLen = MultiByteToWideChar(CP_ACP,0,lpa,aLen,NULL,0);
	LPOLESTR lpw = new WCHAR[wLen];
	MultiByteToWideChar(CP_ACP,0,lpa,aLen,lpw,wLen);
	return lpw;
}

CString CConvert::MBCS2CString( LPCSTR lpa )
{
	LPOLESTR lpw = MBCS2WideChar(lpa);
	CString cstring(lpw);
	delete [] lpw;
	return  cstring;
}

CString CConvert::StdStr2CSting( const std::string& stdStr )
{
	return MBCS2CString(stdStr.c_str());
}

//#include<string>
//using namespace std;
//将string转换成wstring
wstring CConvert::string2wstring(string str)
{
	wstring result;
	//获取缓冲区大小，并申请空间，缓冲区大小按字符计算
	int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), NULL, 0);
	TCHAR* buffer = new TCHAR[len + 1];
	//多字节编码转换成宽字节编码
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), buffer, len);
	buffer[len] = '\0';				//添加字符串结尾
	//删除缓冲区并返回值
	result.append(buffer);
	delete[] buffer;
	return result;
}
//将wstring转换成string
string CConvert::wstring2string(wstring wstr)
{
	string result;
	//获取缓冲区大小，并申请空间，缓冲区大小事按字节计算的
	int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), NULL, 0, NULL, NULL);
	char* buffer = new char[len + 1];
	//宽字节编码转换成多字节编码
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), buffer, len, NULL, NULL);
	buffer[len] = '\0';
	//删除缓冲区并返回值
	result.append(buffer);
	delete[] buffer;
	return result;
}