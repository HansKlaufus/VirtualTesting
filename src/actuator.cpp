#include <cmath>
#include <iostream>
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

    m_Pressure_t            = 0.0f;
    m_Pressure_b            = 0.0f;

    m_Position              = 0.0f;
    m_Velocity              = 0.0f;
    m_Acceleration          = 0.0f;
    m_Force                 = 0.0f;
    m_InitialStroke         = 0.0f;
}

cActuator::cActuator(cObject* p_ParentObject, double p_AreaBore, double p_AreaRod, double p_MaxStroke, double p_ServovalveMaxFlow, double p_ServovalveMaxCurrent, double p_HydraulicPressure) : cIoInterface()
{
    m_ParentObject          = p_ParentObject;

    m_BaseLength            = p_MaxStroke;
    m_AreaBore              = p_AreaBore;
    m_AreaRod               = p_AreaRod;
    m_MaxStroke             = p_MaxStroke;
    m_ServovalveMaxCurrent  = p_ServovalveMaxCurrent;
    m_ServovalveMaxFlow     = p_ServovalveMaxFlow;
    m_HydraulicPressure     = p_HydraulicPressure;

    m_Pressure_t            = p_HydraulicPressure * p_AreaBore / (p_AreaBore+(p_AreaBore-p_AreaRod));
    m_Pressure_b            = p_HydraulicPressure - m_Pressure_t;

    m_Position              = p_MaxStroke;
    m_Velocity              = 0.0f;
    m_Acceleration          = 0.0f;
    m_Force                 = 0.0f;
    m_InitialStroke         = p_MaxStroke;
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
        m_Position      = m_InitialStroke;
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

tIoVector cActuator::GetOutput(cIoInterface* p_Requester)
{
    // Displacement Vector between 2 nodes
    cNode*  v_Node1 = m_ParentObject->GetNodePointerByID(m_NodeId1);
    cNode*  v_Node2 = m_ParentObject->GetNodePointerByID(m_NodeId2);
    cVector v_Base  = (v_Node2->GetPosition() - v_Node1->GetPosition()).GetNormalised();

    tIoVector v_Output;
    v_Output.push_back(v_Base * m_Force);
    v_Output.push_back(v_Base * m_Acceleration);
    v_Output.push_back(v_Base * m_Velocity);
    v_Output.push_back(v_Base * m_Position);

    return v_Output;
}

//tIoVector cActuator::GetOutput(cIoInterface* p_Requester)
//{
//    // Displacement Vector between 2 nodes
//    cNode*  v_Node1    = m_ParentObject->GetNodePointerByID(m_NodeId1);
//    cNode*  v_Node2    = m_ParentObject->GetNodePointerByID(m_NodeId2);
//
//    cVector v_Acceleration = v_Node2->GetAcceleration() - v_Node1->GetAcceleration();
//    cVector v_Velocity     = v_Node2->GetVelocity() - v_Node1->GetVelocity();
//    cVector v_Position     = v_Node2->GetPosition() - v_Node1->GetPosition();
//    cVector v_Force        = v_Position.GetNormalised() * m_Force;
//
//    tIoVector v_Output;
//    v_Output.push_back(v_Force);
//    v_Output.push_back(v_Acceleration);
//    v_Output.push_back(v_Velocity);
//    v_Output.push_back(v_Position);
//
//    return v_Output;
//}
