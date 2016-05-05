#include <cstddef>
#include <iostream>
#include <wx/sstream.h>
#include <wx/xml/xml.h>

#include "dasinterface.h"

unsigned int cDasInterface::m_NextID = 0;

cDasInterface::cDasInterface()
{
    m_ID = cDasInterface::m_NextID++;

    m_IsRunning = false;
    m_Semaphore = false;

    m_SocketClient = new wxSocketClient();
    m_SocketClient->SetFlags(wxSOCKET_WAITALL);
}

cDasInterface::~cDasInterface()
{
    if(this->IsConnected()) this->Disconnect();
    if(m_SocketClient) m_SocketClient->Destroy();
}

void cDasInterface::Connect(wxIPV4address p_ServerAddress)
{
    m_SocketClient->Connect(p_ServerAddress, false);
    m_SocketClient->WaitOnConnect(10);

    if(m_SocketClient->IsConnected())
    {
        std::cout << "INFO in cDasInterface::Connect: Connection to server succesful." << std::endl;

        this->GetIdentification();
        this->GetConfiguration();
    }
    else
    {
        m_SocketClient->Close();
        std::cout << "ERROR in cDasInterface::Connect: Connection to server failed." << std::endl;
    }
}

void cDasInterface::Disconnect()
{
    if(m_IsRunning) this->Stop();
    m_SocketClient->Close();

    std::cout << "INFO in cDasInterface::Disconnect: Disconnected from server." << std::endl;
}

void cDasInterface::GetIdentification()
{
    char         v_Request[] = "IDN?\n";
    char        *v_Answer    = NULL;
    unsigned int v_Length    = strlen(v_Request);

    m_SocketClient->Write(v_Request, v_Length*sizeof(char));
    if(!m_SocketClient->Error())
    {
        // Read answer code
        delete [] v_Answer;
        v_Length = 4;
        v_Answer = new char[v_Length+1];
        v_Answer[v_Length]='\0';

        m_SocketClient->Read(v_Answer, v_Length*sizeof(char));
        if(!m_SocketClient->Error())
        {
            std::string v_Line(v_Answer);

            if(v_Line.compare("ID#0") != 0)
            {
                std::cout << "ERROR in cDasInterface::RequestIdentification: Server returned invalid answer: " << v_Line << std::endl;

                // Discard remainder of string
                delete [] v_Answer;
                v_Length = 2;
                v_Answer = new char[v_Length+1];
                v_Answer[v_Length] = '\0';

                m_SocketClient->Read(v_Answer, v_Length*sizeof(char));
                if(m_SocketClient->Error())
                {
                    std::cout << "ERROR in cDasInterface::GetIdentification: socket error: " << m_SocketClient->LastError() << std::endl;
                }
            }
            else
            {
                // Read string length
                v_Length = 0;

                m_SocketClient->Read(&v_Length, sizeof(int));

                if(!m_SocketClient->Error())
                {
                    // Change Byte Order from Big Endian to Little Endian if on LE machine
                    v_Length = wxINT32_SWAP_ON_LE(v_Length);

                    // Read the identification string
                    delete [] v_Answer;
                    v_Answer = new char[v_Length+1];
                    v_Answer[v_Length] = '\0';

                    m_SocketClient->Read(v_Answer, v_Length*sizeof(char));
                    if(!m_SocketClient->Error())
                    {
                        std::string v_IdString(v_Answer);
                        std::cout << "INFO in cDasInterface::GetIdentification: Server name: " << v_IdString << std::endl;

                        // Discard remainder of string
                        delete [] v_Answer;
                        v_Length = 2;
                        v_Answer = new char[v_Length+1];
                        v_Answer[v_Length] = '\0';

                        m_SocketClient->Read(v_Answer, v_Length*sizeof(char));
                        if(m_SocketClient->Error())
                        {
                            std::cout << "ERROR in cDasInterface::GetIdentification: socket error: " << m_SocketClient->LastError() << std::endl;
                        }
                    }
                    else
                    {
                        std::cout << "ERROR in cDasInterface::GetIdentification: socket error: " << m_SocketClient->LastError() << std::endl;
                    }
                }
                else
                {
                    std::cout << "ERROR in cDasInterface::GetIdentification: socket error: " << m_SocketClient->LastError() << std::endl;
                }
            }
        }
        else
        {
            std::cout << "ERROR in cDasInterface::GetIdentification: socket error: " << m_SocketClient->LastError() << std::endl;
        }
    }
    else
    {
        std::cout << "ERROR in cDasInterface::GetIdentification: socket error: " << m_SocketClient->LastError() << std::endl;
    }

    delete [] v_Answer;
}

