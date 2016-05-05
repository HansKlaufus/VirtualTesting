#ifndef CNODE_H
#define CNODE_H

#include <vector>

#include "object.h"
#include "iointerface.h"
#include "vector.h"

class cNode : public cIoInterface
{
    public:
        cNode();
        cNode(cObject* p_Parent, double p_Mass, cVector p_Position);
        cNode(cObject* p_Parent, double p_Mass, double p_X, double p_Y, double p_Z);
        virtual                     ~cNode();

        virtual void                CalculateState(double p_TimeStep) = 0;
        virtual tIoVector           GetOutput(cIoInterface* p_Requester = NULL);

        cObject*                    GetParentObject() { return m_ParentObject; }

        double                      GetMass() { return m_Mass; }

    protected:
        double                      m_Mass;
        cVector                     m_Force;
        cVector                     m_Position;
        cVector                     m_Velocity;
        cVector                     m_Acceleration;

        void                        SetMass(double val) { m_Mass = val; }
        void                        SetForce(cVector val) { m_Force = val; }
        void                        SetVelocity(cVector val) { m_Velocity = val; }
        void                        SetAcceleration(cVector val) { m_Acceleration = val; }
        void                        SetPosition(cVector val) { m_Position = val; }

    private:
        cObject*                    m_ParentObject;
        void                        SetParentObject(cObject* val) { m_ParentObject = val; }
};

#endif // CNODE_H
