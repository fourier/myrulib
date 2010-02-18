/////////////////////////////////////////////////////////////////////////////
// Name:        dataview.cpp
// Purpose:     wxDataViewCtrl wxWidgets sample
// Author:      Robert Roebling
// Modified by: Francesco Montorsi, Bo Yang
// Created:     06/01/06
// RCS-ID:      $Id: dataview.cpp 59457 2009-03-09 20:49:22Z RR $
// Copyright:   (c) Robert Roebling
// Licence:     wxWindows license
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "mymodels.h"

#include "wx/dataview.h"
#include "wx/datetime.h"
#include "wx/splitter.h"
#include "wx/aboutdlg.h"
#include "wx/choicdlg.h"
#include "wx/numdlg.h"
#include "wx/spinctrl.h"
#include "wx/imaglist.h"
#include "wx/notebook.h"

// ----------------------------------------------------------------------------
// resources
// ----------------------------------------------------------------------------

#ifndef __WXMSW__
    #include "sample.xpm"
#endif

#include "wx_small.xpm"

// ----------------------------------------------------------------------------
// MyApp
// ----------------------------------------------------------------------------

class MyApp: public wxApp
{
public:
    virtual bool OnInit();
};

// ----------------------------------------------------------------------------
// MyFrame
// ----------------------------------------------------------------------------

class MyFrame : public wxFrame
{
public:
    MyFrame(wxFrame *frame, const wxString &title, int x, int y, int w, int h);
    ~MyFrame();

    void BuildDataViewCtrl(wxPanel* parent,
                           unsigned int nPanel,
                           unsigned long style = 0);

public:     // event handlers

    void OnStyleChange(wxCommandEvent& event);
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

    void OnClearLog(wxCommandEvent& event);
    void OnPageChanged(wxBookCtrlEvent& event);

    void OnAddMozart(wxCommandEvent& event);
    void OnDeleteMusic(wxCommandEvent& event);
    void OnDeleteYear(wxCommandEvent& event);
    void OnSelectNinth(wxCommandEvent& event);
    void OnCollapse(wxCommandEvent& event);
    void OnExpand(wxCommandEvent& event);

    void OnPrependList(wxCommandEvent& event);
    void OnDeleteList(wxCommandEvent& event);

    void OnValueChanged( wxDataViewEvent &event );

    void OnActivated( wxDataViewEvent &event );
    void OnExpanding( wxDataViewEvent &event );
    void OnExpanded( wxDataViewEvent &event );
    void OnCollapsing( wxDataViewEvent &event );
    void OnCollapsed( wxDataViewEvent &event );
    void OnSelectionChanged( wxDataViewEvent &event );

    void OnStartEditing( wxDataViewEvent &event );
    void OnEditingStarted( wxDataViewEvent &event );
    void OnEditingDone( wxDataViewEvent &event );

    void OnHeaderClick( wxDataViewEvent &event );
    void OnHeaderRightClick( wxDataViewEvent &event );
    void OnSorted( wxDataViewEvent &event );

    void OnContextMenu( wxDataViewEvent &event );

    void OnRightClick( wxMouseEvent &event );
    void OnGoto( wxCommandEvent &event);
    void OnAddMany( wxCommandEvent &event);

    void OnBeginDrag( wxDataViewEvent &event );
    void OnDropPossible( wxDataViewEvent &event );
    void OnDrop( wxDataViewEvent &event );

private:
    wxNotebook* m_notebook;

    // the controls stored in the various tabs of the main notebook:

    wxDataViewCtrl* m_ctrl[4];

    // the models associated with the first two DVC:

    wxObjectDataPtr<MyMusicTreeModel> m_music_model;
    wxObjectDataPtr<MyListModel> m_list_model;

    // other data:

    wxDataViewColumn* m_col;

    wxTextCtrl* m_log;
    wxLog *m_logOld;

private:
    DECLARE_EVENT_TABLE()
};


// ----------------------------------------------------------------------------
// MyCustomRenderer
// ----------------------------------------------------------------------------

class MyCustomRenderer: public wxDataViewCustomRenderer
{
public:
    MyCustomRenderer( wxDataViewCellMode mode, int alignment ) :
       wxDataViewCustomRenderer( wxString("long"), mode, alignment )
       { m_height = 25; }

    virtual bool Render( wxRect rect, wxDC *dc, int WXUNUSED(state) )
    {
        dc->SetBrush( *wxRED_BRUSH );
        dc->SetPen( *wxTRANSPARENT_PEN );
        dc->DrawRectangle( rect.Deflate(2) );
        return true;
    }

