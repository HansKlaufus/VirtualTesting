#ifndef CSIGNALGENERATOR_H
#define CSIGNALGENERATOR_H

#include <cstddef>
#include <vector>

#include "iointerface.h"
#include "vector.h"

class cSignalGenerator: public cIoInterface
{
    public:
        cSignalGenerator();
        cSignalGenerator(double p_Offset, double p_Amplitude, double p_Frequency, double p_PhaseShift, cVector p_Direction);
        virtual ~cSignalGenerator();

        virtual void        CalculateState(double p_TimeStep);
        virtual double      GetScalar(cIoInterface* p_Requester=NULL) { return m_Output; }
        virtual tIoVector   GetOutput(cIoInterface* p_Requester=NULL);

        double              GetOffset() { return m_Offset; }
        void                SetOffset(double val) { m_Offset = val; }
        double              GetAmplitude() { return m_Amplitude; }
        void                SetAmplitude(double val) { m_Amplitude = val; }
        double              GetFrequency() { return m_Frequency; }
        void                SetFrequency(double val) { m_Frequency = val; }
        double              GetPhaseShift() { return m_PhaseShift; }
        void                SetPhaseShift(double val) { m_PhaseShift = val; }
        cVector             GetDirection() { return m_Direction; }
        void                SetDirection(cVector val) { m_Direction = val; }

    protected:

    private:
        double              m_Time;

        double              m_Amplitude;
        double              m_Frequency;
        double              m_PhaseShift;
        double              m_Offset;
        double              m_Output;
        cVector             m_Direction;
};

#endif // CSIGNALGENERATOR_H
