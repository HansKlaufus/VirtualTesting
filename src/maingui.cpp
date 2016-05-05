#include "ac3dobject.h"
#include "connection.h"
#include "fileio.h"
#include "maingui.h"
#include "memorystream.h"
#include "forcenode.h"
#include "object.h"

#include <cstdlib>
#include <wx/wx.h>
#include <wx/filename.h>
#include <wx/sstream.h>

BEGIN_EVENT_TABLE(cMainGUI, wxFrame)
    EVT_TIMER(STATE_TIMER_ID, cMainGUI::OnStateTimer)
    EVT_TIMER(TCP_TIMER_ID, cMainGUI::OnTcpTimer)
    EVT_TIMER(DRAW_TIMER_ID, cMainGUI::OnDrawTimer)
END_EVENT_TABLE()

cMainGUI::cMainGUI() : cGUI(NULL, wxID_ANY, wxT("Virtual Testing App"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE)
{
    // Define the openGL canvas
    int v_Args[] = {WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0};
    m_GlPane = new cGLCanvas(this, v_Args);

    wxBoxSizer* v_Sizer = new wxBoxSizer(wxHORIZONTAL);
    v_Sizer->Add(m_GlPane, 1, wxEXPAND);

    this->SetSizer(v_Sizer);
    this->SetAutoLayout(true);
    this->SetTitle(_("No file opened."));

    m_Object        = NULL;
    m_IsRunning     = false;
    m_IsConnected   = false;
    m_Time          = 0;
    m_CreateCSVFile = false;
    m_Verbose       = false;
    m_DasInterface  = NULL;
}

cMainGUI::~cMainGUI()
{
    delete m_GlPane;
    delete m_Object;

    if(m_DasInterface) delete m_DasInterface;
}

void cMainGUI::SetStatus(wxString p_Message, int p_Field)
{
    this->m_StatusBar->SetStatusText(p_Message, p_Field);
}

void cMainGUI::Start(void)
{
    if(!m_IsRunning)
    {
        m_IsRunning = true;

        // Enable / Disable menu items
        int v_StartId      = m_MenuBar->FindMenuItem(wxT("Run"), wxT("Start"));
        int v_StopId       = m_MenuBar->FindMenuItem(wxT("Run"), wxT("Stop"));
        int v_ResetId      = m_MenuBar->FindMenuItem(wxT("Run"), wxT("Reset"));
        int v_ConnectId    = m_MenuBar->FindMenuItem(wxT("Run"), wxT("Connect"));
        int v_DisconnectId = m_MenuBar->FindMenuItem(wxT("Run"), wxT("Disconnect"));

        m_MenuBar->Enable(v_StartId, false);
        m_MenuBar->Enable(v_StopId, true);
        m_MenuBar->Enable(v_ResetId, false);
        m_MenuBar->Enable(v_ConnectId, false);
        m_MenuBar->Enable(v_DisconnectId, false);

        // Start State calculations
        m_StateTimer.SetOwner(this, STATE_TIMER_ID);
        m_StateTimer.Start(500, false);
        m_StopWatch.Start();

        // Start Das Interface
        if(m_DasInterface)
        {
            m_DasInterface->Run();

            m_TcpTimer.SetOwner(this, TCP_TIMER_ID);
            m_TcpTimer.Start(100);
        }

        // Start GUI update
        m_DrawTimer.SetOwner(this, DRAW_TIMER_ID);
        m_DrawTimer.Start(100);

        SetStatus(wxT("Execution started..."));
    }
}

void cMainGUI::Stop(void)
{
    if(m_IsRunning) {
        // Stop the timers
        m_StateTimer.Stop();
        m_TcpTimer.Stop();
        m_DrawTimer.Stop();

        if(m_DasInterface && m_DasInterface->IsRunning()) m_DasInterface->Stop();

        // Enable / Disable menu items
        int v_StartId      = m_MenuBar->FindMenuItem(wxT("Run"), wxT("Start"));
        int v_StopId       = m_MenuBar->FindMenuItem(wxT("Run"), wxT("Stop"));
        int v_ResetId      = m_MenuBar->FindMenuItem(wxT("Run"), wxT("Reset"));
        int v_ConnectId    = m_MenuBar->FindMenuItem(wxT("Run"), wxT("Connect"));
        int v_DisconnectId = m_MenuBar->FindMenuItem(wxT("Run"), wxT("Disconnect"));

        m_MenuBar->Enable(v_StartId, true);
        m_MenuBar->Enable(v_StopId, false);
        m_MenuBar->Enable(v_ResetId, true);
        m_MenuBar->Enable(v_ConnectId, true);
        m_MenuBar->Enable(v_DisconnectId, true);

        m_IsRunning = false;

        if(m_CreateCSVFile) m_OutputFile.WriteToFile("output.csv");

        SetStatus(wxT("Execution suspended..."));
    }
}

void cMainGUI::ResetTime(void)
{
    m_Time = 0;

    wxString v_Message = wxString::Format(wxT("Time: %.2f"), m_Time);
    this->SetStatus(v_Message, 2);

    m_GlPane->redraw();
}

void cMainGUI::Connect(void)
{
    // Temporarily disable menu items
    int v_StartId      = m_MenuBar->FindMenuItem(wxT("Run"), wxT("Start"));
    int v_StopId       = m_MenuBar->FindMenuItem(wxT("Run"), wxT("Stop"));
    int v_ResetId      = m_MenuBar->FindMenuItem(wxT("Run"), wxT("Reset"));
    int v_ConnectId    = m_MenuBar->FindMenuItem(wxT("Run"), wxT("Connect"));
    int v_DisconnectId = m_MenuBar->FindMenuItem(wxT("Run"), wxT("Disconnect"));

    m_MenuBar->Enable(v_StartId, false);
    m_MenuBar->Enable(v_StopId, false);
    m_MenuBar->Enable(v_ResetId, false);
    m_MenuBar->Enable(v_ConnectId, false);
    m_MenuBar->Enable(v_DisconnectId, false);

    // Establish a TCP connection
    wxIPV4address v_ServerAddress;
    v_ServerAddress.Hostname(_(SERVER_ADDRESS));
    v_ServerAddress.Service(_(SERVER_PORT));

    m_DasInterface = new cDasInterface();
    m_DasInterface->Connect(v_ServerAddress);

    m_IsConnected = true;

    // Enable / Disable menu items
    m_MenuBar->Enable(v_StartId, true);
    m_MenuBar->Enable(v_StopId, false);
    m_MenuBar->Enable(v_ResetId, true);
    m_MenuBar->Enable(v_ConnectId, false);
    m_MenuBar->Enable(v_DisconnectId, true);
}

void cMainGUI::Disconnect(void)
{
    if(m_DasInterface) delete m_DasInterface;

    // Enable / Disable menu items
    int v_StartId      = m_MenuBar->FindMenuItem(wxT("Run"), wxT("Start"));
    int v_StopId       = m_MenuBar->FindMenuItem(wxT("Run"), wxT("Stop"));
    int v_ResetId      = m_MenuBar->FindMenuItem(wxT("Run"), wxT("Reset"));
    int v_ConnectId    = m_MenuBar->FindMenuItem(wxT("Run"), wxT("Connect"));
    int v_DisconnectId = m_MenuBar->FindMenuItem(wxT("Run"), wxT("Disconnect"));

    m_MenuBar->Enable(v_StartId, !m_IsRunning);
    m_MenuBar->Enable(v_StopId, m_IsRunning);
    m_MenuBar->Enable(v_ResetId, !m_IsRunning);
    m_MenuBar->Enable(v_ConnectId, true);
    m_MenuBar->Enable(v_DisconnectId, false);

    m_IsConnected = false;
}

void cMainGUI::OnMenuOpen(wxCommandEvent &p_Event)
{
	wxFileDialog* v_OpenDialog = new wxFileDialog(this, _("Open File"), wxEmptyString, wxEmptyString, _("Configuration XML Files (*.xml)|*.xml|Configuration Files (*.cfg)|*.cfg|AC3D Files (*.ac)|*.ac|All files (*.*)|*.*"), wxFD_OPEN, wxDefaultPosition);

	if(v_OpenDialog->ShowModal() == wxID_OK)
	{
		wxFileName v_FileName(v_OpenDialog->GetPath());

		if(m_Object)
		{
		    delete m_Object;
            m_Object = NULL;
		}
        m_Object = new cObject();

        if(v_FileName.GetExt() == _("cfg"))
        {
            m_Object->ReadConfiguration(std::string(v_FileName.GetFullPath().mb_str()));
        }
        else if(v_FileName.GetExt() == _("ac"))
        {
            this->ReadAc3dFile(std::string(v_FileName.GetFullPath().mb_str()));
        }
        else if(v_FileName.GetExt() == _("xml"))
        {
            cFileIo v_FileIO(m_Object);
            v_FileIO.XmlReadFile(std::string(v_FileName.GetFullPath().mb_str()));
        }
        else
        {
            std::cout << "ERROR in cMainGUI::OnMenuOpen: file extension not recognised: " << v_FileName.GetExt() << std::endl;
        }
        m_Object->Initialise();

        double v_Radius = this->GetObjectRadius();
        m_GlPane->setRadius(1.5*v_Radius);
        m_GlPane->setCameraPosition(2*v_Radius, 0, 0, 0, 0, 1);
        m_GlPane->redraw();

		this->ResetTime();
		this->SetTitle(v_OpenDialog->GetFilename());
		this->SetStatus(_("Ready..."));

		std::cout << std::endl;
		std::cout << "------------------------ OPENED NEW FILE ------------------------" << std::endl;
	}
	v_OpenDialog->Destroy();
}

void cMainGUI::OnMenuSaveAs(wxCommandEvent &p_Event)
{
	wxFileDialog* v_SaveDialog = new wxFileDialog(this, _("Save File"), wxEmptyString, wxEmptyString, _("Configuration XML Files (*.xml)|*.xml|Configuration Files (*.cfg)|*.cfg|AC3D Files (*.ac)|*.ac|All files (*.*)|*.*"), wxFD_SAVE, wxDefaultPosition);

	if(v_SaveDialog->ShowModal() == wxID_OK)
	{
		wxFileName v_FileName(v_SaveDialog->GetPath());

		if(m_Object)
		{
            if(v_FileName.GetExt() == _("xml"))
            {
                cFileIo v_FileIo(m_Object);
                v_FileIo.XmlWriteFile(std::string(v_FileName.GetFullPath().mb_str()));
            }
            else
            {
                std::cout << "ERROR in cMainGUI::OnMenuSaveAs: file extension not recognised: " << v_FileName.GetExt() << std::endl;
            }
		}

		this->SetTitle(v_SaveDialog->GetFilename());
		this->SetStatus(_("Ready..."));
	}
	v_SaveDialog->Destroy();
}

void cMainGUI::OnMenuClose(wxCommandEvent &p_Event)
{
    this->Stop();

    if(m_Object)
    {
        delete m_Object;
        m_Object = NULL;

		this->ResetTime();
		this->SetTitle(_("No file opened."));

        std::cout << "------------------------ FILE CLOSED ------------------------" << std::endl;

    }

    m_GlPane->redraw();
}

void cMainGUI::OnMenuQuit(wxCommandEvent &p_Event)
{
    this->Stop();
    this->Close(true);
}

void cMainGUI::OnMenuBackgroundRaster(wxCommandEvent &p_Event)
{
    int v_MenuId = m_MenuBar->FindMenuItem(wxT("Settings"), wxT("Show Background Raster"));

    bool v_Show = !m_GlPane->hasBackgroundRaster();
    m_GlPane->showBackgroundRaster(v_Show);
    m_MenuBar->Check(v_MenuId, v_Show);

    Refresh(false);
}

void cMainGUI::OnMenuAxisFrame(wxCommandEvent &p_Event)
{
    int v_MenuId = m_MenuBar->FindMenuItem(wxT("Settings"), wxT("Show Axis Frame"));

    bool v_Show = !m_GlPane->hasAxisFrame();
    m_GlPane->showAxisFrame(v_Show);
    m_MenuBar->Check(v_MenuId, v_Show);

    Refresh(false);
}

void cMainGUI::OnMenuForce(wxCommandEvent &p_Event)
{
    int v_MenuId = m_MenuBar->FindMenuItem(wxT("Settings"), wxT("Show Force Vectors"));

    bool v_Show = !m_GlPane->hasForceVectors();
    m_GlPane->showForceVectors(v_Show);
    m_MenuBar->Check(v_MenuId, v_Show);

    Refresh(false);
}

void cMainGUI::OnMenuForceInput(wxCommandEvent &p_Event)
{
    int v_MenuId = m_MenuBar->FindMenuItem(wxT("Settings"), wxT("Show Force Input Vectors"));

    bool v_Show = !m_GlPane->hasForceInputVectors();
    m_GlPane->showForceInputVectors(v_Show);
    m_MenuBar->Check(v_MenuId, v_Show);

    Refresh(false);
}

void cMainGUI::OnMenuDots(wxCommandEvent &p_Event)
{
    int v_MenuId = m_MenuBar->FindMenuItem(wxT("Settings"), wxT("Connect the dots"));

    bool v_ShowSpheres = false;
    m_GlPane->showSpheres(v_ShowSpheres);
    m_MenuBar->Check(v_MenuId, !v_ShowSpheres);

    Refresh(false);
}

void cMainGUI::OnMenuSpheres(wxCommandEvent &p_Event)
{
    int v_MenuId = m_MenuBar->FindMenuItem(wxT("Settings"), wxT("Balance the balls"));

    bool v_ShowSpheres = true;
    m_GlPane->showSpheres(v_ShowSpheres);
    m_MenuBar->Check(v_MenuId, v_ShowSpheres);

    Refresh(false);
}

void cMainGUI::OnMenuCSVFile(wxCommandEvent &p_Event)
{
    int v_MenuId = m_MenuBar->FindMenuItem(wxT("Settings"), wxT("Create CSV File"));

    m_CreateCSVFile = !m_CreateCSVFile;
    m_MenuBar->Check(v_MenuId, m_CreateCSVFile);

    this->Refresh(false);
}

void cMainGUI::OnMenuVerbose(wxCommandEvent &p_Event)
{
    int v_MenuId = m_MenuBar->FindMenuItem(wxT("Settings"), wxT("Print verbose information"));

    m_Verbose = !m_Verbose;
    m_MenuBar->Check(v_MenuId, m_Verbose);

    this->Refresh(false);
}

void cMainGUI::OnMenuStart(wxCommandEvent &p_Event)
{
    this->Start();
}

void cMainGUI::OnMenuStop(wxCommandEvent &p_Event)
{
    this->Stop();
}

void cMainGUI::OnMenuReset(wxCommandEvent &p_Event)
{
    this->ResetTime();
}

void cMainGUI::OnMenuX(wxCommandEvent &p_Event)
{
    m_GlPane->setCameraPosition(2*this->GetObjectRadius(), 0, 0, 0, 0, 1);
    m_GlPane->setCameraAngle(0, 0, 0);
    m_GlPane->redraw();
}

void cMainGUI::OnMenuY(wxCommandEvent &p_Event)
{
    m_GlPane->setCameraPosition(0, 2*this->GetObjectRadius(), 0, 0, 0, 1);
    m_GlPane->setCameraAngle(0, 0, 0);
    m_GlPane->redraw();
}

void cMainGUI::OnMenuZ(wxCommandEvent &p_Event)
{
    m_GlPane->setCameraPosition(0, 0, 2*this->GetObjectRadius(), 0, 1, 0);
    m_GlPane->setCameraAngle(0, 0, 0);
    m_GlPane->redraw();
}

void cMainGUI::OnStateTimer(wxTimerEvent &p_Event)
{
    // Get true elapsed time
    double v_ElapsedTime = m_StopWatch.Time();
    m_StopWatch.Start();

    // If elapsed time > timer interval + 10%, then slow down timer interval
    double v_TimeStep = p_Event.GetInterval();
    if(v_ElapsedTime > 1.1*v_TimeStep)
    {
        bool v_TimerStarted = m_StateTimer.Start(v_TimeStep*2);
        if(not v_TimerStarted)
        {
            std::cout << "ERROR in cMainGUI::OnStateTimer: timer not started." << std::endl;
        } else {
            std::cout << "WARNING in cMainGUI::OnStateTimer: slowing down timer: " << v_TimeStep << "|" << v_ElapsedTime << std::endl;
        }
    } else if(v_ElapsedTime < 0.9*v_TimeStep)
    {
        bool v_TimerStarted = m_StateTimer.Start(v_TimeStep/2);
        if(not v_TimerStarted)
        {
            std::cout << "ERROR in cMainGUI::OnStateTimer: timer not started." << std::endl;
        } else {
            std::cout << "WARNING in cMainGUI::OnStateTimer: speeding up timer: " << v_TimeStep << "|" << v_ElapsedTime << std::endl;
        }
    }


    // Calculate new time
    v_ElapsedTime = 0.1; //= 1000.0;
    m_Time += v_ElapsedTime;

    // Calculate new object state
    if(m_Object) m_Object->CalculateState(v_ElapsedTime);
}

void cMainGUI::OnTcpTimer(wxTimerEvent &p_Event)
{
    // Process IO
    if(m_DasInterface)  m_DasInterface->GetScanlines();
}

void cMainGUI::OnDrawTimer(wxTimerEvent &p_Event)
{
    // Update GUI
    wxString v_Message = wxString::Format(wxT("Time: %.2f"), m_Time);
    this->SetStatus(v_Message, 2);
    m_GlPane->redraw();

    // Print current state
    if(m_Verbose) m_Object->PrintState();
    m_Object->WriteToFile(m_OutputFile);
}

double cMainGUI::GetObjectRadius()
{
    double v_Radius = 10;
    if(m_Object)
    {
        v_Radius = m_Object->GetRadius();
    }

    return v_Radius;
}

void cMainGUI::ReadAc3dFile(std::string p_FileName)
{
    clAc3dObject *v_Ac3d = new clAc3dObject("Test Object");
    v_Ac3d->ReadAC3DFile(p_FileName);

    this->TranslateAc3d(v_Ac3d);

    std::cout << "Nodes: " << m_Object->GetNumberNodes() << std::endl;
    std::cout << "Conections: " << m_Object->GetNumberConnections() << std::endl;

    m_Object->RemoveDuplicateConnections();

    std::cout << "Nodes: " << m_Object->GetNumberNodes() << std::endl;
    std::cout << "Conections: " << m_Object->GetNumberConnections() << std::endl;

    delete v_Ac3d;
}

void cMainGUI::TranslateAc3d(clAc3dObject* p_Ac3d)
{
    // Create a lookup table
    std::vector<unsigned int> v_VertexId;

    // Create Nodes out of Vertices
    unsigned int v_NumberVertices = p_Ac3d->GetNumberVertices(false);
    for(unsigned int v_Index=0; v_Index<v_NumberVertices; v_Index++)
    {
        std::cout << "Vertex: " << v_Index << " / " << v_NumberVertices << std::endl;

        cVector* v_Vertex = p_Ac3d->GetVertexPointer(v_Index);

        // Create Node and add to Object
        cForceNode* v_Node = new cForceNode(m_Object, 1, v_Vertex->at(0), v_Vertex->at(1), v_Vertex->at(2));
        m_Object->AddNode(v_Node);

        // Add Object ID to translation vector
        v_VertexId.push_back(v_Node->GetID());
    }

    // Create Connections out of Surfaces
    unsigned int v_NumberSurfaces = p_Ac3d->GetNumberSurfaces(false);
    for(unsigned int v_Index=0; v_Index<v_NumberSurfaces; v_Index++)
    {
        std::cout << "Surface: " << v_Index << " / " << v_NumberSurfaces << std::endl;

        clAc3dSurface* v_Surface = p_Ac3d->GetSurfacePointer(v_Index);

        unsigned int v_NumberSurfaceVertices = v_Surface->GetNumberVertices();
        for(unsigned int v_Index2=0; v_Index2<v_NumberSurfaceVertices; v_Index2++)
        {
            unsigned int v_Node1Idx = v_Surface->GetVertexIndex(v_Index2);
            unsigned int v_Node2Idx = (v_Index2==v_NumberSurfaceVertices-1 ? v_Surface->GetVertexIndex(0) : v_Surface->GetVertexIndex(v_Index2+1));

            cConnection* v_Connnection = new cConnection(m_Object, 1, 1);
            v_Connnection->SetNodeId1(v_VertexId.at(v_Node1Idx));
            v_Connnection->SetNodeId2(v_VertexId.at(v_Node2Idx));

            m_Object->AddConnection(v_Connnection);

        }
    }

    // Recurse through kids
    unsigned int v_NumberKids = p_Ac3d->GetNumberKids(false);
    for(unsigned int v_Index=0; v_Index<v_NumberKids; v_Index++)
    {
        std::cout << "Kid: " << v_Index << " / " << v_NumberKids << std::endl;

        clAc3dObject* kid = p_Ac3d->GetKidPointer(v_Index);
        this->TranslateAc3d(kid);
    }
}

void cMainGUI::OnMenuConnect(wxCommandEvent &p_Event)
{
    this->Connect();
}

void cMainGUI::OnMenuDisconnect(wxCommandEvent &p_Event)
{
    this->Disconnect();
}
