#include <cstddef>

#include "airfoil.h"

unsigned int cAirfoil::m_NextID = 0;

cAirfoil::cAirfoil()
{
    m_ID = cAirfoil::m_NextID++;
}

cAirfoil::cAirfoil(double p_Chord, double p_TiltAngle)
{
    m_ID = cAirfoil::m_NextID++;

    m_Chord         = p_Chord;
    m_TiltAngle     = p_TiltAngle;
}

cAirfoil::~cAirfoil()
{
    //dtor
}

void cAirfoil::CreateAirfoil(unsigned int p_NumberNodes)
{

}

void cAirfoil::AddCoordinates(double p_X, double p_Y, double p_Z)
{
    cVector v_Coordinates(p_X, p_Y, p_Z);
    m_Coordinates.push_back(v_Coordinates);
}

void cAirfoil::TranslateCoordinates(double p_X, double p_Y, double p_Z)
{
    cVector v_Offset(p_X, p_Y, p_Z);
    this->TranslateCoordinates(v_Offset);
}

void cAirfoil::TranslateCoordinates(cVector p_Offset)
{
    for(unsigned int v_Index=0; v_Index<m_Coordinates.size(); v_Index++)
    {
        cVector v_Coordinates = m_Coordinates.at(v_Index);
        v_Coordinates = v_Coordinates + p_Offset;
        m_Coordinates.at(v_Index) = v_Coordinates;
    }
}
