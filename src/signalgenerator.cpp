#include <cmath>
#include <vector>

#include "signalgenerator.h"
#include "vector.h"

cSignalGenerator::cSignalGenerator() : cIoInterface()
{
    m_Output = 0;
    m_Time   = 0;

    m_Direction.push_back(0);
    m_Direction.push_back(0);
    m_Direction.push_back(1);
}

cSignalGenerator::cSignalGenerator(double p_Offset, double p_Amplitude, double p_Frequency, double p_PhaseShift, cVector p_Direction) : cIoInterface()
{
    m_Output = 0;
    m_Time   = 0;

    this->SetOffset(p_Offset);
    this->SetAmplitude(p_Amplitude);
    this->SetFrequency(p_Frequency);
    this->SetPhaseShift(p_PhaseShift);
    this->SetDirection(p_Direction.GetNormalised());
}

cSignalGenerator::~cSignalGenerator()
{
    //dtor
}


tIoVector cSignalGenerator::GetOutput(cIoInterface* p_Requester)
{
    cVector   v_Result = m_Direction * m_Output;
    cVector   v_Dummy(0, 0, 0);

    tIoVector v_Output;
    v_Output.push_back(v_Result);
    v_Output.push_back(v_Dummy);
    v_Output.push_back(v_Dummy);
    v_Output.push_back(v_Dummy);

    return v_Output;
}

void cSignalGenerator::CalculateState(double p_TimeStep)
{
    m_Time  += p_TimeStep;
    m_Output = m_Amplitude*std::sin(2*M_PI*m_Frequency*m_Time + m_PhaseShift) + m_Offset;
}
