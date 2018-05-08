#pragma once 
#include <string>
using namespace std;

class CConvert
{
public:  
	CConvert();  
	virtual ~CConvert();  
public:
	static LPSTR WideChar2MBCS( const CString& strCS );
	static std::string WideChar2StdStr(const CString& strcs);
	static LPOLESTR CConvert::MBCS2WideChar( LPCSTR lpa );
	static CString CConvert::MBCS2CString( LPCSTR lpa );
	static CString CConvert::StdStr2CSting( const std::string& stdStr );
	static wstring CConvert::string2wstring(string str);
	static string CConvert::wstring2string(wstring wstr);
};