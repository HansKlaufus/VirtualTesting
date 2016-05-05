#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <math.h>
#include <iostream>
#include <iterator>
#include <fstream>
#include <sstream>
#include <string>
#include <typeinfo>

#include "advancedactuator.h"
#include "airfoil.h"
#include "controlloop.h"
#include "connection.h"
#include "iointerface.h"
#include "naca4.h"
#include "forcenode.h"
#include "object.h"
#include "scu.h"
#include "signalgenerator.h"
#include "wing.h"

unsigned int cObject::m_NextID = 0;

cObject::cObject()
{
    m_ID = cObject::m_NextID++;
}

cObject::~cObject()
{
    //dtor
}

cNode* cObject::GetNodePointerByID(unsigned int p_ID)
{
    cNode*          v_ReturnNode = NULL;
    bool            v_Found = false;
    unsigned int    v_Index = 0;
    while((v_Index < m_NodeArray.size()) && (!v_Found))
    {
        cNode* v_TempNode = this->GetNodePointerByIndex(v_Index);
        if(v_TempNode->GetID() == p_ID)
        {
            v_ReturnNode = v_TempNode;
            v_Found = true;
        }

        v_Index++;
    }

    return v_ReturnNode;
}

cConnection* cObject::GetConnectionPointerByID(unsigned int p_ID)
{
    cConnection*    v_ReturnConnection = NULL;
    bool            v_Found = false;
    unsigned int    v_Index = 0;
    while((v_Index < m_ConnectionArray.size()) && (!v_Found))
    {
        cConnection* v_TempConnection = this->GetConnectionPointerByIndex(v_Index);
        if(v_TempConnection->GetID() == p_ID)
        {
            v_ReturnConnection = v_TempConnection;
        }

        v_Index++;
    }

    return v_ReturnConnection;
}

cSignalGenerator* cObject::GetSignalGeneratorPointerByID(unsigned int p_ID)
{
    cSignalGenerator* v_ReturnSignalGenerator = NULL;
    bool              v_Found = false;
    unsigned int      v_Index = 0;
    while((v_Index < m_SignalGeneratorArray.size()) && (!v_Found))
    {
        cSignalGenerator* v_TempSignalGenerator = this->GetSignalGeneratorPointerByIndex(v_Index);
        if(v_TempSignalGenerator->GetID() == p_ID)
        {
            v_ReturnSignalGenerator = v_TempSignalGenerator;
        }

        v_Index++;
    }

    return v_ReturnSignalGenerator;
}

cActuator* cObject::GetActuatorPointerByID(unsigned int p_ID)
{
    cActuator*        v_ReturnActuator = NULL;
    bool              v_Found = false;
    unsigned int      v_Index = 0;
    while((v_Index < m_ActuatorArray.size()) && (!v_Found))
    {
        cActuator* v_TempActuator = this->GetActuatorPointerByIndex(v_Index);
        if(v_TempActuator->GetID() == p_ID)
        {
            v_ReturnActuator = v_TempActuator;
        }

        v_Index++;
    }

    return v_ReturnActuator;
}

cControlLoop* cObject::GetControlLoopPointerByID(unsigned int p_ID)
{
    cControlLoop*     v_ReturnControlLoop = NULL;
    bool              v_Found = false;
    unsigned int      v_Index = 0;
    while((v_Index < m_ControlLoopArray.size()) && (!v_Found))
    {
        cControlLoop* v_TempControlLoop = this->GetControlLoopPointerByIndex(v_Index);
        if(v_TempControlLoop->GetID() == p_ID)
        {
            v_ReturnControlLoop = v_TempControlLoop;
        }

        v_Index++;
    }

    return v_ReturnControlLoop;
}

void cObject::CalculateState(double p_TimeStep)
{
    for(unsigned int v_Index=0; v_Index<m_ScuArray.size(); v_Index++)
    {
        cScu* v_Scu = this->GetScuPointerByIndex(v_Index);
        v_Scu->CalculateState(p_TimeStep);
    }

    for(unsigned int v_Index=0; v_Index<m_SignalGeneratorArray.size(); v_Index++)
    {
        cSignalGenerator* v_SignalGenerator = this->GetSignalGeneratorPointerByIndex(v_Index);
        v_SignalGenerator->CalculateState(p_TimeStep);
    }

    for(unsigned int v_Index=0; v_Index<m_ControlLoopArray.size(); v_Index++)
    {
        cControlLoop* v_ControlLoop = this->GetControlLoopPointerByIndex(v_Index);
        v_ControlLoop->CalculateState(p_TimeStep);
    }

    for(unsigned int v_Index=0; v_Index<m_ActuatorArray.size(); v_Index++)
    {
        cActuator* v_Actuator = this->GetActuatorPointerByIndex(v_Index);
        v_Actuator->CalculateState(p_TimeStep);
    }

    for(unsigned int v_Index=0; v_Index<m_NodeArray.size(); v_Index++)
    {
        cNode* v_Node = this->GetNodePointerByIndex(v_Index);
        v_Node->CalculateState(p_TimeStep);
    }

    for(unsigned int v_Index=0; v_Index<m_ConnectionArray.size(); v_Index++)
    {
        cConnection* v_Connection = this->GetConnectionPointerByIndex(v_Index);
        v_Connection->CalculateState(p_TimeStep);
    }
}

