/***************************************************************
 * Name:      cApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Hans Klaufus (HKlaufus@gmail.com)
 * Created:   2011-09-23
 * Copyright: Hans Klaufus ()
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "app.h"
#include "maingui.h"

IMPLEMENT_APP(cApp);

bool cApp::OnInit()
{
    cMainGUI* v_Gui = new cMainGUI();

    v_Gui->Show();
    return true;
}
