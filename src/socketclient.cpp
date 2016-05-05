#include <iostream>

#include "socketclient.h"

cSocketClient::cSocketClient()
{
    //ctor
}

cSocketClient::~cSocketClient()
{
    //dtor
}

void cSocketClient::WriteString(char *p_String)
{
    unsigned int v_BufferLength;
    v_BufferLength = (unsigned char)((strlen(p_String)+1) * sizeof(char));

    // Memorise flags; set new flags
    wxSocketFlags v_Flags = this->GetFlags();
    this->SetFlags(wxSOCKET_WAITALL);

    this->Write(&v_BufferLength, sizeof(int));                  // OPTIONAL: Send number bytes of buffer
    this->Write(p_String, v_BufferLength);    // Send buffer

//    std::cout << (this->Error() ? "cSocketClient::WriteSocket: Write failed!" : "cSocketClient::WriteSocket: Write success.") << std::endl;

    // Restore flags
    this->SetFlags(v_Flags);
}

char* cSocketClient::ReadString()
{
    unsigned int v_BufferLength;
    char*        v_InBuffer;

    // Memorise flags; set new flags
    wxSocketFlags v_Flags = this->GetFlags();
    this->SetFlags(wxSOCKET_WAITALL);

    this->Read(&v_BufferLength, sizeof(int));                      // OPTIONAL: Read number of chars in buffer
    v_InBuffer = new char[v_BufferLength/sizeof(char)];
    this->Read(v_InBuffer, v_BufferLength/sizeof(char)); // Read the buffer

    // Restore flags
    this->SetFlags(v_Flags);

    return v_InBuffer;
}
