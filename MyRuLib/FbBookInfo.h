#ifndef __FBBOOKINFO_H__
#define __FBBOOKINFO_H__

#include <wx/wx.h>

#define MAX_IMAGE_WIDTH 200

class FbBookInfo: public wxObject
{
	public:
		enum Fields {
			ANNT = 0,
			DSCR,
			ISBN,
			FILE,
		};
	public:
		FbBookInfo(int id): m_id(id) {}
		FbBookInfo(const FbBookInfo &info);
		virtual ~FbBookInfo();
		int GetCode() const { return m_id; }
		void SetText(size_t index, const wxString &text);
		wxString GetText(size_t index) const;
		void AddImage(wxString &filename, wxString &imagedata, wxString &imagetype);
		wxString GetHTML(const wxString &md5sum, bool bVertical, bool bEditable, const wxString &filetype) const;
	private:
		wxString GetComments(const wxString md5sum, bool bEditable);
	public:
		int m_id;
		wxString m_text[FILE - 1];
		wxArrayString m_images;
};

#include <wx/dynarray.h>
WX_DECLARE_OBJARRAY(FbBookInfo, FbBookInfoArray);

#endif __FBBOOKINFO_H__