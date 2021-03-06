//////////////////////////////////////////////////////////////////////
// This file was auto-generated by codelite's wxCrafter Plugin
// wxCrafter project file: wxcrafter.wxcp
// Do not modify this file by hand!
//////////////////////////////////////////////////////////////////////

#ifndef WXCRAFTER_BASE_CLASSES_H
#define WXCRAFTER_BASE_CLASSES_H

#include <wx/settings.h>
#include <wx/xrc/xmlres.h>
#include <wx/xrc/xh_bmp.h>
#include <wx/frame.h>
#include <wx/iconbndl.h>
#include <wx/artprov.h>
#include <wx/menu.h>
#include <wx/toolbar.h>
#include <wx/sizer.h>
#include <wx/splitter.h>
#include <wx/panel.h>
#include <wx/glcanvas.h>
#include "CanvasWidget.hpp"
#include <wx/stc/stc.h>
#include <wx/textctrl.h>
#include <wx/statusbr.h>
#include <wx/dialog.h>
#include <wx/statbmp.h>
#include <wx/stattext.h>
#include <wx/hyperlink.h>
#include <wx/statline.h>
#include <wx/button.h>
#include <wx/listbox.h>

class MainFrameBaseClass : public wxFrame
{
public:
    enum {
        ID_PAUSE = 1001,
        ID_RESTART = 1002,
        ID_START = 1003,
        ID_STEP = 1004,
    };
protected:
    wxMenuBar* m_menuBar;
    wxMenu* m_menuFile;
    wxMenuItem* m_menuItemFileNew;
    wxMenuItem* m_menuItemFileOpen;
    wxMenuItem* m_menuItemFileSave;
    wxMenuItem* m_menuItemFileSaveAs;
    wxMenuItem* m_menuItemFileSep1;
    wxMenu* m_menuFileRecent;
    wxMenuItem* m_menuItemFileSep2;
    wxMenuItem* m_menuItemFileExit;
    wxMenu* m_menuView;
    wxMenuItem* m_menuItemViewReset;
    wxMenuItem* m_menuItemViewSep1;
    wxMenuItem* m_menuItemViewLog;
    wxMenuItem* m_menuItemViewCode;
    wxMenu* m_menuSimulation;
    wxMenuItem* m_menuItemSimulationStart;
    wxMenuItem* m_menuItemSimulationPause;
    wxMenuItem* m_menuItemSimulationStep;
    wxMenuItem* m_menuItemSimulationRestart;
    wxMenuItem* m_menuItemSimulationSep1;
    wxMenuItem* m_menuItemSimulationScreenshot;
    wxMenu* m_menuHelp;
    wxMenuItem* m_menuItemHelpPlugins;
    wxMenuItem* m_menuItemHelpSep1;
    wxMenuItem* m_menuItemAbout;
    wxToolBar* m_mainToolbar;
    wxSplitterWindow* m_splitterMain;
    wxPanel* m_splitterPageTop;
    wxSplitterWindow* m_splitterTop;
    wxPanel* m_splitterPageView;
    CanvasWidget* m_glCanvasView;
    wxPanel* m_splitterPageCode;
    wxStyledTextCtrl* m_stcCode;
    wxPanel* m_splitterPageBottom;
    wxTextCtrl* m_textCtrlLog;
    wxStatusBar* m_statusBar;

protected:
    virtual void OnFileNew(wxCommandEvent& event) { event.Skip(); }
    virtual void OnFileOpen(wxCommandEvent& event) { event.Skip(); }
    virtual void OnFileSave(wxCommandEvent& event) { event.Skip(); }
    virtual void OnFileSaveAs(wxCommandEvent& event) { event.Skip(); }
    virtual void OnFileExit(wxCommandEvent& event) { event.Skip(); }
    virtual void OnViewReset(wxCommandEvent& event) { event.Skip(); }
    virtual void OnViewLogCheck(wxCommandEvent& event) { event.Skip(); }
    virtual void OnViewLogChecked(wxUpdateUIEvent& event) { event.Skip(); }
    virtual void OnViewCodeCheck(wxCommandEvent& event) { event.Skip(); }
    virtual void OnViewCodeChecked(wxUpdateUIEvent& event) { event.Skip(); }
    virtual void OnSimulationStart(wxCommandEvent& event) { event.Skip(); }
    virtual void OnSimulationNotRunningUpdateUi(wxUpdateUIEvent& event) { event.Skip(); }
    virtual void OnSimulationRunningUpdateUi(wxUpdateUIEvent& event) { event.Skip(); }
    virtual void OnSimulationPause(wxCommandEvent& event) { event.Skip(); }
    virtual void OnSimulationStep(wxCommandEvent& event) { event.Skip(); }
    virtual void OnSimulationRestart(wxCommandEvent& event) { event.Skip(); }
    virtual void OnSimulationScreenshot(wxCommandEvent& event) { event.Skip(); }
    virtual void OnHelpPlugins(wxCommandEvent& event) { event.Skip(); }
    virtual void OnHelpAbout(wxCommandEvent& event) { event.Skip(); }
    virtual void OnCodeUpdateUi(wxUpdateUIEvent& event) { event.Skip(); }
    virtual void OnCodeChange(wxStyledTextEvent& event) { event.Skip(); }

public:
    MainFrameBaseClass(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("CeCe simulator"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(800,600), long style = wxCAPTION|wxRESIZE_BORDER|wxMAXIMIZE_BOX|wxMINIMIZE_BOX|wxSYSTEM_MENU|wxCLOSE_BOX);
    virtual ~MainFrameBaseClass();
};


class AboutDialogBaseClass : public wxDialog
{
protected:
    wxPanel* m_panelContent;
    wxStaticBitmap* m_staticBitmapHeader;
    wxStaticText* m_staticTextAppName;
    wxStaticText* m_staticTextVersion;
    wxStaticText* m_staticTextBuild;
    wxStaticText* m_staticTextCopyright;
    wxHyperlinkCtrl* m_hyperLinkWeb;
    wxStaticLine* m_staticLineSeparator;
    wxStdDialogButtonSizer* m_stdBtnSizerButtons;
    wxButton* m_buttonOk;

protected:

public:
    AboutDialogBaseClass(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("About"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(300,400), long style = wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX);
    virtual ~AboutDialogBaseClass();
};


class PluginsDialogBase : public wxDialog
{
protected:
    wxStaticText* m_staticTextPath;
    wxListBox* m_listBoxPlugins;
    wxStdDialogButtonSizer* m_stdBtnSizerMain;
    wxButton* m_buttonOk;

protected:
    virtual void OnInitDialog(wxInitDialogEvent& event) { event.Skip(); }

public:
    PluginsDialogBase(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Plugins"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(500,300), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER);
    virtual ~PluginsDialogBase();
};

#endif
