#ifndef __FBFILTERDLG_H__
#define __FBFILTERDLG_H__

#include <wx/wx.h>
#include <wx/intl.h>

#include <wx/string.h>
#include <wx/checkbox.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/treectrl.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include "FbWindow.h"
#include "FbTreeListCtrl.h"
#include "FbFilterObj.h"

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class FbFilterDlg
///////////////////////////////////////////////////////////////////////////////
class FbFilterDlg : public FbDialog
{
	public:
		FbFilterDlg(FbFilterObj & filter);
		static bool Execute(FbFilterObj & filter);

	private:
		FbTreeListCtrl * CreateTree(const wxString & title);

	private:
		enum
		{
			ID_CHECK_LIB = 1000,
			ID_CHECK_USR,
			ID_TREE_LANG,
			ID_TREE_TYPE,
		};
		wxCheckBox* m_checkLib;
		wxCheckBox* m_checkUsr;
		FbTreeListCtrl* m_treeLang;
		FbTreeListCtrl* m_treeType;

	private:
		void OnNoButton( wxCommandEvent& event );
		DECLARE_EVENT_TABLE()
};

#endif //__FBFILTERDLG_H__