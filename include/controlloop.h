#ifndef CONTROLLOOP_H
#define CONTROLLOOP_H

#include "iointerface.h"


class cControlLoop : public cIoInterface
{
    public:
        cControlLoop();
        cControlLoop(double p_MassGain, double p_ProportionalGain, double p_IntegralGain, double p_DampingGain, double p_DerivativeGain, double p_ServovalveLoopGain, double p_ServovalveOffset, double p_ServovalveMaxCurrent);
        virtual ~cControlLoop();

        virtual void CalculateState(double p_TimeStep);
        virtual double GetScalar(cIoInterface* p_Requester = NULL) { return m_ServoValveCurrent; }

        double  GetForceCommand() { return m_ForceCommand; }
        double  GetForceFeedback() { return m_ForceFeedback; }

        double  GetForceError() { return m_ForceError; }
        double  GetProportionalError() { return m_ProportionalError; }
        double  GetIntegratedError() { return m_IntegratedError; }
        double  GetDerivedError() { return m_DerivedError; }
        double  GetProportionalOutput() { return m_ProportionalOutput; }
        double  GetIntegratedOutput() { return m_IntegratedOutput; }
        double  GetDampedOutput() { return m_DampedOutput; }
        double  GetDerivedOutput() { return m_DerivedOutput; }
        double  GetSummedOutput() { return m_SummedOutput; }
        double  GetScaledOutput() { return m_ScaledOutput; }

        double  GetProportionalGain() { return m_ProportionalGain; }
        void    SetProportionalGain(double val) { m_ProportionalGain = val; }
        double  GetIntegralGain() { return m_IntegralGain; }
        void    SetIntegralGain(double val) { m_IntegralGain = val; }
        double  GetDampingGain() { return m_DampingGain; }
        void    SetDampingGain(double val) { m_DampingGain = val; }
        double  GetDerivativeGain() { return m_DerivativeGain; }
        void    SetDerivativeGain(double val) { m_DerivativeGain = val; }
        double  GetServovalveMaxCurrent() { return m_ServovalveMaxCurrent; }
        void    SetServovalveMaxCurrent(double val) { m_ServovalveMaxCurrent = val; }
        double  GetServovalveLoopGain() { return m_ServovalveLoopGain; }
        void    SetServovalveLoopGain(double val) { m_ServovalveLoopGain = val; }
        double  GetServovalveOffset() { return m_ServovalveOffset; }
        void    SetServovalveOffset(double val) { m_ServovalveOffset = val; }

    protected:

    private:
        double  m_MassGain;
        double  m_ProportionalGain;
        double  m_IntegralGain;
        double  m_DampingGain;
        double  m_DerivativeGain;
        double  m_ServovalveMaxCurrent;
        double  m_ServovalveLoopGain;
        double  m_ServovalveOffset;

        double  m_ForceCommand;
        double  m_ForceFeedback;
        double  m_ForceError;
        double  m_ProportionalError;
        double  m_IntegratedError;
        double  m_DerivedError;
        double  m_ProportionalOutput;
        double  m_IntegratedOutput;
        double  m_DampedOutput;
        double  m_DerivedOutput;
        double  m_SummedOutput;
        double  m_ScaledOutput;
        double  m_ServoValveCurrent;

        void    SetForceCommand(double val) { m_ForceCommand = val; }
        void    SetForceFeedback(double val) { m_ForceFeedback = val; }
};

#endif // CONTROLLOOP_H
