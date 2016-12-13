#include "stdafx.h"
#include "resource.h"

#include "PageSettingsTabs1.h"

//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////

PageSettingsTabs1::PageSettingsTabs1()
: m_tabData()
, m_strTitle(L"")
, m_strIcon(L"")
, m_bUseDefaultIcon(false)
, m_strShell(L"")
, m_strInitialDir(L"")
, m_nRunAs(0)
, m_strUser(L"")
, m_bNetOnly(false)
, m_tabShellEdit(this)
{
}

//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////

LRESULT PageSettingsTabs1::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	if (CTheme().IsThemingSupported()) ::EnableThemeDialogTexture(m_hWnd, ETDT_USETABTEXTURE);

	m_tabIconEdit.SubclassWindow(GetDlgItem(IDC_TAB_ICON));
	m_tabShellEdit.SubclassWindow(GetDlgItem(IDC_TAB_SHELL));
	m_tabInitialDirEdit.SubclassWindow(GetDlgItem(IDC_TAB_INIT_DIR));

	m_comboPriority.Attach(GetDlgItem(IDC_COMBO_BASE_PRIORITY));
	Helpers::LoadCombo(m_comboPriority, IDC_COMBO_BASE_PRIORITY);

	DoDataExchange(DDX_LOAD);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////

LRESULT PageSettingsTabs1::OnTabTitleChange(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CWindow(GetDlgItem(IDC_TAB_TITLE)).GetWindowText(m_strTitle);
	GetParent().PostMessage(UM_TAB_TITLE_CHANGED, 0, 0);
	return 0;
}

LRESULT PageSettingsTabs1::OnTabIconChange(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CWindow(GetDlgItem(IDC_TAB_ICON)).GetWindowText(m_strIcon);
	GetParent().PostMessage(UM_TAB_ICON_CHANGED, 0, 0);
	return 0;
}

LRESULT PageSettingsTabs1::OnTabShellChange(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if( m_bUseDefaultIcon )
	{
		CWindow(GetDlgItem(IDC_TAB_SHELL)).GetWindowText(m_strShell);
		GetParent().PostMessage(UM_TAB_ICON_CHANGED, 0, 0);
	}
	return 0;
}


//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////