//void cObject::CalculateState(double p_TimeStep)
//{
//    for(unsigned int v_Index=0; v_Index<m_SignalGeneratorArray.size(); v_Index++)
//    {
//        cSignalGenerator* v_SignalGenerator = this->GetSignalGeneratorPointerByIndex(v_Index);
//        v_SignalGenerator->CalculateState(p_TimeStep);
//    }
//
//    for(unsigned int v_Index=0; v_Index<m_ControlLoopArray.size(); v_Index++)
//    {
//        cControlLoop* v_ControlLoop = this->GetControlLoopPointerByIndex(v_Index);
//        v_ControlLoop->CalculateState(p_TimeStep);
//    }
//
//    for(unsigned int v_Index=0; v_Index<m_ActuatorArray.size(); v_Index++)
//    {
//        cActuator* v_Actuator = this->GetActuatorPointerByIndex(v_Index);
//        v_Actuator->CalculateState(p_TimeStep);
//    }
//    for(unsigned int v_Index=0; v_Index<m_NodeArray.size(); v_Index++)
//    {
//        cNode* v_Node = this->GetNodePointerByIndex(v_Index);
//        v_Node->CalculateState(p_TimeStep);
//    }
//
//    for(unsigned int v_Index=0; v_Index<m_ConnectionArray.size(); v_Index++)
//    {
//        cConnection* v_Connection = this->GetConnectionPointerByIndex(v_Index);
//        v_Connection->CalculateState(p_TimeStep);
//    }
//}

