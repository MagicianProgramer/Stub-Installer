// CWizardMainPage.cpp : implementation file
//

#include "stdafx.h"
#include "Uninstaller.h"
#include "CWizardUninstProgPage.h"

#include <string>
#include <vector>
#include <sstream> //istringstream
#include <iostream> // cout
#include <fstream> // ifstream
#include <vector>
using namespace std;

#include "uninst.h"

#define  TIMER_SILENTMODE	10
#define  UNINSTLST		L"uninst.lst"

// CWizardMainPage

IMPLEMENT_DYNAMIC(CWizardUninstProgPage, CPropertyPage)

CWizardUninstProgPage::CWizardUninstProgPage()
	: CPropertyPage(CWizardUninstProgPage::IDD)
{

}

CWizardUninstProgPage::~CWizardUninstProgPage()
{
}

void CWizardUninstProgPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_UNINST, m_ctrlProgUninstall);
}

BEGIN_MESSAGE_MAP(CWizardUninstProgPage, CPropertyPage)
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CInstallerWizard message handlers


BOOL CWizardUninstProgPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(hIcon, TRUE);			// Set big icon
	SetIcon(hIcon, FALSE);		// Set small icon

	// TODO:  Add extra initialization here

/*#ifndef TEST
	if (g_UninstOption.chkOptionsSilentMode)
		SetTimer(TIMER_SILENTMODE, 50, NULL);
#endif // !TEST*/

	//set progress//
	m_ctrlProgUninstall.SetRange(0, 100);
	m_ctrlProgUninstall.SetPos(0);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


LRESULT CWizardUninstProgPage::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	

	return CPropertyPage::WindowProc(message, wParam, lParam);
}




void CWizardUninstProgPage::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == TIMER_SILENTMODE)
	{
		KillTimer(TIMER_SILENTMODE);
		ShowWindow(SW_HIDE);
		UninstallProc();
	}

	CPropertyPage::OnTimer(nIDEvent);
}

int nTotalcnt = 0;
int nidx = 0;
void CWizardUninstProgPage::DeleteFolder(CString szDir, BOOL bDeleteSelf, int *pCnt)
{
	szDir += "\\";
	WIN32_FIND_DATA ds;
	CString szAim = szDir;
	szAim += "*.*";
	HANDLE hDir = FindFirstFile(szAim, &ds);
	if (hDir == INVALID_HANDLE_VALUE)
	{
		FindClose(hDir);		
		return;
	}

	CString strPath;
	do
	{
		if (ds.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (((CString)ds.cFileName != (CString)L".") && ((CString)ds.cFileName != (CString)L".."))
			{

				CString szSubDir = szDir;
				szSubDir += ds.cFileName;
				if (pCnt)
					(*pCnt)++;
				else
					DeleteFolder(szSubDir, TRUE, pCnt);
			}
		}
		else
		{
			strPath = szDir + ds.cFileName;

			if (pCnt)
				(*pCnt)++;
			else
			{
				DeleteFile(strPath);
				m_strMsg.AppendFormat(L"deleted file : %s.\r\n", strPath);
				SetDlgItemText(IDC_EDIT_UNINST_DETAIL, m_strMsg);
				m_ctrlProgUninstall.SetPos(nidx * 100 / nTotalcnt);
				nidx++;
			}
		}

	} while (FindNextFile(hDir, &ds) == TRUE);


	FindClose(hDir);


	if (bDeleteSelf == TRUE)
	{
		if (pCnt)
			(*pCnt)++;
		else
		{
			RemoveDirectory(szDir);
			nidx++;
		}		
	}

	return;
}

void CWizardUninstProgPage::CompleteUninstall()
{
	/////////////////////////////////
	if (g_strDeleteDir.IsEmpty() || !PathFileExists(g_strDeleteDir))
		return;

	//get cnt//
	DeleteFolder(g_strDeleteDir, TRUE, &nTotalcnt);

	//real remove//
	DeleteFolder(g_strDeleteDir, TRUE, NULL);

	m_ctrlProgUninstall.SetPos(100);
}


