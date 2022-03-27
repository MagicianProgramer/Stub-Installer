#pragma once
#define MAX_KEYARRAY	30


class TRegistry {
protected:
	HKEY	topKey;
	int		openCnt;
	HKEY	hKey[MAX_KEYARRAY];

public:
	TRegistry(LPCTSTR  company, LPCTSTR  appName=NULL);
	TRegistry(HKEY top_key);
	~TRegistry();

	HKEY	GetKey();
	BOOL	CloseKey(void);
	
	BOOL	ChangeApp(LPCTSTR company, LPCTSTR appName=NULL);

	void	ChangeTopKey(HKEY topKey);

	BOOL	OpenKey(LPCTSTR subKey, BOOL createFlg=FALSE);

	BOOL	CreateKey(LPCTSTR subKey) { return OpenKey(subKey, TRUE); }

	BOOL	GetInt(LPCTSTR key, int &val, int defVal);
	int		GetInt(LPCTSTR key, int defVal);

	BOOL	SetInt(LPCTSTR key, int val);

	BOOL	GetLong(LPCTSTR key, long &val, long defVal);
	long	GetLong(LPCTSTR key, long defVal);

	BOOL	SetLong(LPCTSTR key, long val);

	BOOL	GetStr(LPCTSTR key, CString &str, LPCTSTR defVal);
	CString GetStr(LPCTSTR key, LPCTSTR defVal);

	BOOL	SetStr(LPCTSTR key, LPCTSTR str);

	BOOL	SetKeyStr(LPCTSTR key, LPCTSTR str);

	BOOL	GetByte(LPCTSTR key, BYTE *data, int *size);

	BOOL	SetByte(LPCTSTR key, const BYTE *data, int size);

	BOOL	DeleteKey(LPCTSTR str);

	BOOL	DeleteValue(LPCTSTR str);

	BOOL	EnumKey(DWORD cnt, wchar_t *buf, int size);

	BOOL	EnumValue(DWORD cnt, wchar_t *buf, int size, DWORD *type=NULL);

	BOOL	DeleteChildTree(LPCTSTR subkey=NULL);
};