void cObject::PrintState(void)
{
    /*
    ** Print Signal Generators
    */
    for(unsigned int v_Index=0; v_Index<m_SignalGeneratorArray.size(); v_Index++)
    {
        cSignalGenerator* v_SignalGenerator = m_SignalGeneratorArray.at(v_Index);
        printf("SignalGenerator ID = %d:\t", v_SignalGenerator->GetID());

        double v_Scalar = v_SignalGenerator->GetScalar();
        printf("Output Scalar = [%.2f]\t", v_Scalar);

        cVector v_Output = v_SignalGenerator->GetOutput().at(0);
        printf("Output Vector = [");
        for(unsigned int v_Index2=0; v_Index2<v_Output.size(); v_Index2++)
        {
            printf("%.2f;", v_Output.at(v_Index2));
        }
        printf("]\t");

        printf("\n");
    }

    /*
    ** Print Control Loops
    */
    for(unsigned int v_Index=0; v_Index<m_ControlLoopArray.size(); v_Index++)
    {
        cControlLoop* v_ControlLoop = m_ControlLoopArray.at(v_Index);
        printf("ControlLoop ID = %d:\t", v_ControlLoop->GetID());

        double v_ForceComand = v_ControlLoop->GetForceCommand();
        printf("Force Command = [%.2f]\t", v_ForceComand);

        double v_ForceFeedback = v_ControlLoop->GetForceFeedback();
        printf("Force Feedback = [%.2f]\t", v_ForceFeedback);

        double v_ForceError = v_ControlLoop->GetForceError();
        printf("Force Error = [%.2f]\t", v_ForceError);

        double v_ProporionalOutput = v_ControlLoop->GetProportionalOutput();
        printf("Proportional Output = [%.2f]\t", v_ProporionalOutput);

        double v_IntegratedOutput = v_ControlLoop->GetIntegratedOutput();
        printf("Integrated Output = [%.2f]\t", v_IntegratedOutput);

        double v_DampedOutput = v_ControlLoop->GetDampedOutput();
        printf("Damped Output = [%.2f]\t", v_DampedOutput);

        double v_DerivativeOutput = v_ControlLoop->GetDerivedOutput();
        printf("Derivative Output = [%.2f]\t", v_DerivativeOutput);

        double v_SummedOutput = v_ControlLoop->GetSummedOutput();
        printf("Summed Output = [%.2f]\t", v_SummedOutput);

        double v_Output = v_ControlLoop->GetScalar();
        printf("Output Scalar = [%.2f]\t", v_Output);

        printf("\n");
    }

    /*
    ** Print Actuators
    */
    for(unsigned int v_Index=0; v_Index<m_ActuatorArray.size(); v_Index++)
    {
        cActuator* v_Actuator = m_ActuatorArray.at(v_Index);
        printf("Actuator ID = %d:\t", v_Actuator->GetID());

        double v_ServovalveCurrent = v_Actuator->GetServovalveCurrent();
        printf("Servovalve Current= [%.2f]\t", v_ServovalveCurrent);

        cVector v_Displacement = v_Actuator->GetPosition();
        printf("Displacement = [");
        for(unsigned int v_Index2=0; v_Index2<v_Displacement.size(); v_Index2++)
        {
            printf("%.2f;", v_Displacement.at(v_Index2));
        }
        printf("]\t");

        cVector v_Velocity = v_Actuator->GetVelocity();
        printf("Velocity = [");
        for(unsigned int v_Index2=0; v_Index2<v_Velocity.size(); v_Index2++)
        {
            printf("%.2f;", v_Velocity.at(v_Index2));
        }
        printf("]\t");

        cVector v_Force = v_Actuator->GetForce();
        printf("Force = [");
        for(unsigned int v_Index2=0; v_Index2<v_Velocity.size(); v_Index2++)
        {
            printf("%.2f;", v_Force.at(v_Index2));
        }
        printf("]\t");

        printf("\n");
    }

    /*
    ** Print Nodes
    */
    for(unsigned int v_Index=0; v_Index<m_NodeArray.size(); v_Index++)
    {
        cNode* v_Node = m_NodeArray.at(v_Index);
        printf("Node ID = %d:\t\t", v_Node->GetID());

        cVector v_Position = v_Node->GetPosition();
        printf("Position = [");
        for(unsigned int v_Index2=0; v_Index2<v_Position.size(); v_Index2++)
        {
            printf("%.2f;", v_Position.at(v_Index2));
        }
        printf("]\t");

        printf("Velocity = [");
        cVector v_Velocity = v_Node->GetVelocity();
        for(unsigned int v_Index2=0; v_Index2<v_Velocity.size(); v_Index2++)
        {
            printf("%.2f;", v_Velocity.at(v_Index2));
        }
        printf("]\t");

        printf("Acceleration = [");
        cVector v_Acceleration = v_Node->GetAcceleration();
        for(unsigned int v_Index2=0; v_Index2<v_Acceleration.size(); v_Index2++)
        {
            printf("%.2f;", v_Acceleration.at(v_Index2));
        }
        printf("]\t");

        printf("Force = [");
        cVector v_Force = v_Node->GetForce();
        for(unsigned int v_Index2=0; v_Index2<v_Force.size(); v_Index2++)
        {
            printf("%.2f;", v_Force.at(v_Index2));
        }
        printf("]\t");

        printf("\n");
    }

    /*
    ** Print Connections
    */
    for(unsigned int v_Index=0; v_Index<m_ConnectionArray.size(); v_Index++)
    {
        cConnection* v_Connection = m_ConnectionArray.at(v_Index);
        printf("Connection ID = %d:\t", v_Connection->GetID());

        cVector v_Position = v_Connection->GetPosition();
        printf("Position = [");
        for(unsigned int v_Index2=0; v_Index2<v_Position.size(); v_Index2++)
        {
            printf("%.2f;", v_Position.at(v_Index2));
        }
        printf("]\t");

        cVector v_Velocity = v_Connection->GetVelocity();
        printf("Velocity = [");
        for(unsigned int v_Index2=0; v_Index2<v_Velocity.size(); v_Index2++)
        {
            printf("%.2f;", v_Velocity.at(v_Index2));
        }
        printf("]\t");

        cVector v_Acceleration = v_Connection->GetAcceleration();
        printf("Acceleration = [");
        for(unsigned int v_Index2=0; v_Index2<v_Acceleration.size(); v_Index2++)
        {
            printf("%.2f;", v_Acceleration.at(v_Index2));
        }
        printf("]\t");

        cVector v_Force = v_Connection->GetForce();
        printf("Force = [");
        for(unsigned int v_Index2=0; v_Index2<v_Force.size(); v_Index2++)
        {
            printf("%.2f;", v_Force.at(v_Index2));
        }
        printf("]\t");

//        double v_ForceScalar = v_Connection->GetForceScalar();
//        printf("ForceScalar = [%.2f;]\t", v_ForceScalar);

        printf("\n");
    }

    printf("\n");
}

