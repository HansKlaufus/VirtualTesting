#include <iostream>
#include <time.h>
#include <typeinfo>

#include "forcenode.h"

cForceNode::cForceNode()
{
    //ctor
}

cForceNode::cForceNode(cObject* p_ParentObject, double p_Mass, cVector p_Position) : cNode(p_ParentObject, p_Mass, p_Position)
{
    if(m_Mass==0)
    {
        std::cout << "ERROR in cNode::cForceNode: m_Mass == 0." << std::endl;
    }
}

cForceNode::cForceNode(cObject* p_ParentObject, double p_Mass, double p_X, double p_Y, double p_Z) : cNode(p_ParentObject, p_Mass, p_X, p_Y, p_Z)
{
    if(m_Mass==0)
    {
        std::cout << "ERROR in cNode::cForceNode: m_Mass == 0." << std::endl;
    }
}

cForceNode::~cForceNode()
{
    //dtor
}

void cForceNode::CalculateState(double p_TimeStep)
{
    /*
    ** Force Node; Calculate new Force
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
        cIoInterface* v_SourcePointer = this->GetSourcePointer(v_ActuatorIdx.at(v_Index));
        m_Force = m_Force + v_SourcePointer->GetForce(this);
    }

    for(unsigned int v_Index=0; v_Index<v_ConnectionIdx.size(); v_Index++)
    {
        cIoInterface* v_SourcePointer = this->GetSourcePointer(v_ConnectionIdx.at(v_Index));
        m_Force = m_Force + v_SourcePointer->GetForce(this);
    }

    for(unsigned int v_Index=0; v_Index<v_SignalGeneratorIdx.size(); v_Index++)
    {
        cIoInterface* v_SourcePointer = this->GetSourcePointer(v_SignalGeneratorIdx.at(v_Index));
        m_Force = m_Force + v_SourcePointer->GetForce(this);
    }

    // Calculate new state
    cVector v_OldAcceleration = m_Acceleration;
    m_Acceleration = m_Force / m_Mass;
    cVector v_AverageAcceleration = (v_OldAcceleration + m_Acceleration) / 2;

    cVector v_OldVelocity = m_Velocity;
//    m_Velocity = m_Velocity + v_AverageAcceleration * p_TimeStep;
    m_Velocity = m_Velocity + m_Acceleration * p_TimeStep;

    cVector v_OldPosition = m_Position;
    m_Position = m_Position + m_Velocity * p_TimeStep;
}
