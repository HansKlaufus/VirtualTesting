#ifndef CNACA4_H
#define CNACA4_H

#include <string>

#include "airfoil.h"

/*
** http://en.wikipedia.org/wiki/NACA_airfoil
*/

class cNACA4: public cAirfoil
{
    public:
        cNACA4();
        cNACA4(std::string p_NacaCode, double p_Chord, double p_TiltAngle);
        virtual ~cNACA4();

        std::string     GetNacaCode() { return m_NacaCode; }
        double          GetMaxCamber() { return m_MaxCamber; }
        double          GetMaxCamberLocation() { return m_MaxCamberLocation; }
        double          GetMaxThickness() { return m_MaxThickness; }

        void            CreateAirfoil(unsigned int p_NumberNodes);

    protected:

    private:
        std::string m_NacaCode;
        double      m_MaxCamber;
        double      m_MaxCamberLocation;
        double      m_MaxThickness;
};

#endif // CNACA4_H
