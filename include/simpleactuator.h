#ifndef CSIMPLEACTUATOR_H
#define CSIMPLEACTUATOR_H

#include "actuator.h"

/*
** A simple model for an hydraulic actuator in which the actuator acts as an velocity integrator
*/

class cSimpleActuator : public cActuator
{
    public:
        cSimpleActuator();
        cSimpleActuator(cObject* p_ParentObject, double p_AreaBore, double p_AreaRod, double p_MaxStroke, double p_ServovalveMaxFlow, double p_ServovalveMaxCurrent, double p_HydraulicPressure);
        virtual ~cSimpleActuator();

        virtual void CalculateState(double p_TimeStep);

    protected:

    private:
        void    CalculateStateServoValve(double p_TimeStep, double &p_Flow_t, double &p_Flow_b);
        void    CalculateStateCylinder(double p_TimeStep, double p_Flow_t, double p_Flow_b);
};

#endif // CSIMPLEACTUATOR_H