void cDasInterface::GetConfiguration()
{
    char         v_Request[] = "CFG?\n";
    char        *v_Answer    = NULL;
    unsigned int v_Length    = strlen(v_Request);

    m_SocketClient->Write(v_Request, v_Length*sizeof(char));
    if(!m_SocketClient->Error())
    {
        // Read answer code
        delete [] v_Answer;
        v_Length = 4;
        v_Answer = new char[v_Length+1];
        v_Answer[v_Length]='\0';

        m_SocketClient->Read(v_Answer, v_Length*sizeof(char));
        if(!m_SocketClient->Error())
        {
            std::string v_Line(v_Answer);

            if(v_Line.compare("SC#0") != 0)
            {
                std::cout << "ERROR in cDasInterface::GetConfiguration: Server returned invalid answer: " << v_Line << std::endl;

                // Discard remainder of string
                delete [] v_Answer;
                v_Length = 2;
                v_Answer = new char[v_Length+1];
                v_Answer[v_Length] = '\0';

                m_SocketClient->Read(v_Answer, v_Length*sizeof(char));
                if(m_SocketClient->Error())
                {
                    std::cout << "ERROR in cDasInterface::GetConfiguration: socket error: " << m_SocketClient->LastError() << std::endl;
                }
            }
            else
            {
                // Read string length
                v_Length = 0;

                m_SocketClient->Read(&v_Length, sizeof(int));

                if(!m_SocketClient->Error())
                {
                    // Change Byte Order from Big Endian to Little Endian if on LE machine
                    v_Length = wxINT32_SWAP_ON_LE(v_Length);

                    // Read the configuration string
                    delete [] v_Answer;
                    v_Answer = new char[v_Length+1];
                    v_Answer[v_Length] = '\0';

                    m_SocketClient->Read(v_Answer, v_Length*sizeof(char));
                    if(!m_SocketClient->Error())
                    {
                        std::string v_ConfigurationString(v_Answer);
//                        std::cout << "Configuration: " << v_ConfigurationString << std::endl;

                        wxString            v_XmlString = wxString(v_ConfigurationString.c_str(), wxConvUTF8);
                        wxStringInputStream v_InputStream(v_XmlString);
                        wxXmlDocument       v_XmlDocument(v_InputStream);
                        wxXmlNode          *v_Child = v_XmlDocument.GetRoot()->GetChildren();

                        while(v_Child)
                        {
                            if(v_Child->GetName() == wxT("channel"))
                            {
                                wxString v_ChannelName = v_Child->GetAttribute(wxT("name"), wxT("noName"));
                                m_ChannelNameArray.push_back(std::string(v_ChannelName.mb_str()));
                                m_ChannelValueArray.push_back(0.0f);
                            }

                            v_Child = v_Child->GetNext();
                        }

                        // Discard remainder of string
                        delete [] v_Answer;
                        v_Length = 2;
                        v_Answer = new char[v_Length+1];
                        v_Answer[v_Length] = '\0';

                        m_SocketClient->Read(v_Answer, v_Length*sizeof(char));
                        if(m_SocketClient->Error())
                        {
                            std::cout << "ERROR in cDasInterface::GetConfiguration." << std::endl;
                        }
                    }
                    else
                    {
                        std::cout << "ERROR in cDasInterface::GetConfiguration: socket error: " << m_SocketClient->LastError() << std::endl;
                    }
                }
                else
                {
                    std::cout << "ERROR in cDasInterface::GetConfiguration: socket error: " << m_SocketClient->LastError() << std::endl;
                }
            }
        }
        else
        {
            std::cout << "ERROR in cDasInterface::GetConfiguration: socket error: " << m_SocketClient->LastError() << std::endl;
        }
    }
    else
    {
        std::cout << "ERROR in cDasInterface::GetConfiguration: socket error: " << m_SocketClient->LastError() << std::endl;
    }

    delete [] v_Answer;
}