void cObject::WriteToFile(std::ofstream& p_OutputFile)
{
    if(p_OutputFile.is_open())
    {
        for(unsigned int v_Index=0; v_Index<m_NodeArray.size(); v_Index++)
        {
            cNode* v_Node = m_NodeArray.at(v_Index);
            cVector v_Position = v_Node->GetPosition();
            for(unsigned int v_Index2=0; v_Index2<v_Position.size(); v_Index2++)
            {
                p_OutputFile << v_Position.at(v_Index2) << "; ";
            }

            cVector v_Force = v_Node->GetForce();
            for(unsigned int v_Index2=0; v_Index2<v_Force.size(); v_Index2++)
            {
                p_OutputFile << v_Force.at(v_Index2) << "; ";
            }
        }
        p_OutputFile << std::endl;
    }
}

void cObject::WriteToFile(cMemoryStream& p_OutputFile)
{
    for(unsigned int v_Index=0; v_Index<m_NodeArray.size(); v_Index++)
    {
        cNode* v_Node = m_NodeArray.at(v_Index);
        cVector v_Position = v_Node->GetPosition();
        for(unsigned int v_Index2=0; v_Index2<v_Position.size(); v_Index2++)
        {
            p_OutputFile << v_Position.at(v_Index2) << "; ";
        }

        cVector v_Force = v_Node->GetForce();
        for(unsigned int v_Index2=0; v_Index2<v_Force.size(); v_Index2++)
        {
            p_OutputFile << v_Force.at(v_Index2) << "; ";
        }
    }
    p_OutputFile << std::endl;
}

void cObject::Initialise(void)
{
    for(unsigned int v_Index=0; v_Index<m_ActuatorArray.size(); v_Index++)
    {
        cActuator* v_Actuator = this->GetActuatorPointerByIndex(v_Index);
        v_Actuator->Initialise();
    }

    for(unsigned int v_Index=0; v_Index<m_ConnectionArray.size(); v_Index++)
    {
        cConnection* v_Connection = this->GetConnectionPointerByIndex(v_Index);
        v_Connection->Initialise();
    }
}

void cObject::Start(double p_TimeStep)
{
    char v_FileName[] = "output.csv";

    std::ofstream v_OutputFile;
    v_OutputFile.open(v_FileName, std::ios::out);
    if(v_OutputFile.is_open())
    {
        double v_Time = 0;
        while (v_Time<100)
        {
            this->CalculateState(p_TimeStep);
            this->PrintState();
            this->WriteToFile(v_OutputFile);

            v_Time+=p_TimeStep;
        }
        v_OutputFile.close();
    }
    else
    {
        std::cout << "ERROR in cObject::Start: Unable to open " << v_FileName << "." << std::endl;
    }
}

double cObject::GetRadius()
{
    double v_Radius = 0;

    for(unsigned int v_Index=0; v_Index<m_NodeArray.size(); v_Index++)
    {
        cNode*  v_Node = this->GetNodePointerByIndex(v_Index);
        cVector v_Position = v_Node->GetPosition();
        double  v_Length = v_Position.GetLength();

        if(v_Length > v_Radius)
        {
            v_Radius = v_Length;
        }
    }

    return v_Radius;
}