    virtual bool Activate( wxRect WXUNUSED(cell),
                           wxDataViewModel *WXUNUSED(model),
                           const wxDataViewItem &WXUNUSED(item),
                           unsigned int WXUNUSED(col) )
    {
        wxLogMessage( "MyCustomRenderer Activate()" );
        return false;
    }

    virtual bool LeftClick( wxPoint cursor, wxRect WXUNUSED(cell),
                           wxDataViewModel *WXUNUSED(model),
                           const wxDataViewItem &WXUNUSED(item),
                           unsigned int WXUNUSED(col) )
    {
        wxLogMessage( "MyCustomRenderer LeftClick( %d, %d )", cursor.x, cursor.y );
        return false;
    }

    virtual wxSize GetSize() const
    {
        //return wxSize(60,m_height);
        return wxSize(60,20);
    }

    virtual bool SetValue( const wxVariant &value )
    {
        m_height = value;
        return true;
    }

    virtual bool GetValue( wxVariant &WXUNUSED(value) ) const { return true; }

private:
    long m_height;
};


// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// MyApp
// ----------------------------------------------------------------------------

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
    if ( !wxApp::OnInit() )
        return false;

    MyFrame *frame =
        new MyFrame(NULL, "wxDataViewCtrl sample", 40, 40, 1000, 540);
    SetTopWindow(frame);

    frame->Show(true);
    return true;
}


// ----------------------------------------------------------------------------
// MyFrame
// ----------------------------------------------------------------------------

enum
{
    ID_CLEARLOG = wxID_HIGHEST+1,
    ID_STYLE_MENU,

    // file menu
    //ID_SINGLE,        wxDV_SINGLE==0 so it's always present
    ID_MULTIPLE,
    ID_ROW_LINES,
    ID_HORIZ_RULES,
    ID_VERT_RULES,

    ID_EXIT = wxID_EXIT,

    // about menu
    ID_ABOUT = wxID_ABOUT,


    // control IDs

    ID_MUSIC_CTRL       = 50,

    ID_ADD_MOZART       = 100,
    ID_DELETE_MUSIC     = 101,
    ID_DELETE_YEAR      = 102,
    ID_SELECT_NINTH     = 103,
    ID_COLLAPSE         = 104,
    ID_EXPAND           = 105,

    ID_PREPEND_LIST     = 200,
    ID_DELETE_LIST      = 201,
    ID_GOTO             = 202,
    ID_ADD_MANY         = 203
};

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU_RANGE( ID_MULTIPLE, ID_VERT_RULES, MyFrame::OnStyleChange )
    EVT_MENU( ID_EXIT, MyFrame::OnQuit )
    EVT_MENU( ID_ABOUT, MyFrame::OnAbout )
    EVT_MENU( ID_CLEARLOG, MyFrame::OnClearLog )

    EVT_NOTEBOOK_PAGE_CHANGED( wxID_ANY, MyFrame::OnPageChanged )

    EVT_BUTTON( ID_ADD_MOZART, MyFrame::OnAddMozart )
    EVT_BUTTON( ID_DELETE_MUSIC, MyFrame::OnDeleteMusic )
    EVT_BUTTON( ID_DELETE_YEAR, MyFrame::OnDeleteYear )
    EVT_BUTTON( ID_SELECT_NINTH, MyFrame::OnSelectNinth )
    EVT_BUTTON( ID_COLLAPSE, MyFrame::OnCollapse )
    EVT_BUTTON( ID_EXPAND, MyFrame::OnExpand )

    EVT_BUTTON( ID_PREPEND_LIST, MyFrame::OnPrependList )
    EVT_BUTTON( ID_DELETE_LIST, MyFrame::OnDeleteList )
    EVT_BUTTON( ID_GOTO, MyFrame::OnGoto)
    EVT_BUTTON( ID_ADD_MANY, MyFrame::OnAddMany)

    EVT_DATAVIEW_ITEM_VALUE_CHANGED( ID_MUSIC_CTRL, MyFrame::OnValueChanged )

    EVT_DATAVIEW_ITEM_ACTIVATED(ID_MUSIC_CTRL, MyFrame::OnActivated )
    EVT_DATAVIEW_ITEM_EXPANDING(ID_MUSIC_CTRL, MyFrame::OnExpanding)
    EVT_DATAVIEW_ITEM_EXPANDED(ID_MUSIC_CTRL, MyFrame::OnExpanded)
    EVT_DATAVIEW_ITEM_COLLAPSING(ID_MUSIC_CTRL, MyFrame::OnCollapsing)
    EVT_DATAVIEW_ITEM_COLLAPSED(ID_MUSIC_CTRL, MyFrame::OnCollapsed)
    EVT_DATAVIEW_SELECTION_CHANGED(ID_MUSIC_CTRL, MyFrame::OnSelectionChanged)

    EVT_DATAVIEW_ITEM_START_EDITING(ID_MUSIC_CTRL, MyFrame::OnStartEditing)
    EVT_DATAVIEW_ITEM_EDITING_STARTED(ID_MUSIC_CTRL, MyFrame::OnEditingStarted)
    EVT_DATAVIEW_ITEM_EDITING_DONE(ID_MUSIC_CTRL, MyFrame::OnEditingDone)

    EVT_DATAVIEW_COLUMN_HEADER_CLICK(ID_MUSIC_CTRL, MyFrame::OnHeaderClick)
    EVT_DATAVIEW_COLUMN_HEADER_RIGHT_CLICKED(ID_MUSIC_CTRL, MyFrame::OnHeaderRightClick)
    EVT_DATAVIEW_COLUMN_SORTED(ID_MUSIC_CTRL, MyFrame::OnSorted)

    EVT_DATAVIEW_ITEM_CONTEXT_MENU(ID_MUSIC_CTRL, MyFrame::OnContextMenu)

    EVT_DATAVIEW_ITEM_BEGIN_DRAG( ID_MUSIC_CTRL, MyFrame::OnBeginDrag )
    EVT_DATAVIEW_ITEM_DROP_POSSIBLE( ID_MUSIC_CTRL, MyFrame::OnDropPossible )
    EVT_DATAVIEW_ITEM_DROP( ID_MUSIC_CTRL, MyFrame::OnDrop )

    EVT_RIGHT_UP(MyFrame::OnRightClick)