void cDasInterface::Run()
{
    std::string v_RunCommand = "RUN ";
    for(unsigned int v_Index=0; v_Index<this->GetNumberChannels()-1; v_Index++)
    {
        v_RunCommand += m_ChannelNameArray.at(v_Index);
        v_RunCommand += ", ";
    }
    v_RunCommand += m_ChannelNameArray.at(this->GetNumberChannels()-1);
    v_RunCommand += "\n";

    char         *v_Request = strdup(v_RunCommand.c_str());
    char         *v_Answer  = NULL;
    unsigned int  v_Length  = strlen(v_Request);

    m_SocketClient->Write(v_Request, v_Length*sizeof(char));
    if(!m_SocketClient->Error())
    {
        delete [] v_Answer;
        v_Length           = 4;
        v_Answer           = new char[v_Length+1];
        v_Answer[v_Length] = '\0';

        m_SocketClient->Read(v_Answer, v_Length*sizeof(char));

        if(!m_SocketClient->Error())
        {
            std::string v_Line(v_Answer);

            if(v_Line.compare("RV#0") != 0)
            {
                std::cout << "ERROR in cDasInterface::Run: Server returned invalid answer: " << v_Line << std::endl;

                // Discard remainder of string
                delete [] v_Answer;
                v_Length = 2;
                v_Answer = new char[v_Length+1];
                v_Answer[v_Length] = '\0';

                m_SocketClient->Read(v_Answer, v_Length*sizeof(char));
                if(m_SocketClient->Error())
                {
                    std::cout << "ERROR in cDasInterface::Run: socket error: " << m_SocketClient->LastError() << std::endl;
                }
            }
            else
            {
                m_IsRunning = true;
                std::cout << "INFO in cDasInterface::Run: execution started." << std::endl;

                // Discard remainder of string
                delete [] v_Answer;
                v_Length = 2;
                v_Answer = new char[v_Length+1];
                v_Answer[v_Length] = '\0';

                m_SocketClient->Read(v_Answer, v_Length*sizeof(char));
                if(m_SocketClient->Error())
                {
                    std::cout << "ERROR in cDasInterface::Run: socket error: " << m_SocketClient->LastError() << std::endl;
                }
            }
        }
        else
        {
            std::cout << "ERROR in cDasInterface::Run: socket error: " << m_SocketClient->LastError() << std::endl;
        }

        delete [] v_Answer;
    }
    else
    {
        std::cout << "ERROR in cDasInterface::Run: Socket error: " << m_SocketClient->LastError() << std::endl;
    }
}

