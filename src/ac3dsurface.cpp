/********************************************************
*** Body file for definition of class clAc3dSurface
***
*** J.L. Klaufus - Haarlem 2010
**/

#include <fstream>
#include <stdlib.h>

#include "ac3dsurface.h"

using namespace std;

clAc3dSurface::clAc3dSurface(void)
{
}

clAc3dSurface::clAc3dSurface(const string theName)
{
  this->SetName(theName);
}

clAc3dSurface::clAc3dSurface(const clAc3dSurface &source)
{
    Assign(source);
}

clAc3dSurface::~clAc3dSurface(void)
{
  //cerr << "DESTROYING SURFACEGRID: " << GetNumberID() << endl;
}

clAc3dSurface& clAc3dSurface::Assign(const clAc3dSurface &source)
{
  // TODO

  return(*this);
}

clAc3dSurface& clAc3dSurface::operator=(const clAc3dSurface &source)
{
  return(Assign(source));
}