// TODO: Move ReadConfiguration routine to class cFileIo
void cObject::ReadConfiguration(std::string p_FileName)
{
    char v_Token[] = ";";
    bool v_Error = false;
    std::ifstream v_InputFile;

    v_InputFile.open(p_FileName.c_str());
    if(v_InputFile.is_open())
    {
        std::vector<cIoInterface*>               v_LookupTable;
        std::vector< std::vector<unsigned int> > v_DelayedSources;
        std::string                              v_Line;

        /*
        ** Signal Generators
        */
        unsigned int v_NumberSignalGenerators = 0;
        v_Error = v_Error or this->GetLine(v_InputFile, v_Line);
        if(!v_Error)
        {
            // Read number of signal generators in object
            v_NumberSignalGenerators = atoi(v_Line.c_str());
        }

        // Read signal generators
        for(unsigned int v_Index=0; v_Index<v_NumberSignalGenerators && !v_Error; v_Index++)
        {
            v_Error = v_Error or this->GetLine(v_InputFile, v_Line);
            if(!v_Error)
            {
                std::vector<std::string> p_Items = this->SplitString(v_Line, v_Token);
                if(p_Items.size()!=8)
                {
                    v_Error = true;
                }
                else
                {
                    unsigned int v_Idx     = atoi(p_Items.at(0).c_str());
                    double v_Amplitude     = atof(p_Items.at(1).c_str());
                    double v_Frequency     = atof(p_Items.at(2).c_str());
                    double v_PhaseShift    = atof(p_Items.at(3).c_str());
                    double v_Offset        = atof(p_Items.at(4).c_str());

                    // Read Directional Vector
                    cVector v_Direction;
                    v_Direction.push_back(atof(p_Items.at(5).c_str()));
                    v_Direction.push_back(atof(p_Items.at(6).c_str()));
                    v_Direction.push_back(atof(p_Items.at(7).c_str()));

                    cSignalGenerator* v_SignalGenerator = new cSignalGenerator(v_Offset, v_Amplitude, v_Frequency, v_PhaseShift, v_Direction);
                    this->AddSignalGenerator(v_SignalGenerator);

                    // Add to lookup table
                    v_LookupTable.push_back(v_SignalGenerator);
                }
            }
        }

        /*
        ** Nodes
        */
        unsigned int v_NumberNodes = 0;
        v_Error = v_Error or this->GetLine(v_InputFile, v_Line);
        if(!v_Error)
        {
            // Read number of nodes in object
            v_NumberNodes = atoi(v_Line.c_str());
        }

        // Read nodes
        for(unsigned int v_Index=0; v_Index<v_NumberNodes && !v_Error; v_Index++)
        {
            v_Error = v_Error or this->GetLine(v_InputFile, v_Line);
            if(!v_Error)
            {
                std::vector<std::string> p_Items = this->SplitString(v_Line, v_Token);
                if(p_Items.size()!=5)
                {
                    v_Error = true;
                }
                else
                {
                    unsigned int v_Idx = atoi(p_Items.at(0).c_str());
                    double v_X         = atof(p_Items.at(1).c_str());
                    double v_Y         = atof(p_Items.at(2).c_str());
                    double v_Z         = atof(p_Items.at(3).c_str());
                    double v_Mass      = atof(p_Items.at(4).c_str());
                    // TODO: Add variable to read different types of nodes: Force, Position, Static

                    cForceNode* v_Node = new cForceNode(this, v_Mass, v_X, v_Y, v_Z);
                    this->AddNode(v_Node);

                    //
                    // No source adding here; nodes will get sources assigned by the source routines
                    //

                    // Add to lookup table
                    v_LookupTable.push_back(v_Node);
                }
            }
        }

        /*
        ** Connections
        */
        unsigned int v_NumberConnections = 0;
        v_Error = v_Error or this->GetLine(v_InputFile, v_Line);
        if(!v_Error)
        {
            // Read number of connections in object
            v_NumberConnections = atoi(v_Line.c_str());
        }

        // Read connections
        for(unsigned int v_Index=0; v_Index<v_NumberConnections && !v_Error; v_Index++)
        {
            v_Error = v_Error or this->GetLine(v_InputFile, v_Line);
            if(!v_Error)
            {
                std::vector<std::string> p_Items = this->SplitString(v_Line, v_Token);
                if(p_Items.size()!=5)
                {
                    v_Error = true;
                }
                else
                {
                    unsigned int v_Idx       = atoi(p_Items.at(0).c_str());
                    unsigned int v_Node1Idx  = atoi(p_Items.at(1).c_str());
                    unsigned int v_Node2Idx  = atoi(p_Items.at(2).c_str());
                    double v_DampingConstant = atof(p_Items.at(3).c_str());
                    double v_SpringConstant  = atof(p_Items.at(4).c_str());

                    cConnection* v_Connection = new cConnection(this, v_DampingConstant, v_SpringConstant);
                    this->AddConnection(v_Connection);

                    cIoInterface* v_IoInterface1 = v_LookupTable.at(v_Node1Idx); // TODO: Remove these; use something like v_DelayedSources instead
                    cIoInterface* v_IoInterface2 = v_LookupTable.at(v_Node2Idx); // TODO: Remove these; use something like v_DelayedSources instead
                    v_Connection->SetNodeId1(v_IoInterface1->GetID());
                    v_Connection->SetNodeId2(v_IoInterface2->GetID());

                    //
                    // Add connection as source to the nodes
                    //
                    std::vector<unsigned int> v_Pair1;
                    v_Pair1.push_back(v_Node1Idx);
                    v_Pair1.push_back(v_Idx);
                    v_DelayedSources.push_back(v_Pair1);

                    std::vector<unsigned int> v_Pair2;
                    v_Pair2.push_back(v_Node2Idx);
                    v_Pair2.push_back(v_Idx);
                    v_DelayedSources.push_back(v_Pair2);

                    //
                    // No source adding here; connections do not have sources
                    //

                    // Add to lookup table
                    v_LookupTable.push_back(v_Connection);
                }
            }
        }

        /*
        ** Control Loops
        */
        unsigned int v_NumberControlLoops = 0;
        v_Error = v_Error or this->GetLine(v_InputFile, v_Line);
        if(!v_Error)
        {
            // Read number of control loops in object
            v_NumberControlLoops = atoi(v_Line.c_str());
        }

        // Read control loops
        for(unsigned int v_Index=0; v_Index<v_NumberControlLoops && !v_Error; v_Index++)
        {
            v_Error = v_Error or this->GetLine(v_InputFile, v_Line);
            if(!v_Error)
            {
                std::vector<std::string> p_Items = this->SplitString(v_Line, v_Token);
                if(p_Items.size()!=10)
                {
                    v_Error = true;
                }
                else
                {
                    unsigned int v_Idx                  = atoi(p_Items.at(0).c_str());
                    unsigned int v_CommandIdx           = atoi(p_Items.at(1).c_str());
                    unsigned int v_FeedbackIdx          = atoi(p_Items.at(2).c_str());
                    double       v_ProportionalGain     = atof(p_Items.at(3).c_str());
                    double       v_IntegralGain         = atof(p_Items.at(4).c_str());
                    double       v_DampingGain          = atof(p_Items.at(5).c_str());
                    double       v_DerivativeGain       = atof(p_Items.at(6).c_str());
                    double       v_ServovalveLoopGain   = atof(p_Items.at(7).c_str());
                    double       v_ServovalveOffset     = atof(p_Items.at(8).c_str());
                    double       v_ServovalveMaxCurrent = atof(p_Items.at(9).c_str());

                    cControlLoop* v_ControlLoop = new cControlLoop(1.0f, v_ProportionalGain, v_IntegralGain, v_DampingGain, v_DerivativeGain, v_ServovalveLoopGain, v_ServovalveOffset, v_ServovalveMaxCurrent);
                    this->AddControlLoop(v_ControlLoop);

                    /*
                    ** Add input sources to control loop
                    */
                    std::vector<unsigned int> v_Pair1;
                    v_Pair1.push_back(v_Idx);
                    v_Pair1.push_back(v_CommandIdx);
                    v_DelayedSources.push_back(v_Pair1);

                    std::vector<unsigned int> v_Pair2;
                    v_Pair2.push_back(v_Idx);
                    v_Pair2.push_back(v_FeedbackIdx);
                    v_DelayedSources.push_back(v_Pair2);

                    // Add control loop to lookup table
                    v_LookupTable.push_back(v_ControlLoop);
                }
            }
        }

        /*
        ** Actuators
        */
        unsigned int v_NumberActuators = 0;
        v_Error = v_Error or this->GetLine(v_InputFile, v_Line);
        if(!v_Error)
        {
            // Read number of actuators in object
            v_NumberActuators = atoi(v_Line.c_str());
        }

        // Read actuators
        for(unsigned int v_Index=0; v_Index<v_NumberActuators && !v_Error; v_Index++)
        {
            v_Error = v_Error or this->GetLine(v_InputFile, v_Line);
            if(!v_Error)
            {
                std::vector<std::string> p_Items = this->SplitString(v_Line, v_Token);
                if(p_Items.size()!=10)
                {
                    v_Error = true;
                }
                else
                {
                    unsigned int v_Idx            = atoi(p_Items.at(0).c_str());
                    unsigned int v_SourceIdx      = atoi(p_Items.at(1).c_str());
                    unsigned int v_Node1Idx       = atoi(p_Items.at(2).c_str());
                    unsigned int v_Node2Idx       = atoi(p_Items.at(3).c_str());
                    double v_Area1                = atof(p_Items.at(4).c_str());
                    double v_Area2                = atof(p_Items.at(5).c_str());
                    double v_MaxStroke            = atof(p_Items.at(6).c_str());
                    double v_ServovalveMaxFlow    = atof(p_Items.at(7).c_str());
                    double v_ServovalveMaxCurrent = atof(p_Items.at(8).c_str());
                    double v_HydraulicPressure    = atof(p_Items.at(9).c_str());

                    cAdvancedActuator* v_Actuator = new cAdvancedActuator(this, v_Area1, v_Area2, v_MaxStroke, v_ServovalveMaxFlow, v_ServovalveMaxCurrent, v_HydraulicPressure);
                    this->AddActuator(v_Actuator);

                    cIoInterface* v_IoInterface1 = v_LookupTable.at(v_Node1Idx);
                    cIoInterface* v_IoInterface2 = v_LookupTable.at(v_Node2Idx);

                    v_Actuator->SetNodeId1(v_IoInterface1->GetID());
                    v_Actuator->SetNodeId2(v_IoInterface2->GetID());

                    //
                    // Add inputsource to actuator
                    //
                    std::vector<unsigned int> v_Pair1;
                    v_Pair1.push_back(v_Idx);
                    v_Pair1.push_back(v_SourceIdx);
                    v_DelayedSources.push_back(v_Pair1);

                    //
                    // Add actuator as source for nodes
                    //
                    std::vector<unsigned int> v_Pair2;
                    v_Pair2.push_back(v_Node1Idx);
                    v_Pair2.push_back(v_Idx);
                    v_DelayedSources.push_back(v_Pair2);

                    std::vector<unsigned int> v_Pair3;
                    v_Pair3.push_back(v_Node2Idx);
                    v_Pair3.push_back(v_Idx);
                    v_DelayedSources.push_back(v_Pair3);

                    // Add to lookup table
                    v_LookupTable.push_back(v_Actuator);
                }
            }
        }

        /*
        ** SCUs
        */
        unsigned int v_NumberScu = 0;
        v_Error = v_Error or this->GetLine(v_InputFile, v_Line);
        if(!v_Error)
        {
            // Read number of SCUs in object
            v_NumberScu = atoi(v_Line.c_str());
        }

        // Read SCUs
        for(unsigned int v_Index=0; v_Index<v_NumberScu && !v_Error; v_Index++)
        {
            v_Error = v_Error or this->GetLine(v_InputFile, v_Line);
            if(!v_Error)
            {
                std::vector<std::string> p_Items = this->SplitString(v_Line, v_Token);
                if(p_Items.size()!=2)
                {
                    v_Error = true;
                }
                else
                {
                    unsigned int v_Idx       = atoi(p_Items.at(0).c_str());
                    unsigned int v_SourceIdx = atoi(p_Items.at(1).c_str());

                    cScu* v_Scu = new cScu();
                    this->AddScu(v_Scu);

                    //
                    // Add inputsource to SCU
                    //
                    std::vector<unsigned int> v_Pair;
                    v_Pair.push_back(v_Idx);
                    v_Pair.push_back(v_SourceIdx);
                    v_DelayedSources.push_back(v_Pair);

                    // Add to lookup table
                    v_LookupTable.push_back(v_Scu);
                }
            }
        }

        /*
        ** Wing
        */
        unsigned int v_NumberSections = 0;
        v_Error = v_Error or this->GetLine(v_InputFile, v_Line);
        if(!v_Error)
        {
            // Read number of sections in wing
            v_NumberSections = atoi(v_Line.c_str());
        }

        cWing* v_Wing = NULL;
        if(v_NumberSections>0)
        {
            v_Wing = new cWing(this);
        }

        // Read sections
        for(unsigned int v_Index=0; v_Index<v_NumberSections && !v_Error; v_Index++)
        {
            v_Error = v_Error or this->GetLine(v_InputFile, v_Line);
            if(!v_Error)
            {
                std::vector<std::string> p_Items = this->SplitString(v_Line, v_Token);
                if(p_Items.size()!=8)
                {
                    v_Error = true;
                }
                else
                {
                    unsigned int v_Idx         = atoi(p_Items.at(0).c_str());
                    std::string  v_NacaCode    = p_Items.at(1);
                    double       v_Chord       = atof(p_Items.at(2).c_str());
                    double       v_TiltAngle   = atof(p_Items.at(3).c_str());
                    double       v_X           = atof(p_Items.at(4).c_str());
                    double       v_Y           = atof(p_Items.at(5).c_str());
                    double       v_Z           = atof(p_Items.at(6).c_str());
                    unsigned int v_NumberNodes = atoi(p_Items.at(7).c_str());

                    // Trim leading and trailing spaces
                    v_NacaCode.erase(0, v_NacaCode.find_first_not_of(' '));
                    v_NacaCode.erase(v_NacaCode.find_last_not_of(' ')+1);

                    // Create airfoil
                    cNACA4* v_Airfoil = new cNACA4(v_NacaCode, v_Chord, v_TiltAngle);
                    cVector v_Offset(v_X, v_Y, v_Z);

                    v_Airfoil->CreateAirfoil(v_NumberNodes);
                    v_Wing->AddSection(v_Airfoil, v_Offset);
                }
            }
        }

        if(v_Wing)
        {
            v_Wing->Wing2Object();
        }

        /*
        ** Add delayes sources
        */
        for(unsigned int v_Index=0; v_Index<v_DelayedSources.size(); v_Index++)
        {
            std::vector<unsigned int> v_SourcePair = v_DelayedSources.at(v_Index);

            cIoInterface* v_IoInterface1 = v_LookupTable.at(v_SourcePair.at(0));
            cIoInterface* v_IoInterface2 = v_LookupTable.at(v_SourcePair.at(1));

            v_IoInterface1->AddSource(v_IoInterface2);
        }

        if(v_Error)
        {
            std::cout << "ERROR in cObject::ReadConfiguration: Parser error." << std::endl;
        }

        v_InputFile.close();
    }
    else
    {
        std::cout << "ERROR in cObject::ReadConfiguration: Could not open file for input: " << p_FileName << std::endl;
    }
}

