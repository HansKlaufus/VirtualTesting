#ifndef CMAINGUI_H
#define CMAINGUI_H

#include <fstream>
#include <vector>
#include <wx/sckstrm.h>
#include <wx/socket.h>
#include <wx/timer.h>
#include <wx/wx.h>

#include "ac3dobject.h"
#include "dasinterface.h"
#include "glcanvas.h"
#include "gui.h"
#include "memorystream.h"
#include "object.h"
#include "socketclient.h"

#define STATE_TIMER_ID  2000
#define TCP_TIMER_ID    2001
#define DRAW_TIMER_ID   2002

#define SERVER_ADDRESS "10.30.151.232" //"192.168.1.15" //"10.30.151.232"
#define SERVER_PORT    "5963"
#define UDP_ADDRESS    "192.168.1.12"
#define UDP_PORT       "3001"

class cMainGUI : public cGUI
{
    private:
        cGLCanvas            *m_GlPane;
        cObject              *m_Object;
        wxTimer               m_StateTimer;
        wxTimer               m_TcpTimer;
        wxTimer               m_DrawTimer;
        wxStopWatch           m_StopWatch;
        bool                  m_IsRunning;
        bool                  m_IsConnected;
        double                m_Time;
        bool                  m_CreateCSVFile;
        bool                  m_Verbose;
        cMemoryStream         m_OutputFile;

        cDasInterface        *m_DasInterface;

        void Start(void);
        void Stop(void);
        void ResetTime(void);
        void Connect(void);
        void Disconnect(void);

        void ReadAc3dFile(std::string p_FileName);
        void TranslateAc3d(clAc3dObject* p_Ac3d);

        void OnStateTimer(wxTimerEvent &p_Event);
        void OnTcpTimer(wxTimerEvent &p_Event);
        void OnDrawTimer(wxTimerEvent &p_Event);

        void OnMenuOpen(wxCommandEvent &p_Event);
        void OnMenuSaveAs(wxCommandEvent &p_Event);
        void OnMenuClose(wxCommandEvent &p_Event);
        void OnMenuQuit(wxCommandEvent &p_Event);
        void OnMenuBackgroundRaster(wxCommandEvent &p_Event);
        void OnMenuAxisFrame(wxCommandEvent &p_Event);
        void OnMenuForce(wxCommandEvent &p_Event);
        void OnMenuForceInput(wxCommandEvent &p_Event);
        void OnMenuDots(wxCommandEvent &p_Event);
        void OnMenuSpheres(wxCommandEvent &p_Event);
        void OnMenuCSVFile(wxCommandEvent &p_Event);
        void OnMenuVerbose(wxCommandEvent &p_Event);
        void OnMenuStart(wxCommandEvent &p_Event);
        void OnMenuStop(wxCommandEvent &p_Event);
        void OnMenuReset(wxCommandEvent &p_Event);
        void OnMenuConnect(wxCommandEvent &p_Event);
        void OnMenuDisconnect(wxCommandEvent &p_Event);
        void OnMenuX(wxCommandEvent &p_Event);
        void OnMenuY(wxCommandEvent &p_Event);
        void OnMenuZ(wxCommandEvent &p_Event);

    public:
        cMainGUI();
        virtual ~cMainGUI();

        void     SetStatus(wxString p_Message, int p_Field = 0);

        double   GetObjectRadius(void);
        int      GetNumberNumberNodes(void);
        cObject* GetObjectPointer() { return m_Object; }

        DECLARE_EVENT_TABLE()
};

#endif // CMAINGUI_H
