#ifndef __FBAUTHLIST_H__
#define __FBAUTHLIST_H__

#include "FbTreeModel.h"
#include "FbDatabase.h"
#include "FbCollection.h"
#include "FbThread.h"

class FbAuthListInfo: public wxObject
{
	public:
		FbAuthListInfo(int author = 0)
			: m_author(author), m_letter(0), m_string(wxEmptyString) {}
		FbAuthListInfo(wxChar letter)
			: m_author(0), m_letter(letter), m_string(wxEmptyString) {}
		FbAuthListInfo(const wxString &string)
			: m_author(0), m_letter(0), m_string(string) {}
		FbAuthListInfo(const FbAuthListInfo & info)
			: m_author(info.m_author), m_letter(info.m_letter), m_string(info.m_string) {}
		bool IsFullText() const
			{ return FbSearchFunction::IsFullText(m_string); }
	private:
		int m_author;
		wxChar m_letter;
		wxString m_string;
		friend class FbAuthListThread;
};

class FbAuthListThread: public FbThread
{
	public:
		FbAuthListThread(wxEvtHandler * frame, const FbMutexLocker &locker, const FbAuthListInfo &info, int order = 0)
			:FbThread(wxTHREAD_JOINABLE), m_frame(frame), m_tester(locker), m_info(info), m_order(order) {}
	protected:
		virtual void * Entry();
		virtual void OnExit();
	private:
		static wxString GetOrder(const wxString &fields, int column);
		void DoAuthor(wxSQLite3Database &database);
		void DoLetter(wxSQLite3Database &database);
		void DoString(wxSQLite3Database &database);
		void DoFullText(wxSQLite3Database &database);
		void MakeModel(wxSQLite3ResultSet &result);
		wxEvtHandler * m_frame;
		FbMutexTester m_tester;
		FbAuthListInfo m_info;
		int m_order;
};

class FbAuthListData: public FbModelData
{
	public:
		FbAuthListData(int code): m_code(code) {}
		virtual wxString GetValue(FbModel & model, size_t col = 0) const;
		int GetCode() const { return m_code; }
	private:
		int m_code;
		DECLARE_CLASS(FbAuthListData);
};

class FbAuthListModel: public FbListModel
{
	public:
		FbAuthListModel(const wxArrayInt &items);
		FbAuthListModel(int order, wxChar letter = 0);
		FbAuthListModel(int order, const wxString &mask);
		virtual ~FbAuthListModel(void);
		virtual void Append(FbModelData * data);
		virtual void Replace(FbModelData * data);
		virtual void Delete();
		void Append(const wxArrayInt &items);
	public:
		virtual size_t GetRowCount() const
			{ return m_items.Count(); }
		virtual FbModelData * GetCurrent()
			{ return GetData(m_position); };
		virtual FbModelData * GetData(size_t row);
	private:
        wxString GetSQL(const wxString & order, const wxString & condition);
	private:
		FbCommonDatabase m_database;
		wxArrayInt m_items;
		FbAuthListData * m_data;
		DECLARE_CLASS(FbAuthListModel);
};

#endif // __FBAUTHLIST_H__
