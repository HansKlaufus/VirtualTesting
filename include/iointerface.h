#ifndef CIOINTERFACE_H
#define CIOINTERFACE_H

#include <cstddef>
#include <vector>

#include "vector.h"

/*
** Abstract Class for IO Interface
*/

typedef std::vector<cVector> tIoVector;

class cIoInterface
{
    public:
        cIoInterface();
        virtual ~cIoInterface();

        virtual unsigned int GetID() { return m_ID; }

        void              AddSource(cIoInterface* p_Source) { m_SourceArray.push_back(p_Source); }
        cIoInterface*     GetSourcePointer(unsigned int p_Index);
        unsigned int      GetNumberSources() { return m_SourceArray.size(); }

        virtual void      CalculateState(double p_TimeStep) = 0;

        // tIoVector should consist of [Force, Acceleration, Velocity, Position]
        virtual double    GetScalar(cIoInterface* p_Requester = NULL)       { return 0; }
        virtual tIoVector GetOutput(cIoInterface* p_Requester = NULL);
        virtual cVector   GetForce(cIoInterface* p_Requester = NULL)        { return this->GetOutput(p_Requester).at(0); }
        virtual cVector   GetAcceleration(cIoInterface* p_Requester = NULL) { return this->GetOutput(p_Requester).at(1); }
        virtual cVector   GetVelocity(cIoInterface* p_Requester = NULL)     { return this->GetOutput(p_Requester).at(2); }
        virtual cVector   GetPosition(cIoInterface* p_Requester = NULL)     { return this->GetOutput(p_Requester).at(3); }

    protected:

    private:
        static unsigned int  m_NextID;
        unsigned int         m_ID;

        std::vector<cIoInterface*> m_SourceArray;
};

#endif // CIOINTERFACE_H