unsigned int cDasInterface::GetNumberScanlines()
{
    unsigned int v_NumberScanlines = 0;

    // Request the number of scanlines in LCS FIFO
    char         v_Request[] = "OMP?\n";
    unsigned int v_Length = strlen(v_Request);
    char        *v_Answer = NULL;

    // Write OMP command
    m_SocketClient->Write(v_Request, v_Length*sizeof(char));
    if(!m_SocketClient->Error())
    {
        // Read answer code
        delete [] v_Answer;
        v_Length            = 4;
        v_Answer            = new char[v_Length+1];
        v_Answer[v_Length]  = '\0';

        m_SocketClient->Read(v_Answer, v_Length*sizeof(char));
        if(!m_SocketClient->Error())
        {
            std::string v_Line(v_Answer);

            if(v_Line.compare("NS#0") != 0)
            {
                std::cout << "ERROR in cDasInterface::GetNumberScanlines: server returned invalid answer: " << v_Line << std::endl;

                // Discard remainder of string
                delete [] v_Answer;
                v_Length = 2;
                v_Answer = new char[v_Length+1];
                v_Answer[v_Length] = '\0';

                m_SocketClient->Read(v_Answer, v_Length*sizeof(char));
                if(m_SocketClient->Error())
                {
                    std::cout << "ERROR in cDasInterface::GetNumberScanlines: socket error: " << m_SocketClient->LastError() << std::endl;
                }
            }
            else
            {
                // Read number of scanlines
                m_SocketClient->Read(&v_NumberScanlines, sizeof(int));

                if(!m_SocketClient->Error())
                {
                    // Change Byte Order from Big Endian to Little Endian if on LE machine
                    v_NumberScanlines = wxINT32_SWAP_ON_LE(v_NumberScanlines);

                    std::cout << "INFO in cDasInterface::GetNumberScanlines: Requesting " << v_NumberScanlines << " scanlines." << std::endl;

                    // Discard remainder of string
                    delete [] v_Answer;
                    v_Length = 2;
                    v_Answer = new char[v_Length+1];
                    v_Answer[v_Length] = '\0';

                    m_SocketClient->Read(v_Answer, v_Length*sizeof(char));
                    if(m_SocketClient->Error())
                    {
                        std::cout << "ERROR in cDasInterface::GetNumberScanlines: socket error: " << m_SocketClient->LastError() << std::endl;
                    }
                }
                else
                {
                    std::cout << "ERROR in cDasInterface::GetNumberScanlines: socket error: " << m_SocketClient->LastError() << std::endl;
                }
            }
        }
        else
        {
            std::cout << "ERROR in cDasInterface::GetNumberScanlines: socket error: " << m_SocketClient->LastError() << std::endl;
        }
    }
    else
    {
        std::cout << "ERROR in cDasInterface::GetNumberScanlines: socket error: " << m_SocketClient->LastError() << std::endl;
    }

    delete [] v_Answer;

    return v_NumberScanlines;
}

void cDasInterface::GetScanline()
{
    // Read answer code
    unsigned int  v_Length = 4;
    char         *v_Answer = new char[v_Length+1];
    v_Answer[v_Length]     = '\0';

    m_SocketClient->Read(v_Answer, v_Length);
    if(!m_SocketClient->Error())
    {
        std::string v_Line(v_Answer);

        if(v_Line.compare("SL#0") != 0)
        {
            std::cout << "ERROR in cDasInterface::GetScanline: Server returned invalid answer: " << v_Line << std::endl;

            // Discard remainder of string
            delete [] v_Answer;
            v_Length = 4;                       // Scanlines are terminated by <LF><CR><LF><CR>
            v_Answer = new char[v_Length+1];
            v_Answer[v_Length] = '\0';

            m_SocketClient->Read(v_Answer, v_Length*sizeof(char));
            if(m_SocketClient->Error())
            {
                std::cout << "ERROR in cDasInterface::GetScanline: socket error: " << m_SocketClient->LastError() << std::endl;
            }
        }
        else
        {
            // Read timestamp
            double v_Timestamp = 0.0f;

            m_SocketClient->Read(&v_Timestamp, sizeof(double));
            if(!m_SocketClient->Error())
            {
                // Read command code
                unsigned long v_Command = 0;

                m_SocketClient->Read(&v_Command, sizeof(long));
                if(!m_SocketClient->Error())
                {
                    // Read channel values
                    for(unsigned int v_Index=0; v_Index<this->GetNumberChannels(); v_Index++)
                    {
                        float v_Value = 0.0f;

                        m_SocketClient->Read(&v_Value, sizeof(float));
                        if(!m_SocketClient->Error())
                        {
                            // Store value
                            m_ChannelValueArray.at(v_Index) = v_Value;
                        }
                        else
                        {
                            std::cout << "ERROR in cDasInterface::GetScanline: socket error: " << m_SocketClient->LastError() << std::endl;
                        }
                    }

                    // Discard remainder of string
                    delete [] v_Answer;
                    v_Length = 4;                       // Scanlines are terminated by <LF><CR><LF><CR>
                    v_Answer = new char[v_Length+1];
                    v_Answer[v_Length] = '\0';

                    m_SocketClient->Read(v_Answer, v_Length*sizeof(char));
                    if(m_SocketClient->Error())
                    {
                        std::cout << "ERROR in cDasInterface::GetScanline: socket error: " << m_SocketClient->LastError() << std::endl;
                    }
                }
                else
                {
                    std::cout << "ERROR in cDasInterface::GetScanline: socket error: " << m_SocketClient->LastError() << std::endl;
                }
            }
            else
            {
                std::cout << "ERROR in cDasInterface::GetScanline: socket error: " << m_SocketClient->LastError() << std::endl;
            }
        }
    }
    else
    {
        std::cout << "ERROR in cDasInterface::GetScanline: socket error: " << m_SocketClient->LastError() << std::endl;
    }
}

