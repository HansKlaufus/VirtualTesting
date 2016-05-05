#ifndef CDASINTERFACE_H
#define CDASINTERFACE_H

#include <vector>
#include <wx/socket.h>

#include "scu.h"

class cDasInterface
{
    public:
        cDasInterface();
        virtual ~cDasInterface();

        bool                        IsRunning() { return m_IsRunning; }
        bool                        IsConnected() { return m_SocketClient->IsConnected(); }

        unsigned int                GetNumberChannels() { return m_ChannelNameArray.size(); }
        unsigned int                GetNumberSCU() { return m_ScuArray.size(); }
        void                        AddScu(cScu *p_Scu);

        void                        Connect(wxIPV4address p_Address);
        void                        Disconnect();

        void                        Run();
        void                        Stop();
        void                        GetScanlines();

    protected:

    private:
        static unsigned int         m_NextID;
        unsigned int                m_ID;

        std::vector<cScu*>          m_ScuArray;
        std::vector<std::string>    m_ChannelNameArray;
        std::vector<float>          m_ChannelValueArray;

        wxSocketClient*             m_SocketClient;

        bool                        m_IsRunning;
        bool                        m_Semaphore;

        void                        GetConfiguration();
        void                        ExecuteAction();
        void                        GetIdentification();
        unsigned int                GetNumberScanlines();
        void                        GetScanline();
        void                        GetScanlines(unsigned int p_NumberScanlines);
        void                        GetFirstScanline(unsigned int p_NumberScanlines);
        void                        GetLastScanline(unsigned int p_NumberScanlines);
        void                        SetChannelName();
        void                        SendAnalogueChannels();
};

#endif // CDASINTERFACE_H
