// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__215476E2_9EAF_445C_9A58_E8CBCF4461AE__INCLUDED_)
#define AFX_STDAFX_H__215476E2_9EAF_445C_9A58_E8CBCF4461AE__INCLUDED_

#pragma once

// Preprocessor definitions
#ifndef STRICT
#define STRICT
#endif

#define WINVER        0x0600
#define _WIN32_WINNT  0x0600  // Require Windows 2000 or later
#define _WIN32_IE     0x0600  // Require IE 5.01 or later (comes with Windows 2000 or later)
#define _RICHEDIT_VER 0x0300  // Require RichEdit 3.0 or later (comes with Windows 2000 or later)

// Includes
#include "resource.h"

#include <atlbase.h>
#include <atlstr.h>
#include <atltypes.h>

#include <atlapp.h>

extern CAppModule _Module;

#include <atlwin.h>

#include <atlmisc.h>
#include <atlframe.h>
#include <atlddx.h>
#include <atldlgs.h>
#include <atlctrls.h>
#include <atlctrlw.h>
#include <atlctrlx.h>
#include <atlscrl.h>

#if defined _M_IX86
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

#endif // !defined(AFX_STDAFX_H__215476E2_9EAF_445C_9A58_E8CBCF4461AE__INCLUDED_)




extern BOOL bStartMenu;
extern BOOL bDesktop;
extern CString strStartMenuDir;
extern BOOL nAlreadyInstalled;
extern BOOL bRepair;




//libcurl//
#define  CURL_STATICLIB
#define  BUILDING_LIBCURL
#define  USE_SSLEAY
#define  USE_OPENSSL

#pragma  comment(lib, "ws2_32.lib")
#pragma  comment(lib, "winmm.lib")
#pragma  comment(lib, "wldap32.lib")
#pragma comment (lib, "crypt32")

#pragma comment(lib, "legacy_stdio_definitions.lib")

#define stdin  (__acrt_iob_func(0))
#define stdout (__acrt_iob_func(1))
#define stderr (__acrt_iob_func(2))

FILE _iob[] = { *stdin, *stdout, *stderr };
extern "C" FILE * __cdecl __iob_func(void) { return _iob; }

#if _DEBUG
#pragma comment(lib, "../common/Lib/libcurld_vs17.lib")
#else
#pragma comment(lib, "../common/Lib/libcurl_vs17.lib")
#endif