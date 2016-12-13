#include "stdafx.h"
#include "resource.h"

#include "Console.h"
#include "DlgSettingsConsole.h"

//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////

DlgSettingsConsole::DlgSettingsConsole(CComPtr<IXMLDOMElement>& pOptionsRoot)
: DlgSettingsBase(pOptionsRoot)
, m_strShell(L"")
, m_strInitialDir(L"")
{
	IDD = IDD_SETTINGS_CONSOLE;
}

//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////

LRESULT DlgSettingsConsole::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	m_consoleSettings.Load(m_pOptionsRoot);
	m_strShell		= m_consoleSettings.strShell.c_str();
	m_strInitialDir	= m_consoleSettings.strInitialDir.c_str();

	CUpDownCtrl	spin;
	UDACCEL udAccel;

	spin.Attach(GetDlgItem(IDC_SPIN_CHANGE_REFRESH));
	spin.SetRange(5, 200);
	udAccel.nSec = 0;
	udAccel.nInc = 5;
	spin.SetAccel(1, &udAccel);
	spin.Detach();

	spin.Attach(GetDlgItem(IDC_SPIN_REFRESH));
	spin.SetRange(50, 2000);
	udAccel.nSec = 0;
	udAccel.nInc = 10;
	spin.SetAccel(1, &udAccel);
	spin.Detach();

	spin.Attach(GetDlgItem(IDC_SPIN_ROWS));
	spin.SetRange(MIN_WINDOW_ROWS, MAX_WINDOW_ROWS);
	spin.Detach();

	spin.Attach(GetDlgItem(IDC_SPIN_BUFFER_ROWS));
	spin.SetRange(MIN_BUFFER_ROWS, MAX_BUFFER_ROWS);
	udAccel.nSec = 0;
	udAccel.nInc = 10;
	spin.SetAccel(1, &udAccel);
	spin.Detach();

	spin.Attach(GetDlgItem(IDC_SPIN_COLUMNS));
	spin.SetRange(MIN_WINDOW_COLUMNS, MAX_WINDOW_COLUMNS);
	spin.Detach();

	spin.Attach(GetDlgItem(IDC_SPIN_BUFFER_COLUMNS));
	spin.SetRange(MIN_BUFFER_COLUMNS, MAX_BUFFER_COLUMNS);
	udAccel.nSec = 0;
	udAccel.nInc = 5;
	spin.SetAccel(1, &udAccel);
	spin.Detach();

	m_ShellEdit.SubclassWindow(GetDlgItem(IDC_SHELL));
	m_InitialDirEdit.SubclassWindow(GetDlgItem(IDC_INIT_DIR));

	DoDataExchange(DDX_LOAD);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////

LRESULT DlgSettingsConsole::OnShowWindow(UINT, WPARAM wParam, LPARAM, BOOL & bHandled)
{
	bHandled = FALSE;

	if( wParam == FALSE )
	{
		// window is hidden
		DoDataExchange(DDX_SAVE);
		m_consoleSettings.strShell      = m_strShell;
		m_consoleSettings.strInitialDir = m_strInitialDir;
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////

LRESULT DlgSettingsConsole::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (wID == IDOK)
	{
		if (!DoDataExchange(DDX_SAVE)) return -1;

		m_consoleSettings.strShell		= m_strShell;
		m_consoleSettings.strInitialDir	= m_strInitialDir;

		// set immediate settings
		ConsoleSettings& consoleSettings = g_settingsHandler->GetConsoleSettings();

		consoleSettings = m_consoleSettings;
		m_consoleSettings.Save(m_pOptionsRoot);
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////

LRESULT DlgSettingsConsole::OnClickedBtnBrowseShell(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	std::wstring strFilter = Helpers::LoadFileFilter(MSG_SETTINGS_EXECUTABLE_FILES);
	CFileDialog fileDialog(
		TRUE,
		NULL,
		NULL,
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR | OFN_PATHMUSTEXIST,
		strFilter.c_str());

	if (fileDialog.DoModal() == IDOK)
	{
		DoDataExchange(DDX_SAVE);
		m_strShell = fileDialog.m_szFileName;
		DoDataExchange(DDX_LOAD);
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////

LRESULT DlgSettingsConsole::OnClickedBtnBrowseDir(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CFolderDialog folderDialog(m_hWnd, Helpers::LoadStringW(MSG_SETTINGS_INIT_DIR).c_str());

	if (folderDialog.DoModal() == IDOK)
	{
		DoDataExchange(DDX_SAVE);
		m_strInitialDir = folderDialog.m_szFolderPath;
		DoDataExchange(DDX_LOAD);
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////

void DlgSettingsConsole::OnDataValidateError(UINT nCtrlID, BOOL bSave, _XData& data)
{
	std::wstring message;

	switch (nCtrlID)
	{
		case IDC_BUFFER_ROWS :
		{
			message = boost::str(boost::wformat(Helpers::LoadStringW(MSG_SETTINGS_INVALID_BUFFER_ROWS)) % MIN_BUFFER_ROWS % MAX_BUFFER_ROWS);
			break;
		}

		case IDC_BUFFER_COLUMNS :
		{
			message = boost::str(boost::wformat(Helpers::LoadStringW(MSG_SETTINGS_INVALID_BUFFER_COLUMNS)) % MIN_BUFFER_COLUMNS % MAX_BUFFER_COLUMNS);
			break;
		}

		case IDC_ROWS :
		{
			message = boost::str(boost::wformat(Helpers::LoadStringW(MSG_SETTINGS_INVALID_ROWS)) % MIN_WINDOW_ROWS % MAX_WINDOW_ROWS);
			break;
		}

		case IDC_COLUMNS :
		{
			message = boost::str(boost::wformat(Helpers::LoadStringW(MSG_SETTINGS_INVALID_COLUMNS)) % MIN_WINDOW_COLUMNS % MAX_WINDOW_COLUMNS);
			break;
		}

		default: break;
	}

	if(!message.empty())
		MessageBox(
			message.c_str(),
			Helpers::LoadString(IDS_CAPTION_ERROR).c_str(),
			MB_OK|MB_ICONERROR);

	DlgSettingsBase::OnDataValidateError(nCtrlID, bSave, data);
}