// TODO: Move SplitString to class CFileIo
std::vector<std::string> cObject::SplitString(std::string p_Line, char* p_Token)
{
    std::vector<std::string> v_Items;

    char* v_Temp = (char*)p_Line.c_str();
    char* pch = strtok(v_Temp, p_Token);
    while(pch != NULL)
    {
        v_Items.push_back(std::string(pch));
        pch = strtok(NULL, p_Token);
    }

    return v_Items;
}

// TODO: Move GetLine to class CFileIo
bool cObject::GetLine(std::ifstream& p_InputFile, std::string& p_Line)
{
    bool v_Error = false;

    std::getline(p_InputFile, p_Line);
    while (not p_InputFile.eof() && this->IsNonSenseLine(p_Line))
    {
        std::getline(p_InputFile, p_Line);
    }

    if(p_InputFile.eof())
    {
        v_Error = true;
    }

    return v_Error;
}

// TODO: Move IsNonSenseLine to class CFileIo
bool cObject::IsNonSenseLine(std::string p_Line)
{
    bool v_ReturnValue = false;

    std::remove(p_Line.begin(), p_Line.end(), ' ');
    if(p_Line.size() == 0 || p_Line.at(0) == '#')
    {
        v_ReturnValue = true;
    }

    return v_ReturnValue;
}

