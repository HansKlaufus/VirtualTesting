#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H

#include <wx/socket.h>


class cSocketClient : public wxSocketClient
{
    public:
        cSocketClient();
        virtual ~cSocketClient();

        void     WriteString(char *p_String);
        char*    ReadString();

    protected:

    private:
};

#endif // SOCKETCLIENT_H
