#include "node.h"
#include "simpleactuator.h"

cSimpleActuator::cSimpleActuator() : cActuator()
{
    //ctor
}

cSimpleActuator::cSimpleActuator(cObject* p_ParentObject, double p_AreaBore, double p_AreaRod, double p_MaxStroke, double p_ServovalveMaxFlow, double p_ServovalveMaxCurrent, double p_HydraulicPressure) : cActuator(p_ParentObject, p_AreaBore, p_AreaRod, p_MaxStroke, p_ServovalveMaxFlow, p_ServovalveMaxCurrent, p_HydraulicPressure)
{
    //ctor
}

cSimpleActuator::~cSimpleActuator()
{
    //dtor
}

void cSimpleActuator::CalculateStateServoValve(double p_TimeStep, double &p_Flow_t, double &p_Flow_b)
{
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

    // Calculate the servovalve flow
    double v_Flow = m_ServovalveCurrent / m_ServovalveMaxCurrent * m_ServovalveMaxFlow;
    p_Flow_b = v_Flow;
    p_Flow_t = p_Flow_b / m_AreaBore * (m_AreaBore - m_AreaRod);
}

void cSimpleActuator::CalculateStateCylinder(double p_TimeStep, double p_Flow_t, double p_Flow_b)
{
    // Some helpful variables
    cObject* v_ParentObject = this->GetParentObject();
    cNode* v_Node1 = v_ParentObject->GetNodePointerByID(this->GetNodeId1());
    cNode* v_Node2 = v_ParentObject->GetNodePointerByID(this->GetNodeId2());

    // Volume additions
    double v_Volume_t = p_TimeStep * p_Flow_t;
    double v_Volume_b = p_TimeStep * p_Flow_b;

    // Actuator extension
    double v_Extension = v_Volume_b / m_AreaBore;

    // Check for MIN / MAX extensions
    m_Position = m_Position + v_Extension;
    if(m_Position < this->GetBaseLength()) {
        m_Position  = this->GetBaseLength();
        v_Extension = 0.0f;
    } else if(m_Position > 2*m_MaxStroke) {
        m_Position  = 2*m_MaxStroke;
        v_Extension = 0.0f;
    }

    // Calculate the remaining variables
    m_Velocity     = v_Extension / p_TimeStep;
    m_Acceleration = m_Velocity / p_TimeStep;
    m_Force        = (/*v_Node1->GetMass() + */ v_Node2->GetMass()) * m_Acceleration;
}

void cSimpleActuator::CalculateState(double p_TimeStep)
{
    double v_Flow_t = 0.0f;
    double v_Flow_b = 0.0f;

    this->CalculateStateServoValve(p_TimeStep, v_Flow_t, v_Flow_b);
    this->CalculateStateCylinder(p_TimeStep, v_Flow_t, v_Flow_b);
}
