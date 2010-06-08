#ifndef __FBFRAMESEQUEN_H__
#define __FBFRAMESEQUEN_H__

#include <wx/wx.h>
#include <wx/toolbar.h>
#include <wx/aui/tabmdi.h>
#include <wx/listbox.h>
#include <wx/splitter.h>
#include <wx/html/htmlwin.h>
#include "FbFrameBase.h"
#include "FbSeqnList.h"

class FbFrameSequen : public FbFrameBase
{
	public:
		FbFrameSequen(wxAuiMDIParentFrame * parent);
		virtual wxString GetTitle() const { return _("Series"); };
		void FindSequence(const wxString &text);
		void OpenSequence(const int sequence, const int book);
		void ActivateAuthors();
		virtual void ShowFullScreen(bool show);
		void Localize(bool bUpdateMenu);
	protected:
		virtual void CreateControls();
		virtual void UpdateBooklist();
		virtual wxMenuBar * CreateMenuBar();
		virtual wxToolBar * CreateToolBar(long style = wxTB_FLAT|wxTB_NODIVIDER|wxTB_HORZ_TEXT, wxWindowID winid = wxID_ANY, const wxString& name = wxEmptyString);
	private:
		void ReplaceData(int old_id, int new_id, wxTreeItemId selected, const wxString &newname);
		void ShowContextMenu(const wxPoint& pos, wxTreeItemId item);
		BookTreeItemData * GetSelectedBook();
		void CreateMasterThread();
		void CreateColumns();
	private:
		wxSplitterWindow * m_BooksSplitter;
		wxTextCtrl * m_FindText;
		wxStaticText * m_FindInfo;
	private:
		wxString m_info;
		int m_SequenceCode;
	private:
		void OnMasterSelected(wxTreeEvent & event);
		void OnBooksCount(wxCommandEvent& event);
		void OnColClick(wxListEvent& event);
		void OnContextMenu(wxTreeEvent& event);
		void OnLetterClicked(wxCommandEvent& event);
		void OnCharEvent(wxKeyEvent& event);
		void OnFindEnter(wxCommandEvent& event);
		void OnMasterAppend(wxCommandEvent& event);
		void OnMasterModify(wxCommandEvent& event);
		void OnMasterDelete(wxCommandEvent& event);
		void OnAppendAuthor(wxCommandEvent& event);
		void OnAppendSequence(wxCommandEvent& event);
		void OnModel( FbArrayEvent& event );
		void OnArray( FbArrayEvent& event );
		DECLARE_EVENT_TABLE()
		DECLARE_CLASS(FbFrameSequen)
	protected:
		class MasterMenu: public wxMenu
		{
			public:
				MasterMenu(int id);
		};
		class EditDlg: public FbDialog
		{
			public:
				EditDlg( const wxString& title = wxEmptyString, int id = 0 );
				static int Append(wxString &newname);
				static int Modify(int id, wxString &newname);
			protected:
				virtual void EndModal(int retCode);
			private:
				bool Load(int id);
				int Find();
				int DoAppend();
				int DoUpdate();
				int DoModify();
				int DoReplace();
				wxString GetValue() { return m_edit.GetValue().Trim(false).Trim(true); };
			private:
				wxTextCtrl m_edit;
				wxStaticText m_text;
				FbCommonDatabase m_database;
				int m_id;
				int m_exists;
		};

	private:
		class MenuBar: public FbFrameMenu
		{
			public:
				MenuBar();
		};
		class MenuMaster: public FbMenu {
			public: MenuMaster();
		};

};

#endif // __FBFRAMESEQUEN_H__
