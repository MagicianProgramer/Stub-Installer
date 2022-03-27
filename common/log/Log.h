#ifndef _ATA_LOGFILE_
#define _ATA_LOGFILE_

/////////////////////////////////////////////////////////////////
//features:	not depends on MFC ore ATL.
//			file name could use absolute path or just the name, in which case the 
//			file will be created at the same place with the binary module, no concern 
//			with curret directory, which always bring me truble.
//			every log line has a time stamp attached, with million seconds.
//			uses printf like format to write log lines
//			uses a preprocessor definition _DEBUG_LOG to switch if write log file
//			multi thread safe, finally added:)

////////////////////////////////////////////////////
//	CLog, a debug log file wrapper
class CLog
{
public:
	//////////////////////////////
	//	Constructor, open the logfile
	CLog(LPCTSTR strFile, bool bAppend = FALSE, long lTruncate = 4096);
	/////////////////////////////////
	//	Destructor, close if logfile if opened
	~CLog();
	/////////////////////////////////
	void SetEnabled(BOOL bWrite);
	//	Write log info into the logfile, with printf like parameters support
	void Write(LPCTSTR pszFormat, ...);

	void SetUnsinstLst(BOOL bSet);
private:
	FILE*	m_pLogFile;
	long	m_lTruncate;
	BOOL	m_bWrite;
	CRITICAL_SECTION	m_cs;

	BOOL m_bUninstLst;
};

#endif //_ATA_LOGFILE_
