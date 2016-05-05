#ifndef CACTUATOR_H
#define CACTUATOR_H

#include "iointerface.h"
#include "object.h"

#define BULK_MODULUS_OIL 1.37E6 //E9
#define DENSITY_OIL      8.72E2

class cActuator : public cIoInterface
{
    public:
        cActuator();
        cActuator(cObject* p_ParentObject, double p_AreaBore, double p_AreaRod, double p_MaxStroke, double p_ServovalveMaxFlow, double p_ServovalveMaxCurrent, double p_HydraulicPressure);
        virtual             ~cActuator();

        virtual void        CalculateState(double p_TimeStep) = 0;
        virtual tIoVector   GetOutput(cIoInterface* p_Requester = NULL);

        void                Initialise();

        cObject*            GetParentObject() { return m_ParentObject; }

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
        double              GetInitialStroke() { return m_InitialStroke; }

    protected:
        double              m_AreaBore;                 // [m2]
        double              m_AreaRod;                  // [m2]

        double              m_ServovalveCurrent;        // [mA]
        double              m_ServovalveMaxCurrent;     // [mA]
        double              m_ServovalveMaxFlow;        // [l/min]

        double              m_Pressure_t;               // [N/m2]
        double              m_Pressure_b;               // [N/m2]

        double              m_Position;                 // [m]
        double              m_Velocity;                 // [m/s]
        double              m_Acceleration;             // [m/s2]
        double              m_Force;                    // [N]

        double              m_HydraulicPressure;        // [bar]
        double              m_BaseLength;               // [m]
        double              m_MaxStroke;                // [m]
        double              m_InitialStroke;

    private:
        cObject*            m_ParentObject;

        unsigned int        m_NodeId1;
        unsigned int        m_NodeId2;
};

#endif // CACTUATOR_H
