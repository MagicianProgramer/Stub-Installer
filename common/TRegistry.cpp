#include "stdafx.h"
#include "TRegistry.h"
#include <strsafe.h>

TRegistry::TRegistry(LPCTSTR  company, LPCTSTR  appName)
{
	openCnt = 0;
	ChangeApp(company, appName);
}

TRegistry::TRegistry(HKEY top_key)
{
	topKey = top_key;
	openCnt = 0;
}

TRegistry::~TRegistry(void)
{
	while (openCnt > 0) {
		CloseKey();
	}
}

HKEY TRegistry::GetKey()
{
	return hKey[openCnt -1];
}

BOOL TRegistry::CloseKey(void)
{
	if (openCnt <= 0) {
		return	FALSE;
	}
	::RegCloseKey(hKey[--openCnt]);

	return	TRUE;
}

BOOL TRegistry::ChangeApp(LPCTSTR company, LPCTSTR appName)
{
	while (openCnt > 0) {
		CloseKey();
	}

	topKey = HKEY_CURRENT_USER;

	WCHAR	wbuf[100];
	StringCchPrintf(wbuf, 100, L"software\\%s", company);

	if (appName)// && GetChar(appName, 0)) 
	{
		StringCchPrintf(wbuf + wcslen(wbuf), 100-wcslen(wbuf), L"\\%s", appName);
	}

	return	CreateKey(wbuf);
}

void TRegistry::ChangeTopKey(HKEY top_key)
{
	while (openCnt > 0)
		CloseKey();

	topKey = top_key;
}

BOOL TRegistry::OpenKey(LPCTSTR subKey, BOOL createFlg)
{
	HKEY	parentKey = (openCnt == 0 ? topKey : hKey[openCnt -1]);

	if (openCnt >= MAX_KEYARRAY) {
		return	FALSE;
	}
	DWORD	tmp;
	LONG	status;

	if (createFlg) {
		status = ::RegCreateKeyEx(parentKey, subKey, 0, NULL, REG_OPTION_NON_VOLATILE,
			KEY_ALL_ACCESS, NULL, &hKey[openCnt], &tmp);
	}
	else {
		if ((status = ::RegOpenKeyEx(parentKey, subKey, 0, KEY_ALL_ACCESS, &hKey[openCnt]))
			!= ERROR_SUCCESS)
			status = ::RegOpenKeyEx(parentKey, subKey, 0, KEY_READ, &hKey[openCnt]);
	}

	if (status == ERROR_SUCCESS)
		return	openCnt++, TRUE;
	else
		return	FALSE;
}


BOOL TRegistry::GetInt(LPCTSTR subKey, int &val, int defVal)
{
	long _val = 0;
	BOOL ret = GetLong(subKey, _val, defVal);
	return (int)_val;
}

int TRegistry::GetInt(LPCTSTR subKey, int defVal)
{
	return (int)GetLong(subKey, defVal);
}

BOOL TRegistry::SetInt(LPCTSTR subKey, int val)
{
	return	SetLong(subKey, (long)val);
}


BOOL TRegistry::GetLong(LPCTSTR subKey, long &val, long defVal)
{
	DWORD	type = REG_DWORD, dw_size = sizeof(long);
	val = defVal;

	if (::RegQueryValueEx(hKey[openCnt -1], subKey, 0, &type, (BYTE *)&val, &dw_size)
		== ERROR_SUCCESS) {			
			return	TRUE;
	}
	// 昔の互換性用
	WCHAR	wbuf[100];
	long	size_byte = sizeof(wbuf);

	if (::RegQueryValue(hKey[openCnt -1], subKey, wbuf, &size_byte) != ERROR_SUCCESS)
		return	FALSE;
	val = wcstol(wbuf, 0, 10);
	return	TRUE;
}

long TRegistry::GetLong(LPCTSTR subKey, long defVal)
{
	long val = 0;
	BOOL ret = GetLong(subKey, val, defVal);

	return val;
}

BOOL TRegistry::SetLong(LPCTSTR subKey, long val)
{
	return	::RegSetValueEx(hKey[openCnt -1], subKey, 0, REG_DWORD,
		(const BYTE *)&val, sizeof(val)) == ERROR_SUCCESS;
}