void CWizardUninstProgPage::LstUninstall()
{
	CString strUninstLst = g_strDeleteDir + L"\\" + UNINSTLST;

	vector<CString> vecDelLst;

	CStringA strFileA(strUninstLst);
	ifstream inputFile(strFileA.GetBuffer());
	int l = 0;

	while (inputFile) {
		l++;
		string s;
		if (!getline(inputFile, s)) break;

		CStringA strFullPathA;
		strFullPathA.Format("%s", s.c_str());

		CString str(strFullPathA);
		vecDelLst.push_back(str);
	}

	if (!inputFile.eof()) {
	}

	for (INT i = 0; i < vecDelLst.size(); i++)
	{
		//AfxMessageBox(vecDelLst[i]);
		if (PathFileExists(vecDelLst[i]))	DeleteFile(vecDelLst[i]);
		m_strMsg.AppendFormat(L"deleted file : %s.\r\n", vecDelLst[i]);
		SetDlgItemText(IDC_EDIT_UNINST_DETAIL, m_strMsg);
		m_ctrlProgUninstall.SetPos((int)100 * (i + 1) / vecDelLst.size());
	}
}


void CWizardUninstProgPage::UninstallProc()
{
	// TODO: Add your control notification handler code here
	if (g_UninstOption.chkOptionsSilentMode)
	{
		Sleep(10);
		AfxGetMainWnd()->ShowWindow(SW_HIDE);
	}

	m_strMsg = L"";

	if (g_UninstOption.chkCloseMainExecutable)
	{
		SetDlgItemText(IDC_STATIC_UNINST_STS, L"closing main executable.");
		CloseMainExecutable();
		m_strMsg.AppendFormat(L"closed main executable (%s).\r\n", g_UninstOption.wszMainExecutableName);
		SetDlgItemText(IDC_EDIT_UNINST_DETAIL, m_strMsg);
	}

	if (g_UninstOption.chkCloseRunningApp)
	{
		SetDlgItemText(IDC_STATIC_UNINST_STS, L"closing running application.");
		DWORD dwPid;
		int nCnt = ProcessFind(g_UninstOption.wszRunningAppFile, dwPid);
		if (nCnt > 0)
		{
			HANDLE	hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
			BOOL bRet = TerminateProcess(hProc, 0);
		}
		m_strMsg.AppendFormat(L"closed running application executable (%s).\r\n", g_UninstOption.wszRunningAppFile);
		SetDlgItemText(IDC_EDIT_UNINST_DETAIL, m_strMsg);
	}

	//////////Delete all files///////////
	SetDlgItemText(IDC_STATIC_UNINST_STS, L"deleting files.");
	if (g_UninstOption.chkOptionsCompleteUninstalltion)
		CompleteUninstall();
	else
		LstUninstall();

	/////////Delete All Shortcuts//////////
	SetDlgItemText(IDC_STATIC_UNINST_STS, L"deleting shortcuts.");
	DeleteShortcut();
	m_strMsg.AppendFormat(L"deleted shortcuts.\r\n");
	SetDlgItemText(IDC_EDIT_UNINST_DETAIL, m_strMsg);

	////////Unregist///////////////
	SetDlgItemText(IDC_STATIC_UNINST_STS, L"unregitering.");
	
	UnregUninstaller();	
	DelStartup();
	DeleteInfoFromRegistry();

	m_strMsg.AppendFormat(L"unregistered.\r\n");
	SetDlgItemText(IDC_EDIT_UNINST_DETAIL, m_strMsg);

	SetDlgItemText(IDC_STATIC_UNINST_STS, L"completed.");
	m_strMsg.AppendFormat(L"completed.\r\n");
	SetDlgItemText(IDC_EDIT_UNINST_DETAIL, m_strMsg);

	if (g_UninstOption.chkOptionsSilentMode)
	{
		GetParent()->GetParent()->PostMessageW(UM_NEXT_PAGE);
		Sleep(10);
		GetParent()->GetParent()->PostMessageW(UM_NEXT_PAGE);
	}
}

BOOL CWizardUninstProgPage::OnSetActive()
{
	// TODO: Add your specialized code here and/or call the base class
	UninstallProc();
		
	return CPropertyPage::OnSetActive();
}
