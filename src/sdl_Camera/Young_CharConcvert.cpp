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
//��stringת����wstring
wstring CConvert::string2wstring(string str)
{
	wstring result;
	//��ȡ��������С��������ռ䣬��������С���ַ�����
	int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), NULL, 0);
	TCHAR* buffer = new TCHAR[len + 1];
	//���ֽڱ���ת���ɿ��ֽڱ���
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), buffer, len);
	buffer[len] = '\0';				//����ַ�����β
	//ɾ��������������ֵ
	result.append(buffer);
	delete[] buffer;
	return result;
}
//��wstringת����string
string CConvert::wstring2string(wstring wstr)
{
	string result;
	//��ȡ��������С��������ռ䣬��������С�°��ֽڼ����
	int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), NULL, 0, NULL, NULL);
	char* buffer = new char[len + 1];
	//���ֽڱ���ת���ɶ��ֽڱ���
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), buffer, len, NULL, NULL);
	buffer[len] = '\0';
	//ɾ��������������ֵ
	result.append(buffer);
	delete[] buffer;
	return result;
}