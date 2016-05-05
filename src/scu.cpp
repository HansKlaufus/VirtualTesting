#include <cstddef>
#include <iostream>

#include "scu.h"

cScu::cScu() : cIoInterface()
{
    m_ParentObject      = NULL;
}

cScu::~cScu()
{
}

void cScu::CalculateState(double p_TimeStep)
{
    if(this->GetNumberSources() == 1)
    {
        cIoInterface *v_ActuatorFeedback = this->GetSourcePointer(1);
        double v_Position   = v_ActuatorFeedback->GetPosition().GetLength();
        double v_ForceA     = v_ActuatorFeedback->GetForce().GetLength();
    }
    else
    {
        std::cout << "ERROR in cScu::CalculateState: number sources != 1" << std::endl;
    }
}

double cScu::GetScalar(cIoInterface *p_Requester)
{
    return 0.0;
}