BOOL TRegistry::GetStr(LPCTSTR subKey, CString &str, LPCTSTR defVal)
{
	DWORD	type = REG_SZ;
	if (!defVal)
	{
		str = L"";
	}
	else
	{
		str = defVal;
	}
	
	int size_byte = 0;
	if (::RegQueryValueEx(hKey[openCnt -1], subKey, 0, &type, NULL,
		(DWORD *)&size_byte) == ERROR_SUCCESS ||
		::RegQueryValue(hKey[openCnt -1], subKey, NULL, (LPLONG)&size_byte) == ERROR_SUCCESS)
	{
		TCHAR* pstr = new TCHAR[size_byte];
		if (::RegQueryValueEx(hKey[openCnt -1], subKey, 0, &type, (BYTE *)pstr,
			(DWORD *)&size_byte) != ERROR_SUCCESS
			&&  ::RegQueryValue(hKey[openCnt -1], subKey, pstr, (LPLONG)&size_byte) != ERROR_SUCCESS)
		{
			delete[] pstr; pstr = NULL;
			return FALSE;
		}
		else
		{
			str = CString(pstr);

			delete[] pstr; pstr = NULL;
			return TRUE;
		}		
	}
	else
	{
		return FALSE;
	}
}

CString TRegistry::GetStr(LPCTSTR subKey, LPCTSTR defVal)
{
	CString str = L"";
	BOOL ret = GetStr(subKey, str, defVal);
	return str;
}

BOOL TRegistry::SetStr(LPCTSTR subKey, LPCTSTR str)
{
	return	::RegSetValueEx(hKey[openCnt -1], subKey, 0, REG_SZ, (const BYTE *)str,
		(wcslen(str) +1) * 2) == ERROR_SUCCESS;
}

BOOL TRegistry::SetKeyStr(LPCTSTR subkey, LPCTSTR str)
{
	//return ::RegSetKeyValue(hKey[openCnt -1], subkey, 0, REG_SZ, (const BYTE *)str, (wcslen(str) +1) * 2) == ERROR_SUCCESS;
	return TRUE;
}

BOOL TRegistry::GetByte(LPCTSTR subKey, BYTE *data, int *size)
{
	DWORD	type = REG_BINARY;

	return	::RegQueryValueEx(hKey[openCnt -1], subKey, 0, &type,
		(BYTE *)data, (LPDWORD)size) == ERROR_SUCCESS;
}

BOOL TRegistry::SetByte(LPCTSTR subKey, const BYTE *data, int size)
{
	return	::RegSetValueEx(hKey[openCnt -1], subKey, 0, REG_BINARY, data, size)
		== ERROR_SUCCESS;
}

BOOL TRegistry::DeleteKey(LPCTSTR subKey)
{
	return	::RegDeleteKey(hKey[openCnt -1], subKey) == ERROR_SUCCESS;
}

BOOL TRegistry::DeleteValue(LPCTSTR subValue)
{
	return	::RegDeleteValue(hKey[openCnt -1], subValue) == ERROR_SUCCESS;
}

BOOL TRegistry::EnumKey(DWORD cnt, wchar_t *buf, int size)
{
	return	::RegEnumKeyEx(hKey[openCnt -1], cnt, buf, (DWORD *)&size, 0, 0, 0, 0)
			== ERROR_SUCCESS;
}

BOOL TRegistry::EnumValue(DWORD cnt, wchar_t *buf, int size, DWORD *type)
{
	return	::RegEnumValue(hKey[openCnt -1], cnt, buf, (DWORD *)&size, 0, type, 0, 0)
			== ERROR_SUCCESS;
}

/*
	subKey を指定した場合は subkey を含むキー以下を削除
	subkey が NULL の場合、カレント の配下を削除
*/
BOOL TRegistry::DeleteChildTree(LPCTSTR subKey)
{
	WCHAR	wbuf[256];
	BOOL	ret = TRUE;

	if (subKey && !OpenKey(subKey)) {
		return	FALSE;
	}

	while (EnumKey(0, wbuf, sizeof(wbuf) / 2))
	{
		if (!(ret = DeleteChildTree(wbuf)))
			break;
	}
	if (subKey)
	{
		CloseKey();
		ret = DeleteKey(subKey) ? ret : FALSE;
	}
	else {
		while (EnumValue(0, wbuf, sizeof(wbuf) / 2))
		{
			if (!DeleteValue(wbuf))
			{
				ret = FALSE;
				break;
			}
		}
	}
	return	ret;
}