END_EVENT_TABLE()

MyFrame::MyFrame(wxFrame *frame, const wxString &title, int x, int y, int w, int h):
  wxFrame(frame, wxID_ANY, title, wxPoint(x, y), wxSize(w, h))
{
    m_log = NULL;
    m_col = NULL;

    m_ctrl[0] = NULL;
    m_ctrl[1] = NULL;
    m_ctrl[2] = NULL;
    m_ctrl[3] = NULL;

    SetIcon(wxICON(sample));


    // build the menus
    // ----------------

    wxMenu *style_menu = new wxMenu;
    //style_menu->AppendCheckItem(ID_SINGLE, "Single selection"));
    style_menu->AppendCheckItem(ID_MULTIPLE, "Multiple selection");
    style_menu->AppendCheckItem(ID_ROW_LINES, "Alternating colours");
    style_menu->AppendCheckItem(ID_HORIZ_RULES, "Display horizontal rules");
    style_menu->AppendCheckItem(ID_VERT_RULES, "Display vertical rules");

    wxMenu *file_menu = new wxMenu;
    file_menu->Append(ID_CLEARLOG, "Clear log");
    file_menu->Append(ID_STYLE_MENU, "&Style", style_menu);
    file_menu->AppendSeparator();
    file_menu->Append(ID_EXIT, "E&xit");

    wxMenu *about_menu = new wxMenu;
    about_menu->Append(ID_ABOUT, "&About");

    wxMenuBar *menu_bar = new wxMenuBar;
    menu_bar->Append(file_menu, "&File");
    menu_bar->Append(about_menu, "&About");

    SetMenuBar(menu_bar);
    CreateStatusBar();


    // first page of the notebook
    // --------------------------

    m_notebook = new wxNotebook( this, wxID_ANY );

    wxPanel *firstPanel = new wxPanel( m_notebook, wxID_ANY );

    BuildDataViewCtrl(firstPanel, 0);    // sets m_ctrl[0]

    wxBoxSizer *button_sizer = new wxBoxSizer( wxHORIZONTAL );
    button_sizer->Add( new wxButton( firstPanel, ID_ADD_MOZART,  "Add Mozart"),             0, wxALL, 10 );
    button_sizer->Add( new wxButton( firstPanel, ID_DELETE_MUSIC,"Delete selected"),        0, wxALL, 10 );
    button_sizer->Add( new wxButton( firstPanel, ID_DELETE_YEAR, "Delete \"Year\" column"), 0, wxALL, 10 );
    button_sizer->Add( new wxButton( firstPanel, ID_SELECT_NINTH,"Select ninth symphony"),  0, wxALL, 10 );
    button_sizer->Add( new wxButton( firstPanel, ID_COLLAPSE,    "Collapse"),               0, wxALL, 10 );
    button_sizer->Add( new wxButton( firstPanel, ID_EXPAND,      "Expand"),                 0, wxALL, 10 );

    wxSizer *firstPanelSz = new wxBoxSizer( wxVERTICAL );
    m_ctrl[0]->SetMinSize(wxSize(-1, 200));
    firstPanelSz->Add(m_ctrl[0], 1, wxGROW|wxALL, 5);
    firstPanelSz->Add(
        new wxStaticText(firstPanel, wxID_ANY, "Most of the cells above are editable!"),
        0, wxGROW|wxALL, 5);
    firstPanelSz->Add(button_sizer);
    firstPanel->SetSizerAndFit(firstPanelSz);


    // second page of the notebook
    // ---------------------------

    wxPanel *secondPanel = new wxPanel( m_notebook, wxID_ANY );

    BuildDataViewCtrl(secondPanel, 1);    // sets m_ctrl[1]

    wxBoxSizer *button_sizer2 = new wxBoxSizer( wxHORIZONTAL );
    button_sizer2->Add( new wxButton( secondPanel, ID_PREPEND_LIST,"Prepend"),         0, wxALL, 10 );
    button_sizer2->Add( new wxButton( secondPanel, ID_DELETE_LIST, "Delete selected"), 0, wxALL, 10 );
    button_sizer2->Add( new wxButton( secondPanel, ID_GOTO,        "Goto 50"),         0, wxALL, 10 );
    button_sizer2->Add( new wxButton( secondPanel, ID_ADD_MANY,    "Add 1000"),        0, wxALL, 10 );

    wxSizer *secondPanelSz = new wxBoxSizer( wxVERTICAL );
    secondPanelSz->Add(m_ctrl[1], 1, wxGROW|wxALL, 5);
    secondPanelSz->Add(button_sizer2);
    secondPanel->SetSizerAndFit(secondPanelSz);


    // third page of the notebook
    // ---------------------------

    wxPanel *thirdPanel = new wxPanel( m_notebook, wxID_ANY );

    BuildDataViewCtrl(thirdPanel, 2);    // sets m_ctrl[2]

    wxSizer *thirdPanelSz = new wxBoxSizer( wxVERTICAL );
    thirdPanelSz->Add(m_ctrl[2], 1, wxGROW|wxALL, 5);
    thirdPanel->SetSizerAndFit(thirdPanelSz);


    // fourth page of the notebook
    // ---------------------------

    wxPanel *fourthPanel = new wxPanel( m_notebook, wxID_ANY );

    BuildDataViewCtrl(fourthPanel, 3);    // sets m_ctrl[3]

    wxSizer *fourthPanelSz = new wxBoxSizer( wxVERTICAL );
    fourthPanelSz->Add(m_ctrl[3], 1, wxGROW|wxALL, 5);
    fourthPanel->SetSizerAndFit(fourthPanelSz);



    // complete GUI
    // ------------

    m_notebook->AddPage(firstPanel, "MyMusicTreeModel");
    m_notebook->AddPage(secondPanel, "MyListModel");
    m_notebook->AddPage(thirdPanel, "wxDataViewListCtrl");
    m_notebook->AddPage(fourthPanel, "wxDataViewTreeCtrl");

    wxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    m_log = new wxTextCtrl( this, wxID_ANY, wxString(), wxDefaultPosition,
                            wxDefaultSize, wxTE_MULTILINE );
    m_log->SetMinSize(wxSize(-1, 100));
    m_logOld = wxLog::SetActiveTarget(new wxLogTextCtrl(m_log));
    wxLogMessage( "This is the log window" );

    mainSizer->Add( m_notebook, 1, wxGROW );
    mainSizer->Add( m_log, 0, wxGROW );

    SetSizerAndFit(mainSizer);
}