void cDasInterface::GetScanlines(unsigned int p_NumberScanlines)
{
    // Request all scanlines
    std::stringstream v_StringStream;
    v_StringStream << "RMB? " << p_NumberScanlines << "\n";
    std::string v_ScanlineCommand = v_StringStream.str();

    char        *v_Request = strdup(v_ScanlineCommand.c_str());
    char        *v_Answer  = NULL;
    unsigned int v_Length  = strlen(v_Request);

    // Write RMB command
    m_SocketClient->Write(v_Request, v_Length*sizeof(char));
    if(!m_SocketClient->Error())
    {
        for(unsigned int v_Index=0; v_Index<p_NumberScanlines; v_Index++)
        {
            this->GetScanline();
        }
    }
    else
    {
        std::cout << "ERROR in cDasInterface::GetScanlines: socket error: " << m_SocketClient->LastError() << std::endl;
    }

    delete [] v_Answer;
}

void cDasInterface::GetFirstScanline(unsigned int p_NumberScanlines)
{
    // Request the last scanline
    char         v_Request[] = "RMB? 1\n";
    unsigned int v_Length    = strlen(v_Request);
    char        *v_Answer    = NULL;

    // Write RMB command
    m_SocketClient->Write(v_Request, v_Length*sizeof(char));
    if(!m_SocketClient->Error())
    {
        this->GetScanline();
    }
    else
    {
        std::cout << "ERROR in cDasInterface::GetLastScanline: socket error: " << m_SocketClient->LastError() << std::endl;
    }

    delete [] v_Answer;

    if(p_NumberScanlines>1)
    {
        // Request all but the last scanlines
        std::stringstream v_StringStream;
        v_StringStream << "RMB? " << p_NumberScanlines-1 << "\n";

        std::string  v_Request = v_StringStream.str();
        unsigned int v_Length  = v_Request.size();

        // Write RMB command
        m_SocketClient->Write(v_Request.c_str(), v_Length*sizeof(char));
        if(!m_SocketClient->Error())
        {
            unsigned int v_NumberChannels = this->GetNumberChannels();
            unsigned int v_Length         = 4+8+4+v_NumberChannels*4+4;

            for(unsigned int v_Index=0; v_Index<p_NumberScanlines-1; v_Index++)
            {
                char *v_Answer = new char[v_Length];

                m_SocketClient->Read(v_Answer, v_Length);
                if(m_SocketClient->Error())
                {
                    std::cout << "ERROR in cDasInterface::GetLastScanline: socket error: " << m_SocketClient->LastError() << std::endl;
                }

                delete [] v_Answer;
            }
        }
        else
        {
            std::cout << "ERROR in cDasInterface::GetLastScanline: socket error: " << m_SocketClient->LastError() << std::endl;
        }
    }
}

