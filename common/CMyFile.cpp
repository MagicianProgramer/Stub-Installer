#include "stdafx.h"
#include "CMyFile.h"


CMyFile::CMyFile()
{
}


CMyFile::~CMyFile()
{
}

BOOL CMyFile::Open(wchar_t* wszfile, wchar_t* mode)
{
	BOOL bRet = FALSE;

	m_fp = _wfopen(wszfile, mode);
	if (m_fp)	bRet = TRUE;

	return bRet;
}

void CMyFile::Close()
{
	if (m_fp)		fclose(m_fp);
}

UINT64 CMyFile::GetFileLength()
{
	UINT64 nRet = 0;
	if (m_fp)
	{
		fseek(m_fp, 0, SEEK_SET);
		fseek(m_fp, 0, SEEK_END);
		nRet = ftell(m_fp);
		fseek(m_fp, 0, SEEK_SET);
	}

	return nRet;
}

int CMyFile::Read(void* buff, int nLen)
{
	UINT64 nRet = 0;

	if (m_fp)
	{
		nRet = fread(buff, 1, nLen, m_fp);
	}

	return nRet;
}

int CMyFile::Write(void* buff, int nLen)
{
	UINT64 nRet = 0;

	if (m_fp)
	{
		nRet = fwrite(buff, 1, nLen, m_fp);
	}

	return nRet;
}

void CMyFile::Seek(UINT64 nOffset, int mode)
{
	if (m_fp)
	{
		fseek(m_fp, nOffset, mode);
	}
}