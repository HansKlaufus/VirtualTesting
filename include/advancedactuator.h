#ifndef CADVANCEDACTUATOR_H
#define CADVANCEDACTUATOR_H

#include "actuator.h"


class cAdvancedActuator : public cActuator
{
    public:
        cAdvancedActuator();
        cAdvancedActuator(cObject* p_ParentObject, double p_AreaBore, double p_AreaRod, double p_MaxStroke, double p_ServovalveMaxFlow, double p_ServovalveMaxCurrent, double p_HydraulicPressure);
        virtual             ~cAdvancedActuator();

        virtual void        CalculateState(double p_TimeStep);

    protected:

    private:
        double              m_ServovalveSensitivity;
        double              m_ServovalveTimeConstant;   // [s]
        double              m_ServovalveUnderlap;       // [m]
        double              m_ServovalveCq;

        double              m_Friction_Coulomb;
        double              m_Friction_Viscous;
        double              m_Flow_Leak;
        double              m_Mass_Rod;

        void                CalculateStateServoValve(double p_TimeStep, double &p_Flow_t, double &p_Flow_b);
        void                CalculateStateCylinder(double p_TimeStep, double p_Flow_t, double p_Flow_b);
};

#endif // CADVANCEDACTUATOR_H
