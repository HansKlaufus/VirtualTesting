///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Dec 21 2009)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "gui.h"

///////////////////////////////////////////////////////////////////////////

cGUI::cGUI( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	m_StatusBar = this->CreateStatusBar( 3, wxST_SIZEGRIP, wxID_ANY );
	m_MenuBar = new wxMenuBar( 0 );
	m_MenuFile = new wxMenu();
	wxMenuItem* m_MenuOpen;
	m_MenuOpen = new wxMenuItem( m_MenuFile, wxID_ANY, wxString( wxT("Open") ) , wxEmptyString, wxITEM_NORMAL );
	m_MenuFile->Append( m_MenuOpen );
	
	wxMenuItem* m_MenuSaveAs;
	m_MenuSaveAs = new wxMenuItem( m_MenuFile, wxID_ANY, wxString( wxT("Save As") ) , wxEmptyString, wxITEM_NORMAL );
	m_MenuFile->Append( m_MenuSaveAs );
	
	wxMenuItem* m_MenuClose;
	m_MenuClose = new wxMenuItem( m_MenuFile, wxID_ANY, wxString( wxT("Close") ) , wxEmptyString, wxITEM_NORMAL );
	m_MenuFile->Append( m_MenuClose );
	
	wxMenuItem* m_MenuQuit;
	m_MenuQuit = new wxMenuItem( m_MenuFile, wxID_ANY, wxString( wxT("Quit") ) , wxEmptyString, wxITEM_NORMAL );
	m_MenuFile->Append( m_MenuQuit );
	
	m_MenuBar->Append( m_MenuFile, wxT("File") );
	
	m_MenuSettings = new wxMenu();
	wxMenuItem* m_MenuBackgroundRaster;
	m_MenuBackgroundRaster = new wxMenuItem( m_MenuSettings, wxID_ANY, wxString( wxT("Show Background Raster") ) , wxEmptyString, wxITEM_CHECK );
	m_MenuSettings->Append( m_MenuBackgroundRaster );
	m_MenuBackgroundRaster->Check( true );
	
	wxMenuItem* m_MenuAxisFrame;
	m_MenuAxisFrame = new wxMenuItem( m_MenuSettings, wxID_ANY, wxString( wxT("Show Axis Frame") ) , wxEmptyString, wxITEM_CHECK );
	m_MenuSettings->Append( m_MenuAxisFrame );
	m_MenuAxisFrame->Check( true );
	
	wxMenuItem* m_Separator1;
	m_Separator1 = m_MenuSettings->AppendSeparator();
	
	wxMenuItem* m_MenuForceInput;
	m_MenuForceInput = new wxMenuItem( m_MenuSettings, wxID_ANY, wxString( wxT("Show Force Input Vectors") ) , wxEmptyString, wxITEM_CHECK );
	m_MenuSettings->Append( m_MenuForceInput );
	
	wxMenuItem* m_MenuForce;
	m_MenuForce = new wxMenuItem( m_MenuSettings, wxID_ANY, wxString( wxT("Show Force Vectors") ) , wxEmptyString, wxITEM_CHECK );
	m_MenuSettings->Append( m_MenuForce );
	
	wxMenuItem* m_Separator2;
	m_Separator2 = m_MenuSettings->AppendSeparator();
	
	wxMenuItem* m_MenuDots;
	m_MenuDots = new wxMenuItem( m_MenuSettings, wxID_ANY, wxString( wxT("Connect the dots") ) , wxEmptyString, wxITEM_RADIO );
	m_MenuSettings->Append( m_MenuDots );
	
	wxMenuItem* m_MenuSpheres;
	m_MenuSpheres = new wxMenuItem( m_MenuSettings, wxID_ANY, wxString( wxT("Balance the balls") ) , wxEmptyString, wxITEM_RADIO );
	m_MenuSettings->Append( m_MenuSpheres );
	m_MenuSpheres->Check( true );
	
	wxMenuItem* m_Separator3;
	m_Separator3 = m_MenuSettings->AppendSeparator();
	
	wxMenuItem* m_MenuCSVFile;
	m_MenuCSVFile = new wxMenuItem( m_MenuSettings, wxID_ANY, wxString( wxT("Create CSV File") ) , wxEmptyString, wxITEM_CHECK );
	m_MenuSettings->Append( m_MenuCSVFile );
	
	wxMenuItem* m_MenuVerbose;
	m_MenuVerbose = new wxMenuItem( m_MenuSettings, wxID_ANY, wxString( wxT("Print verbose information") ) , wxEmptyString, wxITEM_CHECK );
	m_MenuSettings->Append( m_MenuVerbose );
	
	m_MenuBar->Append( m_MenuSettings, wxT("Settings") );
	
	m_MenuCamera = new wxMenu();
	wxMenuItem* m_MenuX;
	m_MenuX = new wxMenuItem( m_MenuCamera, wxID_ANY, wxString( wxT("Set Camera on X") ) , wxEmptyString, wxITEM_NORMAL );
	m_MenuCamera->Append( m_MenuX );
	
	wxMenuItem* m_MenuY;
	m_MenuY = new wxMenuItem( m_MenuCamera, wxID_ANY, wxString( wxT("Set Camera on Y") ) , wxEmptyString, wxITEM_NORMAL );
	m_MenuCamera->Append( m_MenuY );
	
	wxMenuItem* m_MenuZ;
	m_MenuZ = new wxMenuItem( m_MenuCamera, wxID_ANY, wxString( wxT("Set Camera on Z") ) , wxEmptyString, wxITEM_NORMAL );
	m_MenuCamera->Append( m_MenuZ );
	
	m_MenuBar->Append( m_MenuCamera, wxT("Camera") );
	
	m_MenuRun = new wxMenu();
	wxMenuItem* m_MenuStart;
	m_MenuStart = new wxMenuItem( m_MenuRun, wxID_ANY, wxString( wxT("Start") ) , wxEmptyString, wxITEM_NORMAL );
	m_MenuRun->Append( m_MenuStart );
	
	wxMenuItem* m_MenuStop;
	m_MenuStop = new wxMenuItem( m_MenuRun, wxID_ANY, wxString( wxT("Stop") ) , wxEmptyString, wxITEM_NORMAL );
	m_MenuRun->Append( m_MenuStop );
	m_MenuStop->Enable( false );
	
	wxMenuItem* m_MenuReset;
	m_MenuReset = new wxMenuItem( m_MenuRun, wxID_ANY, wxString( wxT("Reset") ) , wxEmptyString, wxITEM_NORMAL );
	m_MenuRun->Append( m_MenuReset );
	
	wxMenuItem* m_Separator4;
	m_Separator4 = m_MenuRun->AppendSeparator();
	
	wxMenuItem* m_MenuConnect;
	m_MenuConnect = new wxMenuItem( m_MenuRun, wxID_ANY, wxString( wxT("Connect") ) , wxEmptyString, wxITEM_NORMAL );
	m_MenuRun->Append( m_MenuConnect );
	
	wxMenuItem* m_MenuDisconnect;
	m_MenuDisconnect = new wxMenuItem( m_MenuRun, wxID_ANY, wxString( wxT("Disconnect") ) , wxEmptyString, wxITEM_NORMAL );
	m_MenuRun->Append( m_MenuDisconnect );
	m_MenuDisconnect->Enable( false );
	
	m_MenuBar->Append( m_MenuRun, wxT("Run") );
	
	this->SetMenuBar( m_MenuBar );
	
	wxBoxSizer* m_Sizer;
	m_Sizer = new wxBoxSizer( wxVERTICAL );
	
	m_Panel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_Sizer->Add( m_Panel, 1, wxEXPAND | wxALL, 5 );
	
	this->SetSizer( m_Sizer );
	this->Layout();
	
	// Connect Events
	this->Connect( m_MenuOpen->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( cGUI::OnMenuOpen ) );
	this->Connect( m_MenuSaveAs->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( cGUI::OnMenuSaveAs ) );
	this->Connect( m_MenuClose->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( cGUI::OnMenuClose ) );
	this->Connect( m_MenuQuit->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( cGUI::OnMenuQuit ) );
	this->Connect( m_MenuBackgroundRaster->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( cGUI::OnMenuBackgroundRaster ) );
	this->Connect( m_MenuAxisFrame->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( cGUI::OnMenuAxisFrame ) );
	this->Connect( m_MenuForceInput->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( cGUI::OnMenuForceInput ) );
	this->Connect( m_MenuForce->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( cGUI::OnMenuForce ) );
	this->Connect( m_MenuDots->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( cGUI::OnMenuDots ) );
	this->Connect( m_MenuSpheres->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( cGUI::OnMenuSpheres ) );
	this->Connect( m_MenuCSVFile->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( cGUI::OnMenuCSVFile ) );
	this->Connect( m_MenuVerbose->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( cGUI::OnMenuVerbose ) );
	this->Connect( m_MenuX->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( cGUI::OnMenuX ) );
	this->Connect( m_MenuY->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( cGUI::OnMenuY ) );
	this->Connect( m_MenuZ->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( cGUI::OnMenuZ ) );
	this->Connect( m_MenuStart->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( cGUI::OnMenuStart ) );
	this->Connect( m_MenuStop->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( cGUI::OnMenuStop ) );
	this->Connect( m_MenuReset->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( cGUI::OnMenuReset ) );
	this->Connect( m_MenuConnect->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( cGUI::OnMenuConnect ) );
	this->Connect( m_MenuDisconnect->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( cGUI::OnMenuDisconnect ) );
}

cGUI::~cGUI()
{
	// Disconnect Events
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( cGUI::OnMenuOpen ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( cGUI::OnMenuSaveAs ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( cGUI::OnMenuClose ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( cGUI::OnMenuQuit ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( cGUI::OnMenuBackgroundRaster ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( cGUI::OnMenuAxisFrame ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( cGUI::OnMenuForceInput ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( cGUI::OnMenuForce ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( cGUI::OnMenuDots ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( cGUI::OnMenuSpheres ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( cGUI::OnMenuCSVFile ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( cGUI::OnMenuVerbose ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( cGUI::OnMenuX ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( cGUI::OnMenuY ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( cGUI::OnMenuZ ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( cGUI::OnMenuStart ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( cGUI::OnMenuStop ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( cGUI::OnMenuReset ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( cGUI::OnMenuConnect ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( cGUI::OnMenuDisconnect ) );
}
