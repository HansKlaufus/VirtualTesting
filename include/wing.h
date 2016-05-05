#ifndef CWING_H
#define CWING_H

#include <vector>

#include "airfoil.h"
#include "object.h"

class cWing
{
    public:
        cWing();
        cWing(cObject* p_ParentObject);
        cWing(cObject* p_ParentObject, double p_NodeMass, double p_SpringConstant, double p_DampingConstant);
        virtual ~cWing();

        unsigned int GetID() { return m_ID; }

        void         AddSection(cAirfoil* p_Airfoil, cVector p_Offset);
        unsigned int GetNumberSections() { return m_Sections.size(); }
        cAirfoil*    GetAirfoilPointer(unsigned int p_Index) { return m_Sections.at(p_Index); }
        void         Wing2Object();

    protected:

    private:
        static unsigned int     m_NextID;
        unsigned int            m_ID;

        cObject*                m_Object;

        double                  m_NodeMass;
        double                  m_SpringConstant;
        double                  m_DampingConstant;

        std::vector<cAirfoil*>  m_Sections;
};

#endif // CWING_H