MyFrame::~MyFrame()
{
    delete wxLog::SetActiveTarget(m_logOld);
}

void MyFrame::BuildDataViewCtrl(wxPanel* parent, unsigned int nPanel, unsigned long style)
{
    switch (nPanel)
    {
    case 0:
        {
            wxASSERT(!m_ctrl[0] && !m_music_model);
            m_ctrl[0] =
                new wxDataViewCtrl( parent, ID_MUSIC_CTRL, wxDefaultPosition,
                                    wxDefaultSize, style );

            m_music_model = new MyMusicTreeModel;
            m_ctrl[0]->AssociateModel( m_music_model.get() );

            m_ctrl[0]->EnableDragSource( wxDF_UNICODETEXT );
            m_ctrl[0]->EnableDropTarget( wxDF_UNICODETEXT );

            // column 0 of the view control:

            wxDataViewTextRenderer *tr =
                new wxDataViewTextRenderer( "string", wxDATAVIEW_CELL_INERT );
            wxDataViewColumn *column0 =
                new wxDataViewColumn( "title", tr, 0, 200, wxALIGN_LEFT,
                                      wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE );
            m_ctrl[0]->AppendColumn( column0 );
#if 0
            // Call this and sorting is enabled
            // immediatly upon start up.
            column0->SetAsSortKey();
#endif

            // column 1 of the view control:

            tr = new wxDataViewTextRenderer( "string", wxDATAVIEW_CELL_EDITABLE );
            wxDataViewColumn *column1 =
                new wxDataViewColumn( "artist", tr, 1, 150, wxALIGN_LEFT,
                                      wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_REORDERABLE |
                                      wxDATAVIEW_COL_RESIZABLE );
            column1->SetMinWidth(150); // this column can't be resized to be smaller
            m_ctrl[0]->AppendColumn( column1 );

            // column 2 of the view control:

            wxDataViewSpinRenderer *sr =
                new wxDataViewSpinRenderer( 0, 2010, wxDATAVIEW_CELL_EDITABLE, wxALIGN_RIGHT );
            wxDataViewColumn *column2 =
                new wxDataViewColumn( "year", sr, 2, 60, wxALIGN_LEFT,
                                      wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_REORDERABLE );
            m_ctrl[0]->AppendColumn( column2 );

            // column 3 of the view control:

            wxArrayString choices;
            choices.Add( "good" );
            choices.Add( "bad" );
            choices.Add( "lousy" );
            wxDataViewChoiceRenderer *c =
                new wxDataViewChoiceRenderer( choices, wxDATAVIEW_CELL_EDITABLE, wxALIGN_RIGHT );
            wxDataViewColumn *column3 =
                new wxDataViewColumn( "rating", c, 3, 100, wxALIGN_LEFT,
                                      wxDATAVIEW_COL_REORDERABLE | wxDATAVIEW_COL_RESIZABLE );
            m_ctrl[0]->AppendColumn( column3 );

            // column 4 of the view control:

            m_ctrl[0]->AppendProgressColumn( "popularity", 4, wxDATAVIEW_CELL_INERT, 80 );

            // column 5 of the view control:

            MyCustomRenderer *cr = new MyCustomRenderer( wxDATAVIEW_CELL_ACTIVATABLE, wxALIGN_RIGHT );
            wxDataViewColumn *column5 =
                new wxDataViewColumn( "custom", cr, 5, -1, wxALIGN_LEFT,
                                      wxDATAVIEW_COL_RESIZABLE );
            m_ctrl[0]->AppendColumn( column5 );


            // select initially the ninth symphony:
            m_ctrl[0]->Select(m_music_model->GetNinthItem());
        }
        break;

    case 1:
        {
            wxASSERT(!m_ctrl[1] && !m_list_model);
            m_ctrl[1] = new wxDataViewCtrl( parent, wxID_ANY, wxDefaultPosition,
                                            wxDefaultSize, style );

            m_list_model = new MyListModel;
            m_ctrl[1]->AssociateModel( m_list_model.get() );

            // the various columns
#if 1
            m_ctrl[1]->AppendTextColumn("editable string", 0, wxDATAVIEW_CELL_EDITABLE, 120);
            m_ctrl[1]->AppendIconTextColumn(wxIcon(wx_small_xpm), 1, wxDATAVIEW_CELL_INERT )->SetTitle( "icon");
#else
            m_ctrl[1]->AppendTextColumn("editable string", 0, wxDATAVIEW_CELL_EDITABLE);
            m_ctrl[1]->AppendIconTextColumn("icon", 1, wxDATAVIEW_CELL_INERT);
#endif
            m_ctrl[1]->AppendColumn(
                new wxDataViewColumn("attributes", new wxDataViewTextRendererAttr, 2 ));
        }
        break;

    case 2:
        {
            wxASSERT(!m_ctrl[2]);
            wxDataViewListCtrl* lc =
                new wxDataViewListCtrl( parent, wxID_ANY, wxDefaultPosition,
                                        wxDefaultSize, style );
            m_ctrl[2] = lc;

            lc->AppendToggleColumn( "Toggle" );
            lc->AppendTextColumn( "Text" );
            lc->AppendProgressColumn( "Progress" );

            wxVector<wxVariant> data;
            for (unsigned int i=0; i<10; i++)
            {
                data.clear();
                data.push_back( (i%3) == 0 );
                data.push_back( wxString::Format("row %d", i) );
                data.push_back( long(5*i) );

                lc->AppendItem( data );
            }
        }
        break;

    case 3:
        {
            wxASSERT(!m_ctrl[3]);
            wxDataViewTreeCtrl* tc =
                new wxDataViewTreeCtrl( parent, wxID_ANY, wxDefaultPosition,
                                        wxDefaultSize, style );
            m_ctrl[3] = tc;

            wxImageList *ilist = new wxImageList( 16, 16 );
            ilist->Add( wxIcon(wx_small_xpm) );
            tc->SetImageList( ilist );

            wxDataViewItem parent =
                tc->AppendContainer( wxDataViewItem(0), "The Root", 0 );
            tc->AppendItem( parent, "Child 1", 0 );
            tc->AppendItem( parent, "Child 2", 0 );
            tc->AppendItem( parent, "Child 3, very long, long, long, long", 0 );

            wxDataViewItem cont =
                tc->AppendContainer( parent, "Container child", 0 );
            tc->AppendItem( cont, "Child 4", 0 );
            tc->AppendItem( cont, "Child 5", 0 );

            tc->Expand(cont);
        }
        break;
    }
}


