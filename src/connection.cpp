#include <iostream>
#include <cmath>

#include "connection.h"
#include "node.h"
#include "vector.h"

cConnection::cConnection() : cIoInterface()
{
}

cConnection::cConnection(cObject* p_ParentObject, double p_DampingConstant, double p_SpringConstant) : cIoInterface()
{
    m_ParentObject = p_ParentObject;
    m_SpringForce = 0;
    m_DampingForce = 0;

    this->SetDampingConstant(p_DampingConstant);
    this->SetSpringConstant(p_SpringConstant);
}


cConnection::~cConnection()
{
    //dtor
}

tIoVector cConnection::GetOutput(cIoInterface* p_Requester)
{
    cVector v_DisplacementVector = this->GetDisplacementVector();
    cVector v_VelocityVector     = this->GetVelocityVector();
    cVector v_AccelerationVector = this->GetAccelerationVector();
    cVector v_ForceVector        = this->GetForceVector(p_Requester);

    tIoVector v_Output;
    v_Output.push_back(v_ForceVector);
    v_Output.push_back(v_AccelerationVector);
    v_Output.push_back(v_VelocityVector);
    v_Output.push_back(v_DisplacementVector);

    return v_Output;
}

void cConnection::CalculateState(double p_TimeStep)
{
    this->CalculateSpringForce();
    this->CalculateDampingForce();
}

void cConnection::Initialise()
{
    m_InitialDisplacementVector = this->GetDisplacementVector();
}

cVector cConnection::GetDisplacementVector()
{
    cVector v_DisplacementVector;
    v_DisplacementVector.assign(m_InitialDisplacementVector.size(), 0.0);

    cNode* v_Node1 = m_ParentObject->GetNodePointerByID(m_NodeId1);
    cNode* v_Node2 = m_ParentObject->GetNodePointerByID(m_NodeId2);

    if(v_Node1 && v_Node2)
    {
        cVector v_Node1Position = v_Node1->GetPosition(this);
        cVector v_Node2Position = v_Node2->GetPosition(this);
        v_DisplacementVector = v_Node2Position - v_Node1Position;
    }

    return v_DisplacementVector;
}

cVector cConnection::GetVelocityVector()
{
    cVector v_VelocityVector;
    v_VelocityVector.assign(m_InitialDisplacementVector.size(), 0.0);

    cNode* v_Node1 = m_ParentObject->GetNodePointerByID(m_NodeId1);
    cNode* v_Node2 = m_ParentObject->GetNodePointerByID(m_NodeId2);

    if(v_Node1 && v_Node2)
    {
        cVector v_Node1Velocity = v_Node1->GetVelocity(this);
        cVector v_Node2Velocity = v_Node2->GetVelocity(this);
        v_VelocityVector = v_Node2Velocity - v_Node1Velocity;
    }

    return v_VelocityVector;
}

cVector cConnection::GetAccelerationVector()
{
    cVector v_AccelerationVector;
    v_AccelerationVector.assign(m_InitialDisplacementVector.size(), 0.0);

    cNode* v_Node1 = m_ParentObject->GetNodePointerByID(m_NodeId1);
    cNode* v_Node2 = m_ParentObject->GetNodePointerByID(m_NodeId2);

    if(v_Node1 && v_Node2)
    {
        cVector v_Node1Acceleration = v_Node1->GetAcceleration(this);
        cVector v_Node2Acceleration = v_Node2->GetAcceleration(this);
        v_AccelerationVector = v_Node2Acceleration - v_Node1Acceleration;
    }

    return v_AccelerationVector;
}

cVector cConnection::GetForceVector(cIoInterface* p_Requester)
{
    cNode* v_Node1 = m_ParentObject->GetNodePointerByID(m_NodeId1);
    cNode* v_Node2 = m_ParentObject->GetNodePointerByID(m_NodeId2);

    cVector v_ForceVector = this->GetDisplacementVector().GetNormalised() * (this->GetSpringForce() + this->GetDampingForce());

    if (p_Requester == v_Node2)
    {
        v_ForceVector = v_ForceVector * -1;
    }

    return v_ForceVector;
}

void cConnection::CalculateSpringForce()
{
    /*
    ** If extention > 0, then force will try to shorten the connection,
    **
    ** So:
    ** Extension > 0 --> SpringForce > 0: SpringForce will PULL on nodes
    ** Extension < 0 --> SpringForce < 0: SpringForce will PUSH on nodes
    */

    cVector v_DisplacementVector = this->GetDisplacementVector();
    double v_Extension = 0.0;
    double v_Angle = v_DisplacementVector.GetAngle(m_InitialDisplacementVector);
    if(v_Angle > M_PI/2)
    {
        /*
        ** Node1 overtakes Node2
        ** Length of displacement vector should therefore be oppsite of sign in order to keep the sign of the force correct
        */
        v_Extension = -1*v_DisplacementVector.GetLength() - m_InitialDisplacementVector.GetLength();
    }
    else
    {
        v_Extension = v_DisplacementVector.GetLength() - m_InitialDisplacementVector.GetLength();
    }

    m_SpringForce = v_Extension * this->GetSpringConstant();

    //DEBUG: Check for NaN
    if(m_SpringForce!=m_SpringForce)
    {
        std::cout << "ERROR in cConnection::CalculateState: m_SpringForce = NaN." << std::endl;
    }
}

void cConnection::CalculateDampingForce()
{
    /*
    ** 6 Situations:
    ** 1) V2 > 0 && V1 < 0  --> Vtot > 0: DamperForce will PULL on nodes
    ** 2) V2 < 0 && V1 > 0  --> Vtot < 0: DamperForce will PUSH on nodes
    ** 3) V1 > 0 && V2 > V1 --> Vtot > 0: DamperForce will PULL on nodes
    ** 4) V2 > 0 && V1 > V2 --> Vtot < 0: DamperForce will PUSH on nodes
    ** 5) V1 < 0 && V2 < V1 --> Vtot < 0: DamperForce will PUSH on nodes
    ** 6) V2 < 0 && V1 < V2 --> Vtot > 0: DamperForce will PULL on nodes
    **
    ** So:
    ** Vtot > 0 --> DamperForce > 0: DamperForce will PULL on nodes
    ** Vtot < 0 --> DamperForce < 0: DamperForce will PUSH on nodes
    */

    cVector v_DisplacementVector = this->GetDisplacementVector();
    cVector v_VelocityVector = this->GetVelocityVector();

    double v_Displacement = v_DisplacementVector.GetLength();
    double v_Velocity = v_VelocityVector.GetLength();

    m_DampingForce = 0.0f;
    if(fabs(v_Velocity)>0.001 && fabs(v_Displacement)>0.001)
    {
        double v_Angle = v_VelocityVector.GetAngle(v_DisplacementVector);
        m_DampingForce = cos(v_Angle) * v_Velocity * this->GetDampingConstant();
    }

    //DEBUG: Check for NaN
    if(m_DampingForce!=m_DampingForce)
    {
        std::cout << "ERROR in cConnection::CalculateState: m_DampingForce = NaN." << std::endl;
    }
}
