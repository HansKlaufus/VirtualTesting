#include <cmath>
#include <cstdlib>
#include <iostream>

#include "naca4.h"

cNACA4::cNACA4() : cAirfoil()
{
    m_NacaCode          = "";
    m_MaxCamber         = 0.0f;
    m_MaxCamberLocation = 0.0f;
    m_MaxThickness      = 0.0f;
}

cNACA4::cNACA4(std::string p_NacaCode, double p_Chord, double p_TiltAngle):cAirfoil(p_Chord, p_TiltAngle)
{
    if(p_NacaCode.size() != 4)
    {
        std::cout << "ERROR in cNACA4::cNACA4: Not a NACA 4-digit series airfoil: " << p_NacaCode << std::endl;
    }
    else
    {
        m_NacaCode          = p_NacaCode;
        m_MaxCamber         = atof(m_NacaCode.substr(0, 1).c_str()) / 100.0f;
        m_MaxCamberLocation = atof(m_NacaCode.substr(1, 1).c_str()) / 10.0f;
        m_MaxThickness      = atof(m_NacaCode.substr(2, 2).c_str()) / 100.0f;
    }
}

cNACA4::~cNACA4()
{
    //dtor
}

void cNACA4::CreateAirfoil(unsigned int p_NumberNodes)
{
    double v_Chord              = this->GetChord();
    double v_MaxCamber          = this->GetMaxCamber();
    double v_MaxCamberLocation  = this->GetMaxCamberLocation();
    double v_MaxThickness       = this->GetMaxThickness();
    double v_Angle              = this->GetTiltAngle()*M_PI/180;

    // Ensure p_NumberNodes is even number
    if(p_NumberNodes%2 != 0)
    {
        p_NumberNodes++;
    }

    // Distribution of nodes over upper surface of airfoil
    for(unsigned int v_Index=0; v_Index<p_NumberNodes/2; v_Index++)
    {
        // Calculate x
        double v_X = v_Index * v_Chord / (p_NumberNodes/2);

        // Calculate the thickness with respect to the mean camber line
        double v_Zt = v_MaxThickness / 0.2 * v_Chord  * (0.2969 * pow(v_X/v_Chord, 0.5) -0.1260 * (v_X/v_Chord) -0.3516 * pow(v_X/v_Chord, 2) +0.2843*pow(v_X/v_Chord, 3) -0.1036*pow(v_X/v_Chord, 4));

        // Calculate the mean camber line
        double v_Zc = 0.0f;
        if(v_X < v_MaxCamberLocation*v_Chord)
        {
            v_Zc = v_MaxCamber * v_X / pow(v_MaxCamberLocation, 2) * (2*v_MaxCamberLocation - v_X / v_Chord);
        }
        else
        {
            v_Zc = v_MaxCamber * (v_Chord - v_X)/pow(1 - v_MaxCamberLocation, 2) * (1 + v_X/v_Chord - 2*v_MaxCamberLocation);
        }

        double v_Z = v_Zc+v_Zt;

        // Calculate lower Z
        double v_Xx = v_X * cos(v_Angle) + v_Z * sin(v_Angle);
        double v_Zz = v_Z * cos(v_Angle) - v_X * sin(v_Angle);

        // Add coordinates
        this->AddCoordinates(v_Xx, 0.0f, v_Zz);
    }

    // Distribution of nodes over lower surface of airfoil
    for(unsigned int v_Index=p_NumberNodes/2; v_Index>0; v_Index--)
    {
        // Calculate x
        double v_X = v_Index * v_Chord / (p_NumberNodes/2);

        // Calculate the thickness with respect to the mean camber line
        double v_Zt = v_MaxThickness / 0.2 * v_Chord  * (0.2969 * pow(v_X/v_Chord, 0.5) -0.1260 * (v_X/v_Chord) -0.3516 * pow(v_X/v_Chord, 2) +0.2843*pow(v_X/v_Chord, 3) -0.1036*pow(v_X/v_Chord, 4));

        // Calculate the mean camber line
        double v_Zc = 0.0f;
        if(v_X < v_MaxCamberLocation*v_Chord)
        {
            v_Zc = v_MaxCamber * v_X / pow(v_MaxCamberLocation, 2) * (2*v_MaxCamberLocation - v_X / v_Chord);
        }
        else
        {
            v_Zc = v_MaxCamber * (v_Chord - v_X)/pow(1 - v_MaxCamberLocation, 2) * (1 + v_X/v_Chord - 2*v_MaxCamberLocation);
        }

        double v_Z = v_Zc-v_Zt;

        // Calculate lower Z
        double v_Xx = v_X * cos(v_Angle) + v_Z * sin(v_Angle);
        double v_Zz = v_Z * cos(v_Angle) - v_X * sin(v_Angle);

        // Add coordinates
        this->AddCoordinates(v_Xx, 0.0f, v_Zz);
    }
}
