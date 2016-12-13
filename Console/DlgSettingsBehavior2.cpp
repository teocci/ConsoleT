#include "stdafx.h"
#include "resource.h"

#include "DlgSettingsBehavior2.h"

//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////

DlgSettingsBehavior2::DlgSettingsBehavior2(CComPtr<IXMLDOMElement>& pOptionsRoot)
: DlgSettingsBase(pOptionsRoot)
{
	IDD = IDD_SETTINGS_BEHAVIOR_2;
}

//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////

LRESULT DlgSettingsBehavior2::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	m_behaviorSettings2.Load(m_pOptionsRoot);
	DoDataExchange(DDX_LOAD);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////

LRESULT DlgSettingsBehavior2::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (wID == IDOK)
	{
		DoDataExchange(DDX_SAVE);

		BehaviorSettings2& behaviorSettings2 = g_settingsHandler->GetBehaviorSettings2();

		behaviorSettings2 = m_behaviorSettings2;
		m_behaviorSettings2.Save(m_pOptionsRoot);
	}

	return 0;
}
