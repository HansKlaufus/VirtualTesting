#ifndef CAIRFOIL_H
#define CAIRFOIL_H

#include <vector>

#include "vector.h"

class cAirfoil
{
    public:
        cAirfoil();
        cAirfoil(double p_Chord, double p_TiltAngle);
        virtual ~cAirfoil();

        unsigned int GetID() { return m_ID; }

        double       GetChord() { return m_Chord; }
        void         SetChord(double val) { m_Chord = val; }
        double       GetTiltAngle() { return m_TiltAngle; }
        void         SetTiltAngle(double val) { m_TiltAngle = val; }

        void         CreateAirfoil(unsigned int p_NumberNodes);
        void         AddCoordinates(double p_X, double p_Y, double p_Z);
        cVector      GetCoordinates(unsigned int p_Index) { return m_Coordinates.at(p_Index); }
        unsigned int GetNumberCoordinates() { return m_Coordinates.size(); }
        void         TranslateCoordinates(double p_X, double p_Y, double p_Z);
        void         TranslateCoordinates(cVector p_Offset);

    protected:

    private:
        static unsigned int     m_NextID;
        unsigned int            m_ID;

        double                  m_Chord;
        double                  m_TiltAngle;

        std::vector<cVector>    m_Coordinates;
};

#endif // CAIRFOIL_H
