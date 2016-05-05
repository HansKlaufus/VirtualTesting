///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Dec 21 2009)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __gui__
#define __gui__

#include <wx/statusbr.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class cGUI
///////////////////////////////////////////////////////////////////////////////
class cGUI : public wxFrame 
{
	private:
	
	protected:
		wxStatusBar* m_StatusBar;
		wxMenuBar* m_MenuBar;
		wxMenu* m_MenuFile;
		wxMenu* m_MenuSettings;
		wxMenu* m_MenuCamera;
		wxMenu* m_MenuRun;
		wxPanel* m_Panel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnMenuOpen( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMenuSaveAs( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMenuClose( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMenuQuit( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMenuBackgroundRaster( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMenuAxisFrame( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMenuForceInput( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMenuForce( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMenuDots( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMenuSpheres( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMenuCSVFile( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMenuVerbose( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMenuX( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMenuY( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMenuZ( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMenuStart( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMenuStop( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMenuReset( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMenuConnect( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMenuDisconnect( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		cGUI( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("GUI"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 800,600 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~cGUI();
	
};

#endif //__gui__