LRESULT PageSettingsTabs1::OnBtnBrowseIcon(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	DoDataExchange(DDX_SAVE);

	int index = 0;
	WCHAR path[MAX_PATH];
	::GetModuleFileName(NULL, path, ARRAYSIZE(path));

	if( PickIconDlg(NULL, path, ARRAYSIZE(path), &index) == 1 )
	{
		if( index != 0 )
			m_strIcon.Format(L"%s,%d", path, index);
		else
			m_strIcon = path;
		DoDataExchange(DDX_LOAD);
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////

LRESULT PageSettingsTabs1::OnClickedBtnBrowseShell(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	std::wstring strFilter = Helpers::LoadFileFilter(MSG_SETTINGS_EXECUTABLE_FILES_AND_LINKS);
	CFileDialog fileDialog(
		TRUE,
		NULL,
		NULL,
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR | OFN_PATHMUSTEXIST | OFN_NODEREFERENCELINKS,
		strFilter.c_str());

	if (fileDialog.DoModal() == IDOK)
	{
		CString strShell(fileDialog.m_szFileName);

		this->ConvertShellLink(strShell);
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////

LRESULT PageSettingsTabs1::OnClickedBtnBrowseDir(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
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

LRESULT PageSettingsTabs1::OnCheckboxClicked(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	DoDataExchange(DDX_SAVE);
	EnableControls();

	GetParent().PostMessage(UM_TAB_ICON_CHANGED, 0, 0);

	return 0;
}

//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////

LRESULT PageSettingsTabs1::OnTabKillFocus(WORD /*wCommandCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if( wID == IDC_TAB_SHELL )
	{
		CString strShell(L"");
		this->ConvertShellLink(strShell);
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////

void PageSettingsTabs1::EnableControls()
{
	GetDlgItem(IDC_TAB_ICON).EnableWindow(m_bUseDefaultIcon == false);
	GetDlgItem(IDC_BTN_BROWSE_ICON).EnableWindow(m_bUseDefaultIcon == false);

	GetDlgItem(IDC_TAB_USER).EnableWindow(m_nRunAs == 2);
	GetDlgItem(IDC_CHECK_NET_ONLY).EnableWindow(m_nRunAs == 2);
}

//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////

void PageSettingsTabs1::Load(std::shared_ptr<TabData>& tabData)
{
	m_tabData         = tabData;

	m_strTitle        = m_tabData->strTitle.c_str();
	m_strIcon         = m_tabData->strIcon.c_str();
	m_bUseDefaultIcon = m_tabData->bUseDefaultIcon;

	m_strShell        = m_tabData->strShell.c_str();
	m_strInitialDir   = m_tabData->strInitialDir.c_str();
	m_nRunAs          = m_tabData->bRunAsUser? 2 : m_tabData->bRunAsAdministrator? 1 : 0;
	m_strUser         = m_tabData->strUser.c_str();
	m_bNetOnly        = m_tabData->bNetOnly;

	m_comboPriority.SetCurSel(m_tabData->dwBasePriority);

	DoDataExchange(DDX_LOAD);

	EnableControls();
}

//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////

void PageSettingsTabs1::Save()
{
	DoDataExchange(DDX_SAVE);

	m_tabData->strTitle            = m_strTitle;
	m_tabData->strIcon             = m_strIcon;
	m_tabData->bUseDefaultIcon     = m_bUseDefaultIcon;

	m_tabData->strShell            = m_strShell;
	m_tabData->strInitialDir       = m_strInitialDir;
	m_tabData->bRunAsUser          = m_nRunAs == 2;
	m_tabData->strUser             = m_strUser;
	m_tabData->bNetOnly            = m_bNetOnly;
	m_tabData->bRunAsAdministrator = m_nRunAs == 1;

	m_tabData->dwBasePriority      = m_comboPriority.GetCurSel();
}

//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////

void PageSettingsTabs1::ConvertShellLink(CString& strShell)
{
	DoDataExchange(DDX_SAVE);

	CString strLink;

	if( strShell.IsEmpty() )
	{
		// text modified in shell text box
		// this text can contain spaces or quotes
		strLink = m_strShell.Trim();
		while( strLink.GetLength() > 0 && strLink.GetAt(0) == L'"' )
			strLink = strLink.Right(strLink.GetLength() - 1);
		while( strLink.GetLength() > 0 && strLink.GetAt(strLink.GetLength() - 1) == L'"' )
			strLink = strLink.Left(strLink.GetLength() - 1);
	}
	else
	{
		// strShell from drag and drop or select file dialog
		strLink = strShell;

		m_strShell = strShell;
		if( m_strShell.Find(L' ') != -1 && m_strShell.Find(L'"') == -1 )
		{
			m_strShell.Insert(0, L'"');
			m_strShell.Insert(m_strShell.GetLength(), L'"');
		}
	}

	if( strLink.Right(4).CompareNoCase(L".lnk") == 0 )
	{
		// set title
		wchar_t szTitle[_MAX_PATH];

		if( _wsplitpath_s(
			strLink.GetString(),
			nullptr, 0,
			nullptr, 0,
			szTitle, ARRAYSIZE(szTitle),
			nullptr, 0) == 0 )
		{
			m_strTitle = szTitle;
		}

		CComPtr<IShellLink> shellLink;
		if( SUCCEEDED(shellLink.CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER)) )
		{
			CComPtr<IPersistFile> persistFile;
			if( SUCCEEDED(shellLink.QueryInterface(&persistFile)) )
			{
				if( SUCCEEDED(persistFile->Load(strLink, STGM_READ)) &&
					  SUCCEEDED(shellLink->Resolve(m_hWnd, 0)) )
				{
					WCHAR szBuffer[MAX_PATH];
					WCHAR szBuffer2[2048];
					WIN32_FIND_DATA wfd;

					if( SUCCEEDED(shellLink->GetPath(szBuffer, MAX_PATH, (WIN32_FIND_DATA*)&wfd, SLGP_SHORTPATH)) &&
						  SUCCEEDED(shellLink->GetArguments(szBuffer2, ARRAYSIZE(szBuffer2))) )
					{
						m_strShell = szBuffer;

						if( m_strShell.Find(L' ') != -1 )
						{
							m_strShell.Insert(0, L'"');
							m_strShell.Insert(m_strShell.GetLength(), L'"');
						}

						if( szBuffer2[0] )
						{
							m_strShell += L" ";
							m_strShell += szBuffer2;
						}
					}

					if( SUCCEEDED(shellLink->GetWorkingDirectory(szBuffer, MAX_PATH)) )
					{
						m_strInitialDir = szBuffer;
					}

					int iIcon = 0;
					if( SUCCEEDED(shellLink->GetIconLocation(szBuffer, MAX_PATH, &iIcon)) )
					{
						m_strIcon = szBuffer;
						if( iIcon )
						{
							swprintf_s<MAX_PATH>(szBuffer, L",%i", iIcon);
							m_strIcon += szBuffer;
						}
						m_bUseDefaultIcon = m_strIcon.IsEmpty();
					}

					CComPtr<IShellLinkDataList> dataList;
					DWORD dwFlags;
					if( SUCCEEDED(shellLink.QueryInterface(&dataList)) &&
						  SUCCEEDED(dataList->GetFlags(&dwFlags)) )
					{
						m_nRunAs = (dwFlags & SLDF_RUNAS_USER) == SLDF_RUNAS_USER ? 1 : 0;
					}
				}
			}
		}
	}

	DoDataExchange(DDX_LOAD);
}

//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////

bool CFileNameAndLinkEdit::OnDropFile(CString& strFilename)
{
	p->ConvertShellLink(strFilename);
	return false;
}