#include <iostream>
#include <time.h>
#include <typeinfo>

#include "actuator.h"
#include "positionnode.h"

cPositionNode::cPositionNode()
{
    //ctor
}

cPositionNode::cPositionNode(cObject* p_ParentObject, double p_Mass, cVector p_Position) : cNode(p_ParentObject, p_Mass, p_Position)
{
}

cPositionNode::cPositionNode(cObject* p_ParentObject, double p_Mass, double p_X, double p_Y, double p_Z) : cNode(p_ParentObject, p_Mass, p_X, p_Y, p_Z)
{
}

cPositionNode::~cPositionNode()
{
    //dtor
}

void cPositionNode::CalculateState(double p_TimeStep)
{
    /*
    ** Position Node; Calculate new Position
    */

    // Check for source types
    std::vector<unsigned int> v_ActuatorIdx;
    std::vector<unsigned int> v_ConnectionIdx;
    std::vector<unsigned int> v_SignalGeneratorIdx;
    for(unsigned int v_Index=0; v_Index<this->GetNumberSources(); v_Index++)
    {
        cIoInterface* v_SourcePointer = this->GetSourcePointer(v_Index);
        std::string   v_ClassType     = typeid(*v_SourcePointer).name();
        if(v_ClassType.find("cActuator")<v_ClassType.size())
        {
            v_ActuatorIdx.push_back(v_Index);
        }
        else if(v_ClassType.find("cSimpleActuator")<v_ClassType.size())
        {
            v_ActuatorIdx.push_back(v_Index);
        }
        else if(v_ClassType.find("cAdvancedActuator")<v_ClassType.size())
        {
            v_ActuatorIdx.push_back(v_Index);
        }
        else if(v_ClassType.find("cConnection")<v_ClassType.size())
        {
            v_ConnectionIdx.push_back(v_Index);
        }
        else if(v_ClassType.find("cSignalGenerator")<v_ClassType.size())
        {
            v_SignalGeneratorIdx.push_back(v_Index);
        }
    }

    m_Force.assign(3, 0);
    for(unsigned int v_Index=0; v_Index<v_ActuatorIdx.size(); v_Index++)
    {
        cActuator* v_SourcePointer = (cActuator*) (this->GetSourcePointer(v_ActuatorIdx.at(v_Index)));

        unsigned int v_NodeId = v_SourcePointer->GetNodeId1();
        if(v_NodeId == this->GetID())
        {
            v_NodeId = v_SourcePointer->GetNodeId2();
            cNode* v_Node  = this->GetParentObject()->GetNodePointerByID(v_NodeId);
            m_Position     = v_Node->GetPosition() - v_SourcePointer->GetPosition(this);
        } else {
            cNode* v_Node  = this->GetParentObject()->GetNodePointerByID(v_NodeId);
            m_Position     = v_Node->GetPosition() + v_SourcePointer->GetPosition(this);
        }

        m_Velocity     = v_SourcePointer->GetVelocity(this);
        m_Acceleration = v_SourcePointer->GetAcceleration(this);
        m_Force        = m_Force + v_SourcePointer->GetForce(this);
    }

    for(unsigned int v_Index=0; v_Index<v_ConnectionIdx.size(); v_Index++)
    {
        cIoInterface* v_SourcePointer = this->GetSourcePointer(v_ConnectionIdx.at(v_Index));
        m_Force        = m_Force + v_SourcePointer->GetForce(this);
    }

    for(unsigned int v_Index=0; v_Index<v_SignalGeneratorIdx.size(); v_Index++)
    {
        cIoInterface* v_SourcePointer = this->GetSourcePointer(v_SignalGeneratorIdx.at(v_Index));
        m_Force        = m_Force + v_SourcePointer->GetForce(this);
    }
}

//void cPositionNode::CalculateState(double p_TimeStep)
//{
//    /*
//    ** Position Node; Calculate new Position
//    */
//
//    // Check for source types
//    std::vector<unsigned int> v_ActuatorIdx;
//    std::vector<unsigned int> v_ConnectionIdx;
//    std::vector<unsigned int> v_SignalGeneratorIdx;
//    for(unsigned int v_Index=0; v_Index<this->GetNumberSources(); v_Index++)
//    {
//        cIoInterface* v_SourcePointer = this->GetSourcePointer(v_Index);
//        std::string   v_ClassType     = typeid(*v_SourcePointer).name();
//        if(v_ClassType.find("cActuator")<v_ClassType.size())
//        {
//            v_ActuatorIdx.push_back(v_Index);
//        }
//        else if(v_ClassType.find("cSimpleActuator")<v_ClassType.size())
//        {
//            v_ActuatorIdx.push_back(v_Index);
//        }
//        else if(v_ClassType.find("cAdvancedActuator")<v_ClassType.size())
//        {
//            v_ActuatorIdx.push_back(v_Index);
//        }
//        else if(v_ClassType.find("cConnection")<v_ClassType.size())
//        {
//            v_ConnectionIdx.push_back(v_Index);
//        }
//        else if(v_ClassType.find("cSignalGenerator")<v_ClassType.size())
//        {
//            v_SignalGeneratorIdx.push_back(v_Index);
//        }
//    }
//
//    cVector v_OldPosition = m_Position;
//    m_Position.assign(3, 0);
//    for(unsigned int v_Index=0; v_Index<v_ActuatorIdx.size(); v_Index++)
//    {
//        cIoInterface* v_SourcePointer = this->GetSourcePointer(v_ActuatorIdx.at(v_Index));
//        m_Position = m_Position + v_SourcePointer->GetPosition(this);
//    }
//
//    for(unsigned int v_Index=0; v_Index<v_ConnectionIdx.size(); v_Index++)
//    {
//        cIoInterface* v_SourcePointer = this->GetSourcePointer(v_ConnectionIdx.at(v_Index));
//        m_Position = m_Position + v_SourcePointer->GetPosition(this);
//    }
//
//    for(unsigned int v_Index=0; v_Index<v_SignalGeneratorIdx.size(); v_Index++)
//    {
//        cIoInterface* v_SourcePointer = this->GetSourcePointer(v_SignalGeneratorIdx.at(v_Index));
//        m_Position = m_Position + v_SourcePointer->GetPosition(this);
//    }
//
//    // Calculate new state
//    cVector v_OldVelocity   = m_Velocity;
//    m_Velocity              = (m_Position - v_OldPosition) / p_TimeStep;
//    m_Acceleration          = (m_Velocity - v_OldVelocity) / p_TimeStep;
//    m_Force                 = m_Acceleration * m_Mass;
//}