// TODO: Move WriteConfiguration to class CFileIo
void cObject::WriteConfiguration(std::string p_FileName)
{
    std::ofstream v_OutputFile;

    v_OutputFile.open(p_FileName.c_str());
    if(v_OutputFile.is_open())
    {
        v_OutputFile << "# Configuration file for VirtualTesting" << std::endl;

        v_OutputFile << "# Number of Nodes in Object" << std::endl;
        v_OutputFile << this->GetNumberNodes() << std::endl;

        v_OutputFile << "# Index; X, Y, Z, Mass" << std::endl;
        for(unsigned int v_Index=0; v_Index < this->GetNumberNodes(); v_Index++)
        {
            cNode* v_Node = this->GetNodePointerByIndex(v_Index);
            cVector v_Position = v_Node->GetPosition();

            v_OutputFile << v_Index << ";";

            for(unsigned int v_Index2=0; v_Index2<v_Position.size(); v_Index2++)
            {
                v_OutputFile << v_Position.at(v_Index2) << ";";
            }

            v_OutputFile << v_Node->GetMass() << std::endl;
        }

        v_OutputFile << "# Number of Connections in Object" << std::endl;
        v_OutputFile << this->GetNumberConnections() << std::endl;

        v_OutputFile << "# Index; Node1; Node2; DampingConstant; SpringConstant" << std::endl;
        for(unsigned int v_Index=0; v_Index < this->GetNumberConnections(); v_Index++)
        {
            cConnection* v_Connection = this->GetConnectionPointerByIndex(v_Index);

            v_OutputFile << v_Index << ";";
            v_OutputFile << v_Connection->GetNodeId1() << ";";
            v_OutputFile << v_Connection->GetNodeId2() << ";";
            v_OutputFile << v_Connection->GetDampingConstant() << ";";
            v_OutputFile << v_Connection->GetSpringConstant() << ";" << std::endl;
        }

        v_OutputFile.close();
    }
    else
    {
        std::cout << "ERROR in cObject::WriteConfiguration: Could not open file for output: " << p_FileName << std::endl;
    }
}

void cObject::RemoveDuplicateConnections(void)
{
    for(unsigned int v_Index=0; v_Index<this->GetNumberConnections(); v_Index++)
    {
        cConnection* v_Connection_A = this->GetConnectionPointerByIndex(v_Index);
        unsigned int v_NodeId_A1 = v_Connection_A->GetNodeId1();
        unsigned int v_NodeId_A2 = v_Connection_A->GetNodeId2();

        for(unsigned int v_Index2=v_Index+1; v_Index2<this->GetNumberConnections(); v_Index2++)
        {
            cConnection* v_Connection_B = this->GetConnectionPointerByIndex(v_Index2);
            unsigned int v_NodeId_B1 = v_Connection_B->GetNodeId1();
            unsigned int v_NodeId_B2 = v_Connection_B->GetNodeId2();

            if(((v_NodeId_A1 == v_NodeId_B1)&&(v_NodeId_A2==v_NodeId_B2)) || ((v_NodeId_A1 == v_NodeId_B2)&&(v_NodeId_A2==v_NodeId_B1)))
            {
                // Duplicate Connection; remove last
                m_ConnectionArray.erase(m_ConnectionArray.begin()+v_Index2);
                v_Index2--;
            }
        }

    }
}