// ----------------------------------------------------------------------------
// MyFrame - generic event handlers
// ----------------------------------------------------------------------------

void MyFrame::OnClearLog( wxCommandEvent& WXUNUSED(event) )
{
    m_log->Clear();
}

void MyFrame::OnPageChanged( wxBookCtrlEvent& WXUNUSED(event) )
{
    unsigned int nPanel = m_notebook->GetSelection();

    GetMenuBar()->FindItem(ID_STYLE_MENU)->SetItemLabel(
                wxString::Format("Style of panel #%d", nPanel+1));

    for (unsigned int id = ID_MULTIPLE; id <= ID_VERT_RULES; id++)
    {
        unsigned long style = 0;
        switch (id)
        {
        /*case ID_SINGLE:
            style = wxDV_SINGLE;
            break;*/
        case ID_MULTIPLE:
            style = wxDV_MULTIPLE;
            break;
        case ID_ROW_LINES:
            style = wxDV_ROW_LINES;
            break;
        case ID_HORIZ_RULES:
            style = wxDV_HORIZ_RULES;
            break;
        case ID_VERT_RULES:
            style = wxDV_VERT_RULES;
            break;
        default:
            wxFAIL;
        }

        GetMenuBar()->FindItem(id)->Check( m_ctrl[nPanel]->HasFlag(style) );
    }
}

