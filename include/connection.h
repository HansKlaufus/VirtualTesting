#ifndef CCONNECTION_H
#define CCONNECTION_H

#include "iointerface.h"
#include "object.h"
#include "vector.h"

class cConnection: public cIoInterface
{
    public:
        cConnection();
        cConnection(cObject* p_ParentObject, double p_DampingConstant, double p_SpringConstant);
        virtual             ~cConnection();

        virtual tIoVector   GetOutput(cIoInterface* p_Requester=NULL);
        virtual void        CalculateState(double p_TimeStep);

        double              GetDampingConstant() { return m_DampingConstant; }
        void                SetDampingConstant(double val) { m_DampingConstant = val; }
        double              GetSpringConstant() { return m_SpringConstant; }
        void                SetSpringConstant(double val) { m_SpringConstant = val; }
        unsigned int        GetNodeId1() { return m_NodeId1; }
        void                SetNodeId1(unsigned int val) { m_NodeId1 = val; }
        unsigned int        GetNodeId2() { return m_NodeId2; }
        void                SetNodeId2(unsigned int val) { m_NodeId2 = val; }

        void                SetParentObject(cObject* val) { m_ParentObject = val; }
        cObject*            GetParentObject() { return m_ParentObject; }

        double              GetForceScalar() { return m_SpringForce + m_DampingForce; }

        void                Initialise();

    protected:

    private:
        double              m_DampingConstant;
        double              m_SpringConstant;
        unsigned int        m_NodeId1;
        unsigned int        m_NodeId2;
        cObject*            m_ParentObject;

        cVector             m_InitialDisplacementVector;
        double              m_SpringForce;
        double              m_DampingForce;

        void                CalculateSpringForce();
        void                CalculateDampingForce();

        cVector             GetDisplacementVector();
        cVector             GetVelocityVector();
        cVector             GetAccelerationVector();
        cVector             GetForceVector(cIoInterface* p_Requester=NULL);

        double              GetSpringForce() { return m_SpringForce; }
        double              GetDampingForce() { return m_DampingForce; }
};

#endif // CCONNECTION_H
