#include <cmath>
#include <time.h>

#include "actuator.h"
#include "iointerface.h"
#include "node.h"

cActuator::cActuator() : cIoInterface()
{
    m_ParentObject          = NULL;

    m_BaseLength            = 0.0f;
    m_AreaBore              = 0.0f;
    m_AreaRod               = 0.0f;
    m_MaxStroke             = 0.0f;
    m_ServovalveMaxFlow     = 0.0f;
    m_ServovalveMaxCurrent  = 0.0f;
    m_HydraulicPressure     = 0.0f;
    m_ServovalveCurrent     = 0.0f;

    m_Force                 = 0.0f;
    m_Acceleration          = 0.0f;
    m_Velocity              = 0.0f;
    m_Stroke                = 0.0f;
    m_InitialStroke         = 0.0f;
}

cActuator::cActuator(cObject* p_ParentObject, double p_AreaBore, double p_AreaRod, double p_MaxStroke, double p_ServovalveMaxFlow, double p_ServovalveMaxCurrent, double p_HydraulicPressure) : cIoInterface()
{
    m_ParentObject = p_ParentObject;

    m_BaseLength            = p_MaxStroke;
    m_AreaBore              = p_AreaBore;
    m_AreaRod               = p_AreaRod;
    m_MaxStroke             = p_MaxStroke;
    m_ServovalveMaxCurrent  = p_ServovalveMaxCurrent;
    m_ServovalveMaxFlow     = p_ServovalveMaxFlow;
    m_HydraulicPressure     = p_HydraulicPressure;

    m_Force                 = 0.0f;
    m_Acceleration          = 0.0f;
    m_Velocity              = 0.0f;
    m_Stroke                = 0.0f;
    m_InitialStroke         = 0.0f;
}

cActuator::~cActuator()
{
    //dtor
}

void cActuator::Initialise()
{
    cNode* v_Node1 = m_ParentObject->GetNodePointerByID(m_NodeId1);
    cNode* v_Node2 = m_ParentObject->GetNodePointerByID(m_NodeId2);

    if(v_Node1 && v_Node2)
    {
        cVector v_Node1Position = v_Node1->GetPosition(this);
        cVector v_Node2Position = v_Node2->GetPosition(this);
        cVector v_DisplacementVector = v_Node2Position - v_Node1Position;
        m_InitialStroke = v_DisplacementVector.GetLength();
        m_Stroke = m_InitialStroke;
    }
}

void cActuator::SetServovalveCurrent(double p_ServovalveCurrent)
{
    m_ServovalveCurrent = p_ServovalveCurrent;
    if(m_ServovalveCurrent > m_ServovalveMaxCurrent)
    {
        m_ServovalveCurrent = m_ServovalveMaxCurrent;
    }
    else if (m_ServovalveCurrent < -m_ServovalveMaxCurrent)
    {
        m_ServovalveCurrent = -m_ServovalveMaxCurrent;
    }
}

void cActuator::CalculateState(double p_TimeStep)
{
    m_ServovalveCurrent = 0.0f;
    for(unsigned int v_Index=0; v_Index<this->GetNumberSources(); v_Index++)
    {
        cIoInterface* v_SourcePointer = this->GetSourcePointer(v_Index);
        m_ServovalveCurrent = m_ServovalveCurrent + v_SourcePointer->GetScalar();
    }

    if(m_ServovalveCurrent > m_ServovalveMaxCurrent)
    {
        m_ServovalveCurrent = m_ServovalveMaxCurrent;
    }
    else if(m_ServovalveCurrent < -m_ServovalveMaxCurrent)
    {
        m_ServovalveCurrent = -m_ServovalveMaxCurrent;
    }

    // Flow with this current
    double v_Flow = m_ServovalveMaxFlow * m_ServovalveCurrent / m_ServovalveMaxCurrent;

    // Volume added during this timestep, in m³
    double v_Volume = v_Flow * p_TimeStep / 1000;

    // Position at end of this timestep
    double v_OldStroke = m_Stroke;
    m_Stroke = m_Stroke + v_Volume / m_AreaBore;
    if(m_Stroke > (m_BaseLength+m_MaxStroke))
    {
        m_Stroke = (m_BaseLength+m_MaxStroke);
    }
    else if(m_Stroke < m_BaseLength)
    {
        m_Stroke = m_BaseLength;
    }

    // Velocity at end of this timestep
    double v_OldVelocity = m_Velocity;
    m_Velocity = (m_Stroke - v_OldStroke)/p_TimeStep;

    // Acceleration at end of this timestep
    m_Acceleration = (m_Velocity - v_OldVelocity) / p_TimeStep;

    // Displacement Vector between 2 nodes
    cNode* v_Node1 = m_ParentObject->GetNodePointerByID(m_NodeId1);
    cNode* v_Node2 = m_ParentObject->GetNodePointerByID(m_NodeId2);
    cVector v_Displacement = v_Node2->GetPosition() - v_Node1->GetPosition();
    v_Displacement = v_Displacement.GetNormalised();

    // Calculate Force
    cVector v_Force = v_Node2->GetForce() - v_Node1->GetForce();
    double  v_Angle = v_Displacement.GetAngle(v_Force);

    v_Force = v_Force * cos(v_Angle);
    m_Force = v_Force.GetLength();
//    m_Force = (m_Stroke-m_InitialStroke)*100;// - m_Velocity*100000000;
}

tIoVector cActuator::GetOutput(cIoInterface* p_Requester)
{
    // Displacement Vector between 2 nodes
    cNode* v_Node1 = m_ParentObject->GetNodePointerByID(m_NodeId1);
    cNode* v_Node2 = m_ParentObject->GetNodePointerByID(m_NodeId2);

    cVector v_Displacement = v_Node2->GetPosition() - v_Node1->GetPosition();
    v_Displacement = v_Displacement.GetNormalised();

    tIoVector v_Output;
    v_Output.push_back(v_Displacement*m_Force);
    v_Output.push_back(v_Displacement*m_Acceleration);
    v_Output.push_back(v_Displacement*m_Velocity);
    v_Output.push_back(v_Displacement*m_Stroke);

    return v_Output;
}

cVector cActuator::GetRodEnd(cIoInterface* p_Requester)
{
    // Displacement Vector between 2 nodes
    cNode* v_Node1 = m_ParentObject->GetNodePointerByID(m_NodeId1);
    cNode* v_Node2 = m_ParentObject->GetNodePointerByID(m_NodeId2);

    cVector v_Displacement = v_Node2->GetPosition() - v_Node1->GetPosition();
    v_Displacement = v_Displacement.GetNormalised();

    cVector v_Output = v_Node1->GetPosition() + v_Displacement*m_Stroke;

    return v_Output;
}
