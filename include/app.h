/***************************************************************
 * Name:      cApp
 * Purpose:   Defines Application Class
 * Author:    Hans Klaufus (HKlaufus@gmail.com)
 * Created:   2011-09-23
 * Copyright: Hans Klaufus ()
 * License:
 **************************************************************/

#ifndef CAPP_H
#define CAPP_H

#include <wx/app.h>

class cApp : public wxApp
{
    public:
        virtual bool OnInit();
};

#endif // CAPP_H


