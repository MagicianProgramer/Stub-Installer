#pragma once
class CMyFile
{
public:
	CMyFile();
	~CMyFile();
public:
	BOOL Open(wchar_t* wszfile, wchar_t* mode);
	void Close();
	UINT64 GetFileLength();
	int Read(void* buff, int nLen);
	int Write(void* buff, int nLen);
	void Seek(UINT64 nOffset, int mode);
private:
	FILE* m_fp;
};

