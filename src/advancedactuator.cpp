#include <cmath>
#include <iostream>
#include <time.h>

#include "advancedactuator.h"
#include "node.h"

cAdvancedActuator::cAdvancedActuator() : cActuator()
{
    m_ServovalveSensitivity  = 2.4f;
    m_ServovalveTimeConstant = 0.002f;
    m_ServovalveUnderlap     = 0.0f;
    m_ServovalveCq           = M_PI/(M_PI+2); // Assumption: Turbulent flow

    m_Friction_Coulomb      = 0.0f;     // TODO: Define Coulomb friction
    m_Friction_Viscous      = 0.0f;     // TODO: Define viscous friction
    m_Flow_Leak             = 0.0f;     // TODO: Define leak flows for top and bottom chambers
    m_Mass_Rod              = 0.0f;     // TODO: Define rod mass
}

cAdvancedActuator::cAdvancedActuator(cObject* p_ParentObject, double p_AreaBore, double p_AreaRod, double p_MaxStroke, double p_ServovalveMaxFlow, double p_ServovalveMaxCurrent, double p_HydraulicPressure) : cActuator(p_ParentObject, p_AreaBore, p_AreaRod, p_MaxStroke, p_ServovalveMaxFlow, p_ServovalveMaxCurrent, p_HydraulicPressure)
{
    m_ServovalveSensitivity  = 2.4f;
    m_ServovalveTimeConstant = 0.002f;
    m_ServovalveUnderlap     = 0.0f;
    m_ServovalveCq           = p_ServovalveMaxFlow / sqrt(p_HydraulicPressure); //M_PI/(M_PI+2);   // Assumption: Turbulent flow // TODO: Check!!!

    m_Friction_Coulomb      = 0.0f;     // TODO: Define Coulomb friction
    m_Friction_Viscous      = 0.0f;     // TODO: Define viscous friction
    m_Flow_Leak             = 0.0f;     // TODO: Define leak flows for top and bottom chambers
    m_Mass_Rod              = 0.0f;     // TODO: Define rod mass
}

cAdvancedActuator::~cAdvancedActuator()
{
    //dtor
}

void cAdvancedActuator::CalculateStateServoValve(double p_TimeStep, double &p_Flow_t, double &p_Flow_b)
{
    /*
    ** Model for servo valve
    **
    ** Assumptions:
    **  1. Flows are turbulent
    **  2. Valve has 'under lap'
    **  3. No internale leakages
    **  4. No hysteresis
    **  5. 1st order dynamic behaviour
    **  6. Return pressure is negligable
    */

    // Calculate the servovalve current
    m_ServovalveCurrent = 0.0f;
    for(unsigned int v_Index=0; v_Index<this->GetNumberSources(); v_Index++)
    {
        cIoInterface* v_SourcePointer = this->GetSourcePointer(v_Index);
        m_ServovalveCurrent = m_ServovalveCurrent + v_SourcePointer->GetScalar();
    }

    double v_ServovalveMaxCurrent = this->GetServovalveMaxCurrent();
    if(m_ServovalveCurrent > v_ServovalveMaxCurrent)
    {
        m_ServovalveCurrent = v_ServovalveMaxCurrent;
    }
    else if(m_ServovalveCurrent < -v_ServovalveMaxCurrent)
    {
        m_ServovalveCurrent = -v_ServovalveMaxCurrent;
    }

    // Calculate servovalve displacement
    static double v_ServovalveDisplacement = 0.0f;
    double v_ServovalveVelocity = (m_ServovalveCurrent*m_ServovalveSensitivity - v_ServovalveDisplacement)/m_ServovalveTimeConstant;
    v_ServovalveDisplacement = v_ServovalveDisplacement + v_ServovalveVelocity * p_TimeStep;
//    double v_ServovalveDisplacement = -m_ServovalveCurrent/this->GetServovalveMaxCurrent();

    // Calculate the top and bottom flow
    double v_AreaBore = this->GetBoreArea();
    double v_AreaRod = this->GetRodArea();
    double v_HydraulicPressure = this->GetHydraulicPressure();

    double v_Flow_b_1 = -m_ServovalveCq * (m_ServovalveUnderlap+v_ServovalveDisplacement) * sqrt(m_Pressure_b);
    double v_Flow_b_2 = +m_ServovalveCq * (m_ServovalveUnderlap-v_ServovalveDisplacement) * sqrt(v_HydraulicPressure-m_Pressure_b);

    double v_Flow_t_1 = +m_ServovalveCq * (m_ServovalveUnderlap+v_ServovalveDisplacement) * sqrt(v_HydraulicPressure-m_Pressure_t) * (v_AreaBore-v_AreaRod)/v_AreaBore ;
    double v_Flow_t_2 = -m_ServovalveCq * (m_ServovalveUnderlap-v_ServovalveDisplacement) * sqrt(m_Pressure_t) * (v_AreaBore-v_AreaRod)/v_AreaBore ;

    if(v_ServovalveDisplacement>m_ServovalveUnderlap)
    {
        p_Flow_b = v_Flow_b_1;
        p_Flow_t = v_Flow_t_1;
    }
    else if (v_ServovalveDisplacement<-m_ServovalveUnderlap)
    {
        p_Flow_b = v_Flow_b_2;
        p_Flow_t = v_Flow_t_2;
    }
    else
    {
        p_Flow_b = v_Flow_b_1 + v_Flow_b_2;
        p_Flow_t = v_Flow_t_1 + v_Flow_t_2;
    }

    double v_ServovalveMaxFlow = this->GetServovalveMaxFlow();
    if(fabs(p_Flow_t)>v_ServovalveMaxFlow || fabs(p_Flow_b)>v_ServovalveMaxFlow)
    {
        std::cout << "ERROR in cActuator::CalculateStateServoValve: Maximum flow exceeded." << std::endl;
    }
}

