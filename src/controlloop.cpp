#include <iostream>

#include "controlloop.h"
#include "vector.h"

cControlLoop::cControlLoop() : cIoInterface()
{
    m_ForceCommand          = 0;;
    m_ForceFeedback         = 0;
    m_ForceError            = 0;
    m_ProportionalError     = 0;
    m_IntegratedError       = 0;
    m_DerivedError          = 0;

    m_ProportionalOutput    = 0;
    m_IntegratedOutput      = 0;
    m_DampedOutput          = 0;
    m_DerivedOutput         = 0;
    m_SummedOutput          = 0;
    m_ScaledOutput          = 0;

    m_ServoValveCurrent     = 0;

    m_MassGain              = 1;
    m_ProportionalGain      = 0;
    m_IntegralGain          = 0;
    m_DampingGain           = 0;
    m_DerivativeGain        = 0;
    m_ServovalveLoopGain    = 0;
    m_ServovalveOffset      = 0;
    m_ServovalveMaxCurrent  = 0;
}

cControlLoop::cControlLoop(double p_MassGain, double p_ProportionalGain, double p_IntegralGain, double p_DampingGain, double p_DerivativeGain, double p_ServovalveLoopGain, double p_ServovalveOffset, double p_ServovalveMaxCurrent) : cIoInterface()
{
    m_ForceCommand          = 0;;
    m_ForceFeedback         = 0;
    m_ForceError            = 0;
    m_ProportionalError     = 0;
    m_IntegratedError       = 0;
    m_DerivedError          = 0;

    m_ProportionalOutput    = 0;
    m_IntegratedOutput      = 0;
    m_DampedOutput          = 0;
    m_DerivedOutput         = 0;
    m_SummedOutput          = 0;
    m_ScaledOutput          = 0;

    m_ServoValveCurrent     = 0;

    m_MassGain              = p_MassGain;
    m_ProportionalGain      = p_ProportionalGain;
    m_IntegralGain          = p_IntegralGain;
    m_DampingGain           = p_DampingGain;
    m_DerivativeGain        = p_DerivativeGain;
    m_ServovalveLoopGain    = p_ServovalveLoopGain;
    m_ServovalveOffset      = p_ServovalveOffset;
    m_ServovalveMaxCurrent  = p_ServovalveMaxCurrent;
}

cControlLoop::~cControlLoop()
{
    //dtor
}

void cControlLoop::CalculateState(double p_TimeStep)
{
    unsigned int v_NumberSources = this->GetNumberSources();
    if(v_NumberSources!=2)
    {
        std::cout << "ERROR in cControlLoop::CalculateState: incompatible number of sources." << std::endl;
    }
    else
    {
        // retrieve inputs
        m_ForceCommand          = this->GetSourcePointer(0)->GetScalar(this);
        cVector v_ForceFeedback = this->GetSourcePointer(1)->GetForce(this);
        m_ForceFeedback         = v_ForceFeedback.GetLength();

        // Calculate errors
        m_ForceError            = (m_ForceCommand - m_ForceFeedback)/m_MassGain;
        m_ProportionalError     = m_ForceError - m_DampedOutput;
        m_IntegratedError       += m_ProportionalError*p_TimeStep;
        m_DerivedError          = m_ProportionalError/p_TimeStep;

        // Calculate branch outputs
        m_ProportionalOutput    = m_ProportionalError*m_ProportionalGain;
        m_IntegratedOutput      = m_IntegratedError*m_IntegralGain;
        m_DampedOutput          = m_IntegratedError*m_DampingGain;
        m_DerivedOutput         = m_DerivedError*m_DerivativeGain;
        m_SummedOutput          = m_ProportionalOutput + m_IntegratedOutput + m_DerivedOutput;

        // Prepare for servovalve
        m_ScaledOutput          = m_SummedOutput*m_ServovalveLoopGain + m_ServovalveOffset;
        if(m_ScaledOutput > m_ServovalveMaxCurrent)
        {
            m_ServoValveCurrent = m_ServovalveMaxCurrent;
        }
        else if (m_ScaledOutput < -m_ServovalveMaxCurrent)
        {
            m_ServoValveCurrent = -m_ServovalveMaxCurrent;
        }
        else
        {
            m_ServoValveCurrent = m_ScaledOutput;
        }
    }
}
