#ifndef COBJECT_H
#define COBJECT_H

#include "memorystream.h"

#include <fstream>
#include <vector>

class cActuator;
class cControlLoop;
class cNode;
class cConnection;
class cScu;
class cSignalGenerator;

class cObject
{
    public:
        cObject();
        virtual                     ~cObject();

        unsigned int                GetID() { return m_ID; }

        unsigned int                GetNumberNodes() { return m_NodeArray.size(); }
        void                        AddNode(cNode* p_Node) { m_NodeArray.push_back(p_Node); }
        cNode*                      GetNodePointerByIndex(unsigned int p_Index) { return m_NodeArray.at(p_Index); }
        cNode*                      GetNodePointerByID(unsigned int p_ID);
        std::vector<cNode*>         GetNodeArray() { return m_NodeArray; }
        void                        SetNodeArray(std::vector<cNode*> val) { m_NodeArray = val; }

        unsigned int                GetNumberConnections() { return m_ConnectionArray.size(); }
        void                        AddConnection(cConnection* p_Connection) { m_ConnectionArray.push_back(p_Connection); }
        cConnection*                GetConnectionPointerByIndex(unsigned int p_Index) { return m_ConnectionArray.at(p_Index); }
        cConnection*                GetConnectionPointerByID(unsigned int p_ID);
        std::vector<cConnection*>   GetConnectionArray() { return m_ConnectionArray; }
        void                        SetConnectionArray(std::vector<cConnection*> val) { m_ConnectionArray = val; }

        unsigned int                GetNumberSignalGenerators() { return m_SignalGeneratorArray.size(); }
        void                        AddSignalGenerator(cSignalGenerator* p_SignalGenerator) { m_SignalGeneratorArray.push_back(p_SignalGenerator); }
        cSignalGenerator*           GetSignalGeneratorPointerByIndex(unsigned int p_Index) { return m_SignalGeneratorArray.at(p_Index); }
        cSignalGenerator*           GetSignalGeneratorPointerByID(unsigned int p_ID);

        unsigned int                GetNumberActuators() { return m_ActuatorArray.size(); }
        void                        AddActuator(cActuator* p_Actuator) { m_ActuatorArray.push_back(p_Actuator); }
        cActuator*                  GetActuatorPointerByIndex(unsigned int p_Index) { return m_ActuatorArray.at(p_Index); }
        cActuator*                  GetActuatorPointerByID(unsigned int p_ID);

        unsigned int                GetNumberControlLoops() { return m_ControlLoopArray.size(); }
        void                        AddControlLoop(cControlLoop* p_ControlLoop) { m_ControlLoopArray.push_back(p_ControlLoop); }
        cControlLoop*               GetControlLoopPointerByIndex(unsigned int p_Index) { return m_ControlLoopArray.at(p_Index); }
        cControlLoop*               GetControlLoopPointerByID(unsigned int p_ID);

        unsigned int                GetNumberScu() { return m_ScuArray.size(); }
        void                        AddScu(cScu* p_Scu) { m_ScuArray.push_back(p_Scu); }
        cScu*                       GetScuPointerByIndex(unsigned int p_Index) { return m_ScuArray.at(p_Index); }
        cScu*                       GetScuPointerByID(unsigned int p_ID);

        void                        CalculateState(double p_TimeStep);
        void                        PrintState();
        void                        WriteToFile(std::ofstream& p_OutputFile);
        void                        WriteToFile(cMemoryStream& p_OutputFile);

        void                        Initialise();
        void                        Start() { this->Start(0.1); };
        void                        Start(double p_TimeStep);

        double                      GetRadius();

        void                        ReadConfiguration(std::string p_FileName);
        void                        WriteConfiguration(std::string p_FileName);

        void                        RemoveDuplicateConnections(void);

    protected:

    private:
        static unsigned int             m_NextID;
        unsigned int                    m_ID;

        std::vector<cActuator*>         m_ActuatorArray;
        std::vector<cNode*>             m_NodeArray;
        std::vector<cConnection*>       m_ConnectionArray;
        std::vector<cSignalGenerator*>  m_SignalGeneratorArray;
        std::vector<cControlLoop*>      m_ControlLoopArray;
        std::vector<cScu*>              m_ScuArray;

        std::vector<std::string>        SplitString(std::string p_Line, char* p_Token);
        bool                            GetLine(std::ifstream& p_InputFile, std::string& p_Line);
        bool                            IsNonSenseLine(std::string p_Line);
};

#endif // COBJECT_H