void MyFrame::OnStyleChange( wxCommandEvent& WXUNUSED(event) )
{
    unsigned int nPanel = m_notebook->GetSelection();

    // build the style
    unsigned long style = 0;
    /*if (GetMenuBar()->FindItem(ID_SINGLE)->IsChecked())
        style |= wxDV_SINGLE;*/
    if (GetMenuBar()->FindItem(ID_MULTIPLE)->IsChecked())
        style |= wxDV_MULTIPLE;
    if (GetMenuBar()->FindItem(ID_ROW_LINES)->IsChecked())
        style |= wxDV_ROW_LINES;
    if (GetMenuBar()->FindItem(ID_HORIZ_RULES)->IsChecked())
        style |= wxDV_HORIZ_RULES;
    if (GetMenuBar()->FindItem(ID_VERT_RULES)->IsChecked())
        style |= wxDV_VERT_RULES;

    wxSizer* sz = m_ctrl[nPanel]->GetContainingSizer();
    wxASSERT(sz);

    sz->Detach(m_ctrl[nPanel]);
    wxDELETE(m_ctrl[nPanel]);
    m_ctrl[nPanel] = NULL;

    if (nPanel == 0)
        m_music_model.reset(NULL);
    else if (nPanel == 1)
        m_list_model.reset(NULL);

    // rebuild the DVC for the selected panel:
    BuildDataViewCtrl((wxPanel*)m_notebook->GetPage(nPanel), nPanel, style);

    sz->Prepend(m_ctrl[nPanel], 1, wxGROW|wxALL, 5);
    sz->Layout();
}

void MyFrame::OnQuit( wxCommandEvent& WXUNUSED(event) )
{
    Close(true);
}

void MyFrame::OnAbout( wxCommandEvent& WXUNUSED(event) )
{
    wxAboutDialogInfo info;
    info.SetName(_("DataView sample"));
    info.SetDescription(_("This sample demonstrates wxDataViewCtrl"));
    info.SetCopyright(_T("(C) 2007-2009 Robert Roebling"));
    info.AddDeveloper("Robert Roebling");
    info.AddDeveloper("Francesco Montorsi");

    wxAboutBox(info);
}


// ----------------------------------------------------------------------------
// MyFrame - event handlers for the first page
// ----------------------------------------------------------------------------

