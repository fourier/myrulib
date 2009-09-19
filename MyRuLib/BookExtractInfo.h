#ifndef __BOOKEXTRACTINFO_H__
#define __BOOKEXTRACTINFO_H__

#include <wx/wx.h>
#include <wx/filename.h>
#include <wx/arrimpl.cpp>
#include <DatabaseResultSet.h>

class BookExtractInfo
{
	public:
		BookExtractInfo(DatabaseResultSet* result);
	public:
        wxString GetBook();
        wxString GetZip(const wxString &path = wxEmptyString);
	public:
		int id_book;
		int id_archive;
		wxString book_name;
		wxString book_path;
		wxString zip_name;
		wxString zip_path;
};

WX_DECLARE_OBJARRAY(BookExtractInfo, BookExtractInfoArray);

#endif // __BOOKEXTRACTINFO_H__
