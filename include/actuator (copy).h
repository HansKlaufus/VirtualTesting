#ifndef CACTUATOR_H
#define CACTUATOR_H

#include "iointerface.h"
#include "object.h"

class cActuator : public cIoInterface
{
    public:
        cActuator();
        cActuator(cObject* p_ParentObject, double m_AreaBore, double m_AreaRod, double p_MaxStroke, double p_ServovalveMaxFlow, double p_ServovalveMaxCurrent, double p_HydraulicPressure);
        virtual             ~cActuator();

        virtual void        CalculateState(double p_TimeStep);
        virtual tIoVector   GetOutput(cIoInterface* p_Requester = NULL);
        cVector             GetRodEnd(cIoInterface* p_Requester = NULL);

        void                Initialise();

        unsigned int        GetNodeId1() { return m_NodeId1; }
        void                SetNodeId1(unsigned int val) { m_NodeId1 = val; }
        unsigned int        GetNodeId2() { return m_NodeId2; }
        void                SetNodeId2(unsigned int val) { m_NodeId2 = val; }

        double              GetBoreArea() { return m_AreaBore; }
        void                SetBoreArea(double val) { m_AreaBore = val; }
        double              GetRodArea() { return m_AreaRod; }
        void                SetRodArea(double val) { m_AreaRod = val; }
        double              GetBaseLength() { return m_BaseLength; }
        double              GetMaxStroke() { return m_MaxStroke; }
        void                SetMaxStroke(double val) { m_MaxStroke = val; }
        double              GetServovalveMaxFlow() { return m_ServovalveMaxFlow; }
        void                SetServovalveMaxFlow(double val) { m_ServovalveMaxFlow = val; }
        double              GetServovalveMaxCurrent() { return m_ServovalveMaxCurrent; }
        void                SetServovalveMaxCurrent(double val) { m_ServovalveMaxCurrent = val; }
        double              GetHydraulicPressure() { return m_HydraulicPressure; }
        void                SetHydraulicPressure(double val) { m_HydraulicPressure = val; }
        double              GetServovalveCurrent() { return m_ServovalveCurrent; }
        void                SetServovalveCurrent(double p_ServovalveCurrent);

    protected:

    private:
        cObject*            m_ParentObject;

        unsigned int        m_NodeId1;
        unsigned int        m_NodeId2;

        double              m_BaseLength;
        double              m_AreaBore;
        double              m_AreaRod;
        double              m_MaxStroke;
        double              m_ServovalveMaxFlow;
        double              m_ServovalveMaxCurrent;
        double              m_HydraulicPressure;
        double              m_ServovalveCurrent;

        double              m_Force;
        double              m_Acceleration;
        double              m_Velocity;
        double              m_Stroke;
        double              m_InitialStroke;
};

#endif // CACTUATOR_H