void MyFrame::OnBeginDrag( wxDataViewEvent &event )
{
    wxDataViewItem item( event.GetItem() );

    // only allow drags for item, not containers
    if (m_music_model->IsContainer( item ) )
    {
        event.Veto();
        return;
    }

    MyMusicTreeModelNode *node = (MyMusicTreeModelNode*) item.GetID();
    wxTextDataObject *obj = new wxTextDataObject;
    obj->SetText( node->m_title );
    event.SetDataObject( obj );
}

void MyFrame::OnDropPossible( wxDataViewEvent &event )
{
    wxDataViewItem item( event.GetItem() );

    // only allow drags for item, not containers
    if (m_music_model->IsContainer( item ) )
        event.Veto();

    if (event.GetDataFormat() != wxDF_UNICODETEXT)
        event.Veto();
}

void MyFrame::OnDrop( wxDataViewEvent &event )
{
    wxDataViewItem item( event.GetItem() );

    // only allow drops for item, not containers
    if (m_music_model->IsContainer( item ) )
    {
        event.Veto();
        return;
    }

    if (event.GetDataFormat() != wxDF_UNICODETEXT)
    {
        event.Veto();
        return;
    }

    wxTextDataObject obj;
    obj.SetData( wxDF_UNICODETEXT, event.GetDataSize(), event.GetDataBuffer() );

    wxLogMessage( "Text dropped: %s", obj.GetText() );
}

void MyFrame::OnAddMozart( wxCommandEvent& WXUNUSED(event) )
{
    m_music_model->AddToClassical( "Kleine Nachtmusik", "Wolfgang Mozart", 1787 );
}

void MyFrame::OnDeleteMusic( wxCommandEvent& WXUNUSED(event) )
{
    wxDataViewItemArray items;
    int len = m_ctrl[0]->GetSelections( items );
    for( int i = 0; i < len; i ++ )
        if (items[i].IsOk())
            m_music_model->Delete( items[i] );
}

void MyFrame::OnDeleteYear( wxCommandEvent& WXUNUSED(event) )
{
    m_ctrl[0]->DeleteColumn( m_ctrl[0]->GetColumn( 2 ) );
    FindWindow( ID_DELETE_YEAR )->Disable();
}

void MyFrame::OnSelectNinth( wxCommandEvent& WXUNUSED(event) )
{
    if (!m_music_model->GetNinthItem().IsOk())
    {
        wxLogError( "Cannot select the ninth symphony: it was removed!" );
        return;
    }

    m_ctrl[0]->Select( m_music_model->GetNinthItem() );
}

void MyFrame::OnCollapse( wxCommandEvent& WXUNUSED(event) )
{
    wxDataViewItem item = m_ctrl[0]->GetSelection();
    if (item.IsOk())
        m_ctrl[0]->Collapse( item );
}

void MyFrame::OnExpand( wxCommandEvent& WXUNUSED(event) )
{
    wxDataViewItem item = m_ctrl[0]->GetSelection();
    if (item.IsOk())
        m_ctrl[0]->Expand( item );
}

void MyFrame::OnValueChanged( wxDataViewEvent &event )
{
    if (!m_log)
        return;

    wxLogMessage( "wxEVT_DATAVIEW_ITEM_VALUE_CHANGED, Item Id: %d;  Column: %d",
                  event.GetItem().GetID(), event.GetColumn() );
}

void MyFrame::OnActivated( wxDataViewEvent &event )
{
    if(!m_log)
        return;

    wxString title = m_music_model->GetTitle( event.GetItem() );
    wxLogMessage( "wxEVT_COMMAND_DATAVIEW_ITEM_ACTIVATED, Item: %s", title );

    if (m_ctrl[0]->IsExpanded( event.GetItem() ))
        wxLogMessage( "Item: %s is expanded", title );
}

void MyFrame::OnSelectionChanged( wxDataViewEvent &event )
{
    if(!m_log)
        return;

    wxString title = m_music_model->GetTitle( event.GetItem() );
    if (title.empty())
        title = "None";

    wxLogMessage( "wxEVT_COMMAND_DATAVIEW_SELECTION_CHANGED, First selected Item: %s", title );
}

void MyFrame::OnExpanding( wxDataViewEvent &event )
{
    if (!m_log)
        return;

    wxString title = m_music_model->GetTitle( event.GetItem() );
    wxLogMessage( "wxEVT_COMMAND_DATAVIEW_ITEM_EXPANDING, Item: %s", title );
}


void MyFrame::OnStartEditing( wxDataViewEvent &event )
{
    wxString artist = m_music_model->GetArtist( event.GetItem() );
    if (artist == "Ludwig van Beethoven")
    {
        event.Veto();

        if (!m_log)
           return;

        wxLogMessage( "wxEVT_COMMAND_DATAVIEW_ITEM_START_EDITING vetoed, Item: %s", artist );
    }
}

