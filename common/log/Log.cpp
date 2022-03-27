#include "stdafx.h"
#include "log.h"

#pragma warning(disable:4996)

CLog::CLog(LPCTSTR strFile, bool bAppend, long lTruncate)
{
	m_bWrite = TRUE;
	m_lTruncate = lTruncate;
	TCHAR	szFile[MAX_PATH+1];
	if (_tcslen(strFile)>3 && strFile[1]!=':')	//no absolute path designated
	{
		::GetModuleFileName(NULL, szFile, MAX_PATH);
		long llength = _tcslen(szFile);
		TCHAR*	pcat = szFile+(llength - 1);	//point to the last char
		while (llength--)
		{
			pcat--;
			if (*pcat == '\\')
				break;
		}
		if (*pcat == '\\')
		{
			pcat++;
			_tcscpy(pcat, strFile);
		}
		else	//something wrong, use the original filename, ignore path problem
			_tcscpy(szFile, strFile);
	}
	else
		_tcscpy_s(szFile, strFile);


	m_pLogFile = _tfopen(szFile, bAppend ? _T("a") : _T("w"));
	InitializeCriticalSection(&m_cs);

	m_bUninstLst = FALSE;
}

void CLog::SetUnsinstLst(BOOL bSet)
{
	m_bUninstLst = bSet;
}

/////////////////////////////////
//	Destructor, close if logfile if opened
CLog::~CLog()
{
	if (m_pLogFile)
	{
		if (!m_bUninstLst)
		{
			fputs("===============Finish Loging================\r\n", m_pLogFile);
		}		
		fclose(m_pLogFile);
	}
	DeleteCriticalSection(&m_cs);
}

void CLog::SetEnabled(BOOL bWrite)
{
	m_bWrite = bWrite;
}

/////////////////////////////////
//	Write log info into the logfile, with printf like parameters support
void CLog::Write(LPCTSTR pszFormat, ...)
{
	if (!m_bWrite) 
		return;

	if (!m_pLogFile)
		return;

	EnterCriticalSection(&m_cs);
	//write the formated log string to szLog
	TCHAR	szLog[256];
	va_list argList;
	va_start( argList, pszFormat );
	_vstprintf( szLog, pszFormat, argList );
	va_end( argList );

	//Trancate if the file grow too large
	long	lLength = ftell(m_pLogFile);
	if (lLength > m_lTruncate)
		rewind(m_pLogFile);

	//Get current time
	SYSTEMTIME	time;
	::GetLocalTime(&time);
	TCHAR	szLine[256];

	if (!m_bUninstLst)
	{
		_stprintf(szLine, _T("%04d-%02d-%02d %02d:%02d:%02d.%03d %s\n"),
			time.wYear, time.wMonth, time.wDay,
			time.wHour, time.wMinute, time.wSecond, time.wMilliseconds,
			szLog);
	}
	else
	{
		_stprintf(szLine, _T("%s\n"), szLog);
	}

	OutputDebugString(szLog);
	
	fflush(m_pLogFile);
	_fputts(szLine, m_pLogFile);
	

	LeaveCriticalSection(&m_cs);
}
