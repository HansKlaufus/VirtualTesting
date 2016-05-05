#include <iostream>
#include <time.h>
#include <typeinfo>

#include "actuator.h"
#include "connection.h"
#include "node.h"

cNode::cNode() : cIoInterface()
{
}

cNode::cNode(cObject* p_ParentObject, double p_Mass, cVector p_Position) : cIoInterface()
{
    m_ParentObject = p_ParentObject;

    if(m_Mass==0)
    {
        std::cout << "WARNING in cNode::cNode: m_Mass == 0." << std::endl;
    }

    this->SetMass(p_Mass);
    this->SetPosition(p_Position);
    this->SetVelocity(cVector(0, 0, 0));
    this->SetAcceleration(cVector(0, 0, 0));
    this->SetForce(cVector(0, 0, 0));
}

cNode::cNode(cObject* p_ParentObject, double p_Mass, double p_X, double p_Y, double p_Z) : cIoInterface()
{
    m_ParentObject = p_ParentObject;

    if(p_Mass==0)
    {
        std::cout << "WARNING in cNode::cNode: m_Mass == 0." << std::endl;
    }

    cVector v_Position;
    v_Position.push_back(p_X);
    v_Position.push_back(p_Y);
    v_Position.push_back(p_Z);

    this->SetMass(p_Mass);
    this->SetPosition(v_Position);
    this->SetVelocity(cVector(0, 0, 0));
    this->SetAcceleration(cVector(0, 0, 0));
    this->SetForce(cVector(0, 0, 0));
}

cNode::~cNode()
{
    //dtor
}

tIoVector cNode::GetOutput(cIoInterface* p_Requester)
{
    tIoVector v_ReturnVector;

    v_ReturnVector.push_back(m_Force);
    v_ReturnVector.push_back(m_Acceleration);
    v_ReturnVector.push_back(m_Velocity);
    v_ReturnVector.push_back(m_Position);

    return v_ReturnVector;
}
