/******************************************************
*** Header file for declaration of class clAc3dObject
***
*** J.L. Klaufus - Haarlem 2010
**/

#ifndef AC3DOBJECT_H
#define AC3DOBJECT_H

#include <string>
#include <vector>

#include "ac3dsurface.h"
#include "vector.h"

using namespace std;

class clAc3dObject
{
private:
  string        type;
  string        name;
  string        data;
  string        texture;
  vector<float> texRep; //[2];
  vector<float> rot;    //[6];
  vector<float> loc;    //[3];
  string        url;

  vector<cVector*>       vertexList;
  vector<clAc3dSurface*> surfaceList;
  vector<clAc3dObject*>  kidList;

protected:
  void         ParseAC3DFile(fstream&);
  void         ParseAC3DKid(fstream&, clAc3dObject&);
  void         ParseAC3DObject(fstream&, clAc3dObject&);
  void         ParseAC3DSurface(fstream&, clAc3dSurface&);

  void         WriteVTKPoints(iostream&, clAc3dObject&);
  unsigned int WriteVTKPolygons(iostream&, clAc3dObject&, unsigned int&);

public:
  clAc3dObject(void);
  clAc3dObject(string);
  clAc3dObject(const clAc3dObject&);
  ~clAc3dObject(void);

  clAc3dObject& Assign(const clAc3dObject&);
  clAc3dObject& operator=(const clAc3dObject&);

  void           SetType(const string newType)            {type = newType;};
  void           SetName(const string newName)            {name = newName;};
  void           SetData(const string newData)            {data = newData;};
  void           SetTexture(const string newTexture)      {texture= newTexture;};
  void           SetTexRep(const vector<float> newTexRep) {texRep = newTexRep;};
  void           SetRot(const vector<float> newRot)       {rot = newRot;};
  void           SetLoc(const vector<float> newLoc)       {loc = newLoc;};
  void           SetUrl(const string newUrl)              {url= newUrl;};

  void           AddVertex(cVector *vertex)          {vertexList.push_back(vertex);};
  void           AddSurface(clAc3dSurface *surface)  {surfaceList.push_back(surface);};

  string         GetType(void) const    {return (type);};
  string         GetName(void) const    {return (name);};
  string         GetData(void) const    {return (data);};
  string         GetTexture(void) const {return (texture);};
  vector<float>  GetTexRep(void) const  {return (texRep);};
  vector<float>  GetRot(void) const     {return (rot);};
  vector<float>  GetLoc(void) const     {return (loc);};
  string         GetUrl(void) const     {return (url);};

  cVector*       GetVertexPointer(const int index) const  {return (vertexList[index]);};
  clAc3dSurface* GetSurfacePointer(const int index) const {return (surfaceList[index]);};
  clAc3dObject*  GetKidPointer(const int index) const     {return (kidList[index]);};

  unsigned int   GetNumberVertices(bool) const;
  unsigned int   GetNumberSurfaces(bool) const;
  unsigned int   GetNumberKids(bool) const;
  void           GetVertices(vector<cVector*>&, bool) const;
  void           GetSurfaceVertices(vector<vector<unsigned int> > &, bool) const;
  void           WriteLegacyVTKFile(void);
  void           WriteLegacyVTKFile(const string);
  void           WriteLegacyVTKFile(const char*);

  void           ReadAC3DFile(const string);
  void           ReadAC3DFile(const char*);
};

#endif