void cAdvancedActuator::CalculateStateCylinder(double p_TimeStep, double p_Flow_t, double p_Flow_b)
{
    /*
    ** Model for Asymmetric Cylinder
    ** Variables on top side use subscript _t
    ** Variables on bottom side use subscript _b
    */

    // Some helpful variables
    cObject* v_ParentObject = this->GetParentObject();

    cNode* v_Node1 = v_ParentObject->GetNodePointerByID(this->GetNodeId1());
    cNode* v_Node2 = v_ParentObject->GetNodePointerByID(this->GetNodeId2());
    cVector v_Displacement = v_Node2->GetPosition() - v_Node1->GetPosition();
    v_Displacement = v_Displacement.GetNormalised();

    // Calculate the actuator areas
    double v_Area_t     = this->GetBoreArea() - this->GetRodArea();
    double v_Area_b     = this->GetBoreArea();

    // Calculate the chamber volumes
    double v_Stroke     = this->GetPosition().GetLength();
    double v_Volume_t   = (this->GetBaseLength()/2 - (v_Stroke-this->GetInitialStroke()))*v_Area_t;
    double v_Volume_b   = (this->GetBaseLength()/2 + (v_Stroke-this->GetInitialStroke()))*v_Area_b;

    // Check for zero volumes
    if(v_Volume_t<1E-6) v_Volume_t = 1E-6;
    if(v_Volume_b<1E-6) v_Volume_b = 1E-6;

    // Calculate extension rate
    static double v_OldStroke = this->GetInitialStroke();
    double v_Velocity = (v_Stroke - v_OldStroke)/p_TimeStep;
    v_OldStroke = v_Stroke;

    // Calculate the net flows; NEGATIVE flow is OUT of actuator; POSITIVE flow is INTO actuator
    double v_NetFlow_t  = p_Flow_t;// - m_Flow_Leak - v_Area_t*v_Velocity;
    double v_NetFlow_b  = p_Flow_b;// - m_Flow_Leak + v_Area_b*v_Velocity; // Mind the minus-sign

    // Calculate the pressure flux
    double v_PressureFlux_t = v_NetFlow_t * BULK_MODULUS_OIL/v_Volume_t;
    double v_PressureFlux_b = v_NetFlow_b * BULK_MODULUS_OIL/v_Volume_b;

    // Calculate the chamber pressures
    m_Pressure_t += v_PressureFlux_t * p_TimeStep;
    m_Pressure_b += v_PressureFlux_b * p_TimeStep;

    //DEBUG: Check for NaN
    if(m_Pressure_t!=m_Pressure_t || m_Pressure_b!=m_Pressure_b)
    {
        std::cout << "ERROR in cActuator::CalculateState: m_Pressure = NaN." << std::endl;
    }

    // Test for negative pressures
    if(m_Pressure_t<0 || m_Pressure_b>this->GetHydraulicPressure())
    {
        m_Pressure_t = 0;
        m_Pressure_b = this->GetHydraulicPressure();
    }
    else if (m_Pressure_t>this->GetHydraulicPressure() || m_Pressure_b<0)
    {
        m_Pressure_t = this->GetHydraulicPressure();
        m_Pressure_b = 0;
    }

    // Calculate the pressure forces
    double v_Force_t = m_Pressure_t * v_Area_t;
    double v_Force_b = m_Pressure_b * v_Area_b;

    // Calculate the friction forces
    double v_Sign               = (v_Velocity < 0) ? -1.0f : +1.0f;
    double v_Friction_Coulomb   = m_Friction_Coulomb * v_Sign;
    double v_Friction_Viscous   = m_Friction_Viscous * v_Velocity;
    double v_Force_Friction     = v_Friction_Coulomb + v_Friction_Viscous;

    // Calculate the external force
    cVector v_Force         = v_Node2->GetForce() - v_Node1->GetForce();
    double  v_Angle         = v_Displacement.GetAngle(v_Force);
    v_Force                 = v_Force * cos(v_Angle);
    double v_Force_External = 0; //v_Force.GetLength();

    // Calculate the actuator force
    m_Force = v_Force_b - v_Force_t - v_Force_External - v_Force_Friction;

    //DEBUG: Check for NaN
    if(m_Force!=m_Force)
    {
        std::cout << "ERROR in cActuator::CalculateState: m_Force = NaN." << std::endl;
    }
}

void cAdvancedActuator::CalculateState(double p_TimeStep)
{
    double v_Flow_t = 0.0f;
    double v_Flow_b = 0.0f;

    this->CalculateStateServoValve(p_TimeStep, v_Flow_t, v_Flow_b);
    this->CalculateStateCylinder(p_TimeStep, v_Flow_t, v_Flow_b);
}