void MyFrame::OnEditingStarted( wxDataViewEvent &event )
{
    if (!m_log)
        return;

    wxString title = m_music_model->GetTitle( event.GetItem() );
    wxLogMessage( "wxEVT_COMMAND_DATAVIEW_ITEM_EDITING_STARTED, Item: %s", title );
}

void MyFrame::OnEditingDone( wxDataViewEvent &event )
{
    if (!m_log)
        return;

    wxString title = m_music_model->GetTitle( event.GetItem() );
    wxLogMessage( "wxEVT_COMMAND_DATAVIEW_ITEM_EDITING_DONE, Item: %s", title );
}

void MyFrame::OnExpanded( wxDataViewEvent &event )
{
    if (!m_log)
        return;

    wxString title = m_music_model->GetTitle( event.GetItem() );
    wxLogMessage( "wxEVT_COMMAND_DATAVIEW_ITEM_EXPANDED, Item: %s", title );
}

void MyFrame::OnCollapsing( wxDataViewEvent &event )
{
    if (!m_log)
        return;

    wxString title = m_music_model->GetTitle( event.GetItem() );
    wxLogMessage( "wxEVT_COMMAND_DATAVIEW_ITEM_COLLAPSING, Item: %s", title );
}

void MyFrame::OnCollapsed( wxDataViewEvent &event )
{
    if (!m_log)
        return;

    wxString title = m_music_model->GetTitle( event.GetItem() );
    wxLogMessage( "wxEVT_COMMAND_DATAVIEW_ITEM_COLLAPSED, Item: %s", title );
}

void MyFrame::OnContextMenu( wxDataViewEvent &event )
{
    if (!m_log)
        return;

    wxString title = m_music_model->GetTitle( event.GetItem() );
    wxLogMessage( "wxEVT_COMMAND_DATAVIEW_ITEM_CONTEXT_MENU, Item: %s", title );

    wxMenu menu;
    menu.Append( 1, "menuitem 1" );
    menu.Append( 2, "menuitem 2" );
    menu.Append( 3, "menuitem 3" );

    m_ctrl[0]->PopupMenu(&menu);
}

void MyFrame::OnHeaderClick( wxDataViewEvent &event )
{
    // we need to skip the event to let the default behaviour of sorting by
    // this column when it is clicked to take place
    event.Skip();

    if (!m_log)
        return;

    int pos = m_ctrl[0]->GetColumnPosition( event.GetDataViewColumn() );

    wxLogMessage( "wxEVT_COMMAND_DATAVIEW_COLUMN_HEADER_CLICK, Column position: %d", pos );
}

void MyFrame::OnHeaderRightClick( wxDataViewEvent &event )
{
    if(!m_log)
        return;

    int pos = m_ctrl[0]->GetColumnPosition( event.GetDataViewColumn() );

    wxLogMessage( "wxEVT_COMMAND_DATAVIEW_COLUMN_HEADER_RIGHT_CLICK, Column position: %d", pos );
}

void MyFrame::OnSorted( wxDataViewEvent &event )
{
    if(!m_log)
        return;

    int pos = m_ctrl[0]->GetColumnPosition( event.GetDataViewColumn() );

    wxLogMessage( "wxEVT_COMMAND_DATAVIEW_COLUMN_SORTED, Column position: %d", pos );
}

void MyFrame::OnRightClick( wxMouseEvent &event )
{
    if(!m_log)
        return;

    wxLogMessage( "wxEVT_MOUSE_RIGHT_UP, Click Point is X: %d, Y: %d",
                 event.GetX(), event.GetY() );
}


// ----------------------------------------------------------------------------
// MyFrame - event handlers for the second page
// ----------------------------------------------------------------------------

void MyFrame::OnPrependList( wxCommandEvent& WXUNUSED(event) )
{
    m_list_model->Prepend("Test");
}

void MyFrame::OnDeleteList( wxCommandEvent& WXUNUSED(event) )
{
    wxDataViewItemArray items;
    int len = m_ctrl[1]->GetSelections( items );
    if (len > 0)
        m_list_model->DeleteItems( items );
}

void MyFrame::OnGoto(wxCommandEvent& WXUNUSED(event))
{
    wxDataViewItem item = m_list_model->GetItem( 50 );
    m_ctrl[1]->EnsureVisible(item,m_col);
}

void MyFrame::OnAddMany(wxCommandEvent& WXUNUSED(event))
{
    m_list_model->AddMany();
}