void cDasInterface::GetLastScanline(unsigned int p_NumberScanlines)
{
    if(p_NumberScanlines>1)
    {
        // Request all but the last scanlines
        std::stringstream v_StringStream;
        v_StringStream << "RMB? " << p_NumberScanlines-1 << "\n";

        std::string  v_Request = v_StringStream.str();
        unsigned int v_Length  = v_Request.size();

        // Write RMB command
        m_SocketClient->Write(v_Request.c_str(), v_Length*sizeof(char));
        if(!m_SocketClient->Error())
        {
            unsigned int v_NumberChannels = this->GetNumberChannels();
            unsigned int v_Length         = 4+8+4+v_NumberChannels*4+4;

            for(unsigned int v_Index=0; v_Index<p_NumberScanlines-1; v_Index++)
            {
                char *v_Answer = new char[v_Length];

                m_SocketClient->Read(v_Answer, v_Length);
                if(m_SocketClient->Error())
                {
                    std::cout << "ERROR in cDasInterface::GetLastScanline: socket error: " << m_SocketClient->LastError() << std::endl;
                }

                delete [] v_Answer;
            }
        }
        else
        {
            std::cout << "ERROR in cDasInterface::GetLastScanline: socket error: " << m_SocketClient->LastError() << std::endl;
        }
    }

    // Request the last scanline
    char         v_Request[] = "RMB? 1\n";
    unsigned int v_Length    = strlen(v_Request);
    char        *v_Answer    = NULL;

    // Write RMB command
    m_SocketClient->Write(v_Request, v_Length*sizeof(char));
    if(!m_SocketClient->Error())
    {
        this->GetScanline();
    }
    else
    {
        std::cout << "ERROR in cDasInterface::GetLastScanline: socket error: " << m_SocketClient->LastError() << std::endl;
    }

    delete [] v_Answer;
}

void cDasInterface::GetScanlines()
{
    if(!m_Semaphore)
    {
        m_Semaphore = true;

        unsigned int v_NumberScanlines = this->GetNumberScanlines();
        this->GetLastScanline(v_NumberScanlines);

        m_Semaphore = false;
    }
    else
    {
        std::cout << "WARNING in cDasInterface::GetScanlines(): Semaphore active." << std::endl;
    }
}

void cDasInterface::Stop()
{
    char          v_Request[] = "STOP\n";
    char         *v_Answer    = NULL;
    unsigned int  v_Length    = strlen(v_Request);

    m_SocketClient->Write(v_Request, v_Length*sizeof(char));
    if(!m_SocketClient->Error())
    {
        delete [] v_Answer;
        v_Length           = 4;
        v_Answer           = new char[v_Length+1];
        v_Answer[v_Length] = '\0';

        m_SocketClient->Read(v_Answer, v_Length);
        if(!m_SocketClient->Error())
        {
            std::string v_Line(v_Answer);

            if(v_Line.compare("RV#0") != 0)
            {
                std::cout << "ERROR in cDasInterface::Stop: Server returned invalid answer: " << v_Line << std::endl;

                // Discard remainder of string
                delete [] v_Answer;
                v_Length = 2;
                v_Answer = new char[v_Length+1];
                v_Answer[v_Length] = '\0';

                m_SocketClient->Read(v_Answer, v_Length*sizeof(char));
                if(m_SocketClient->Error())
                {
                    std::cout << "ERROR in cDasInterface::Stop: socket error: " << m_SocketClient->LastError() << std::endl;
                }
            }
            else
            {
                m_IsRunning = false;
                std::cout << "INFO in cDasInterface::Stop: execution halted." << std::endl;

                // Discard remainder of string
                delete [] v_Answer;
                v_Length = 2;
                v_Answer = new char[v_Length+1];
                v_Answer[v_Length] = '\0';

                m_SocketClient->Read(v_Answer, v_Length*sizeof(char));
                if(m_SocketClient->Error())
                {
                    std::cout << "ERROR in cDasInterface::Stop: socket error: " << m_SocketClient->LastError() << std::endl;
                }
            }
        }
        else
        {
            std::cout << "ERROR in cDasInterface::Stop: socket error: " << m_SocketClient->LastError() << std::endl;
        }

        delete [] v_Answer;
    }
    else
    {
        std::cout << "ERROR in cDasInterface::Stop: socket error: " << m_SocketClient->LastError() << std::endl;
    }
}

void cDasInterface::ExecuteAction()
{
    std::cout << "WARNING in cDasInterface::ExecuteAction: Command not implemented." << std::endl;
}

void cDasInterface::SetChannelName()
{
    std::cout << "WARNING in cDasInterface::SetChannelName: Command not implemented." << std::endl;
}

void cDasInterface::SendAnalogueChannels()
{
    std::cout << "WARNING in cDasInterface::SendAnalogueChannels: Command not implemented." << std::endl;
}
