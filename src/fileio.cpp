#include "advancedactuator.h"
#include "connection.h"
#include "controlloop.h"
#include "fileio.h"
#include "iointerface.h"
#include "naca4.h"
#include "forcenode.h"
#include "positionnode.h"
#include "simpleactuator.h"
#include "staticnode.h"
#include "scu.h"
#include "signalgenerator.h"
#include "wing.h"

#include <cstddef>
#include <iostream>
#include <typeinfo>

unsigned int cFileIo::m_NextID = 0;

cFileIo::cFileIo()
{
    m_ID = cFileIo::m_NextID++;

    m_Object = NULL;
}

cFileIo::cFileIo(cObject *p_Object)
{
    m_ID = cFileIo::m_NextID++;

    m_Object = p_Object;
}

cFileIo::~cFileIo()
{
    //dtor
}

void cFileIo::XmlWriteFile(std::string p_FileName)
{
    if(m_Object)
    {
        wxXmlDocument *v_XmlDocument = new wxXmlDocument;

        wxXmlNode *v_Root = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("OBJECT"));
        v_XmlDocument->SetRoot(v_Root);

        this->XmlWriteNodes(v_Root);
        this->XmlWriteConnections(v_Root);
        this->XmlWriteControlLoops(v_Root);
        this->XmlWriteActuators(v_Root);
        this->XmlWriteSignalGenerators(v_Root);
        this->XmlWriteScus(v_Root);

        v_XmlDocument->Save(wxString(p_FileName.c_str(), wxConvUTF8));

        delete v_XmlDocument;
    }
    else
    {
        std::cout << "ERROR in cFileIo::XmlWriteFile: No file loaded." << std::endl;
    }
}

void cFileIo::XmlWriteNodes(wxXmlNode *p_Parent)
{
    unsigned int v_NumberNodes = m_Object->GetNumberNodes();
    if(v_NumberNodes>0)
    {
        wxXmlNode *v_Chapter = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("NODES"));
        p_Parent->AddChild(v_Chapter);

        for(unsigned int v_Index=0; v_Index<v_NumberNodes; v_Index++)
        {
            cNode *v_Node = m_Object->GetNodePointerByIndex(v_Index);

            wxXmlNode *v_Item = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("NODE"));
            v_Item->AddAttribute(wxT("ID"), wxString::Format(wxT("%d"), v_Node->GetID()));
            v_Item->AddAttribute(wxT("Mass"), wxString::Format(wxT("%.3f"), v_Node->GetMass()));

            cVector v_Position = v_Node->GetPosition();
            v_Item->AddAttribute(wxT("X"), wxString::Format(wxT("%.3f"), v_Position.at(0)));
            v_Item->AddAttribute(wxT("Y"), wxString::Format(wxT("%.3f"), v_Position.at(1)));
            v_Item->AddAttribute(wxT("Z"), wxString::Format(wxT("%.3f"), v_Position.at(2)));

            std::string v_ClassType = typeid(*v_Node).name();
            if(v_ClassType.find("cForceNode")<v_ClassType.size())
            {
                v_Item->AddAttribute(wxT("Type"), wxT("F"));
            }
            else if(v_ClassType.find("cPositionNode")<v_ClassType.size())
            {
                v_Item->AddAttribute(wxT("Type"), wxT("P"));
            }
            else if(v_ClassType.find("cStaticNode")<v_ClassType.size())
            {
                v_Item->AddAttribute(wxT("Type"), wxT("S"));
            }
            else
            {
                std::cout << "WARNING in cFileIo::XmlWriteNodes: unknown node type: " << v_ClassType << std::endl;
            }

            v_Chapter->AddChild(v_Item);
        }
    }
}

void cFileIo::XmlWriteConnections(wxXmlNode *p_Parent)
{
    unsigned int v_NumberConnections = m_Object->GetNumberConnections();
    if(v_NumberConnections>0)
    {
        wxXmlNode *v_Chapter = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("CONNECTIONS"));
        p_Parent->AddChild(v_Chapter);

        for(unsigned int v_Index=0; v_Index<v_NumberConnections; v_Index++)
        {
            cConnection *v_Connection = m_Object->GetConnectionPointerByIndex(v_Index);

            wxXmlNode *v_Item = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("CONNECTION"));
            v_Item->AddAttribute(wxT("ID"), wxString::Format(wxT("%d"), v_Connection->GetID()));
            v_Item->AddAttribute(wxT("NodeId1"), wxString::Format(wxT("%d"), v_Connection->GetNodeId1()));
            v_Item->AddAttribute(wxT("NodeId2"), wxString::Format(wxT("%d"), v_Connection->GetNodeId2()));
            v_Item->AddAttribute(wxT("SpringConstant"), wxString::Format(wxT("%.3f"), v_Connection->GetSpringConstant()));
            v_Item->AddAttribute(wxT("DampingConstant"), wxString::Format(wxT("%.3f"), v_Connection->GetDampingConstant()));

            v_Chapter->AddChild(v_Item);
        }
    }
}

void cFileIo::XmlWriteSignalGenerators(wxXmlNode *p_Parent)
{
    unsigned int v_NumberSignalGenerators = m_Object->GetNumberSignalGenerators();
    if(v_NumberSignalGenerators>0)
    {
        wxXmlNode *v_Chapter = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("SIGNALGENERATORS"));
        p_Parent->AddChild(v_Chapter);

        for(unsigned int v_Index=0; v_Index<v_NumberSignalGenerators; v_Index++)
        {
            cSignalGenerator *v_SignalGenerator = m_Object->GetSignalGeneratorPointerByIndex(v_Index);

            wxXmlNode *v_Item = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("SIGNALGENERATOR"));
            v_Item->AddAttribute(wxT("ID"), wxString::Format(wxT("%d"), v_SignalGenerator->GetID()));
            v_Item->AddAttribute(wxT("Amplitude"), wxString::Format(wxT("%.3f"), v_SignalGenerator->GetAmplitude()));
            v_Item->AddAttribute(wxT("Frequency"), wxString::Format(wxT("%.3f"), v_SignalGenerator->GetFrequency()));
            v_Item->AddAttribute(wxT("PhaseShift"), wxString::Format(wxT("%.3f"), v_SignalGenerator->GetPhaseShift()));
            v_Item->AddAttribute(wxT("Offset"), wxString::Format(wxT("%.3f"), v_SignalGenerator->GetOffset()));

            cVector v_Direction = v_SignalGenerator->GetDirection();
            v_Item->AddAttribute(wxT("X"), wxString::Format(wxT("%.3f"), v_Direction.at(0)));
            v_Item->AddAttribute(wxT("Y"), wxString::Format(wxT("%.3f"), v_Direction.at(1)));
            v_Item->AddAttribute(wxT("Z"), wxString::Format(wxT("%.3f"), v_Direction.at(2)));

            v_Chapter->AddChild(v_Item);
        }
    }
}

void cFileIo::XmlWriteControlLoops(wxXmlNode *p_Parent)
{
    unsigned int v_NumberControlLoops = m_Object->GetNumberControlLoops();
    if(v_NumberControlLoops>0)
    {
        wxXmlNode *v_Chapter = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("CONTROLLOOPS"));
        p_Parent->AddChild(v_Chapter);

        for(unsigned int v_Index=0; v_Index<v_NumberControlLoops; v_Index++)
        {
            cControlLoop *v_ControlLoop = m_Object->GetControlLoopPointerByIndex(v_Index);

            wxXmlNode *v_Item = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("CONTROLLOOP"));
            v_Item->AddAttribute(wxT("ID"), wxString::Format(wxT("%d"), v_ControlLoop->GetID()));
            v_Item->AddAttribute(wxT("CommandID"), wxString::Format(wxT("%d"), v_ControlLoop->GetSourcePointer(0)->GetID()));
            v_Item->AddAttribute(wxT("FeedbackID"), wxString::Format(wxT("%d"), v_ControlLoop->GetSourcePointer(1)->GetID()));
            v_Item->AddAttribute(wxT("ProportionalGain"), wxString::Format(wxT("%.3f"), v_ControlLoop->GetProportionalGain()));
            v_Item->AddAttribute(wxT("IntegralGain"), wxString::Format(wxT("%.3f"), v_ControlLoop->GetIntegralGain()));
            v_Item->AddAttribute(wxT("DampingGain"), wxString::Format(wxT("%.3f"), v_ControlLoop->GetDampingGain()));
            v_Item->AddAttribute(wxT("DerivativeGain"), wxString::Format(wxT("%.3f"), v_ControlLoop->GetDerivativeGain()));
            v_Item->AddAttribute(wxT("ServovalveLoopGain"), wxString::Format(wxT("%.3f"), v_ControlLoop->GetServovalveLoopGain()));
            v_Item->AddAttribute(wxT("ServovalveOffset"), wxString::Format(wxT("%.3f"), v_ControlLoop->GetServovalveOffset()));
            v_Item->AddAttribute(wxT("ServovalveMaxCurrent"), wxString::Format(wxT("%.3f"), v_ControlLoop->GetServovalveMaxCurrent()));

            v_Chapter->AddChild(v_Item);
        }
    }
}

void cFileIo::XmlWriteActuators(wxXmlNode *p_Parent)
{
    unsigned int v_NumberActuators = m_Object->GetNumberActuators();
    if(v_NumberActuators>0)
    {
        wxXmlNode *v_Chapter = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("ACTUATORS"));
        p_Parent->AddChild(v_Chapter);

        for(unsigned int v_Index=0; v_Index<v_NumberActuators; v_Index++)
        {
            cActuator *v_Actuator = m_Object->GetActuatorPointerByIndex(v_Index);

            wxXmlNode *v_Item = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("ACTUATOR"));
            v_Item->AddAttribute(wxT("ID"), wxString::Format(wxT("%d"), v_Actuator->GetID()));
            v_Item->AddAttribute(wxT("InputID"), wxString::Format(wxT("%d"), v_Actuator->GetSourcePointer(0)->GetID()));
            v_Item->AddAttribute(wxT("NodeId1"), wxString::Format(wxT("%d"), v_Actuator->GetNodeId1()));
            v_Item->AddAttribute(wxT("NodeId2"), wxString::Format(wxT("%d"), v_Actuator->GetNodeId2()));
            v_Item->AddAttribute(wxT("BoreArea"), wxString::Format(wxT("%.3f"), v_Actuator->GetBoreArea()));
            v_Item->AddAttribute(wxT("RodArea"), wxString::Format(wxT("%.3f"), v_Actuator->GetRodArea()));
            v_Item->AddAttribute(wxT("MaxStroke"), wxString::Format(wxT("%.3f"), v_Actuator->GetMaxStroke()));
            v_Item->AddAttribute(wxT("ServovalveMaxFlow"), wxString::Format(wxT("%.3f"), v_Actuator->GetServovalveMaxFlow()*6E4));      // Convert m3/s to LPM
            v_Item->AddAttribute(wxT("ServovalveMaxCurrent"), wxString::Format(wxT("%.3f"), v_Actuator->GetServovalveMaxCurrent()));
            v_Item->AddAttribute(wxT("HydraulicPressure"), wxString::Format(wxT("%.3f"), v_Actuator->GetHydraulicPressure()/1E5));      // Convert N/m2 to Bar

            v_Chapter->AddChild(v_Item);
        }
    }
}

void cFileIo::XmlWriteScus(wxXmlNode *p_Parent)
{
    unsigned int v_NumberScu = m_Object->GetNumberScu();
    if(v_NumberScu>0)
    {
        wxXmlNode *v_Chapter = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("SCUS"));
        p_Parent->AddChild(v_Chapter);

        for(unsigned int v_Index=0; v_Index<v_NumberScu; v_Index++)
        {
            cScu *v_SCU = m_Object->GetScuPointerByIndex(v_Index);

            wxXmlNode *v_Item = new wxXmlNode(wxXML_ELEMENT_NODE, wxT("SCU"));
            v_Item->AddAttribute(wxT("ID"), wxString::Format(wxT("%d"), v_SCU->GetID()));
            v_Item->AddAttribute(wxT("InputID"), wxString::Format(wxT("%d"), v_SCU->GetSourcePointer(0)->GetID()));

            v_Chapter->AddChild(v_Item);
        }
    }
}

void cFileIo::XmlWriteWing(wxXmlNode *p_Parent)
{
    std::cout << "WARNING in cFileIo::XmlWriteWing: method not implemented." << std::endl;
}

void cFileIo::XmlReadFile(std::string p_FileName)
{
    if(m_Object)
    {
        std::vector<cIoInterface*>               v_LookupTable;
        std::vector< std::vector<unsigned int> > v_DelayedSources;

        /*
        ** Read XML file
        */
        wxXmlDocument *v_XmlDocument = new wxXmlDocument;
        v_XmlDocument->Load(wxString(p_FileName.c_str(), wxConvUTF8));

        wxXmlNode *v_Root  = v_XmlDocument->GetRoot();
        wxXmlNode *v_Child = v_Root->GetChildren();

        while(v_Child)
        {
            wxString v_ChildName = v_Child->GetName();
            if(v_ChildName == wxT("SIGNALGENERATORS"))
            {
                this->XmlReadSignalGenerators(v_Child, &v_LookupTable, &v_DelayedSources);
            }
            else if(v_ChildName == wxT("NODES"))
            {
                this->XmlReadNodes(v_Child, &v_LookupTable, &v_DelayedSources);
            }
            else if(v_ChildName == wxT("CONNECTIONS"))
            {
                this->XmlReadConnections(v_Child, &v_LookupTable, &v_DelayedSources);
            }
            else if(v_ChildName == wxT("CONTROLLOOPS"))
            {
                this->XmlReadControlLoops(v_Child, &v_LookupTable, &v_DelayedSources);
            }
            else if(v_ChildName == wxT("ACTUATORS"))
            {
                this->XmlReadActuators(v_Child, &v_LookupTable, &v_DelayedSources);
            }
            else if(v_ChildName == wxT("SCUS"))
            {
                this->XmlReadScus(v_Child, &v_LookupTable, &v_DelayedSources);
            }
            else if(v_ChildName == wxT("WINGS"))
            {
                this->XmlReadWings(v_Child);
            }
            else
            {
                std::cout << "ERROR in cFileIo::XmlReadFile: unknown child name: " << v_ChildName << std::endl;
            }

            v_Child = v_Child->GetNext();
        }

        delete v_XmlDocument;

        /*
        ** Add delayed sources
        */
        for(unsigned int v_Index=0; v_Index<v_DelayedSources.size(); v_Index++)
        {
            std::vector<unsigned int> v_SourcePair = v_DelayedSources.at(v_Index);

            cIoInterface* v_IoInterface1 = v_LookupTable.at(v_SourcePair.at(0));
            cIoInterface* v_IoInterface2 = v_LookupTable.at(v_SourcePair.at(1));

            v_IoInterface1->AddSource(v_IoInterface2);
        }
    }
    else
    {
        std::cout << "ERROR in cFileIo::XmlReadFile: Object not defined." << std::endl;
    }
}

void cFileIo::XmlReadNodes(wxXmlNode *p_Parent, std::vector<cIoInterface*> *p_LookupTable, std::vector< std::vector<unsigned int> > *p_DelayedSources)
{
    wxXmlNode *v_Child = p_Parent->GetChildren();
    while(v_Child)
    {
        if(v_Child->GetName() == wxT("NODE"))
        {
            unsigned int v_Idx = wxAtoi(v_Child->GetAttribute(wxT("ID"), wxT("0")));
            double v_X         = wxAtof(v_Child->GetAttribute(wxT("X"), wxT("0")));
            double v_Y         = wxAtof(v_Child->GetAttribute(wxT("Y"), wxT("0")));
            double v_Z         = wxAtof(v_Child->GetAttribute(wxT("Z"), wxT("0")));
            double v_Mass      = wxAtof(v_Child->GetAttribute(wxT("Mass"), wxT("0")));
            wxString v_Type    = v_Child->GetAttribute(wxT("Type"), wxT("F"));

            if(v_Mass < 1E-6)
            {
                v_Mass = 1.0f;
                v_Type = wxT("S");
            }

            cNode* v_Node = NULL;
            if(v_Type == wxT("F"))
            {
                v_Node = new cForceNode(m_Object, v_Mass, v_X, v_Y, v_Z);
            }
            else if(v_Type == wxT("P"))
            {
                v_Node = new cPositionNode(m_Object, v_Mass, v_X, v_Y, v_Z);
            }
            else if(v_Type == wxT("S"))
            {
                v_Node = new cStaticNode(m_Object, v_Mass, v_X, v_Y, v_Z);
            }
            else
            {
                std::cout << "ERROR in cFileIo::XmlReadNodes: unknown node type: " << v_Type << std::endl;
            }

            m_Object->AddNode(v_Node);

            //
            // No source adding here; nodes will get sources assigned by the source routines
            //

            // Add to lookup table
//            p_LookupTable->push_back(v_Node);
            if(p_LookupTable->size() < (v_Idx+1))
            {
                p_LookupTable->resize(v_Idx+1, NULL);
            }
            p_LookupTable->at(v_Idx) = v_Node;
        }
        else
        {
            std::cout << "ERROR in cFileIo::XmlReadNodes: unknown child name: " << v_Child->GetName() << std::endl;
        }

        v_Child = v_Child->GetNext();
    }
}

void cFileIo::XmlReadConnections(wxXmlNode *p_Parent, std::vector<cIoInterface*> *p_LookupTable, std::vector< std::vector<unsigned int> > *p_DelayedSources)
{
    wxXmlNode *v_Child = p_Parent->GetChildren();
    while(v_Child)
    {
        if(v_Child->GetName() == wxT("CONNECTION"))
        {
            unsigned int v_Idx       = wxAtoi(v_Child->GetAttribute(wxT("ID"), wxT("0")));
            unsigned int v_Node1Idx  = wxAtoi(v_Child->GetAttribute(wxT("NodeId1"), wxT("0")));
            unsigned int v_Node2Idx  = wxAtoi(v_Child->GetAttribute(wxT("NodeId2"), wxT("0")));
            double v_DampingConstant = wxAtof(v_Child->GetAttribute(wxT("DampingConstant"), wxT("0")));
            double v_SpringConstant  = wxAtof(v_Child->GetAttribute(wxT("SpringConstant"), wxT("0")));

            cConnection* v_Connection = new cConnection(m_Object, v_DampingConstant, v_SpringConstant);
            m_Object->AddConnection(v_Connection);

            cIoInterface* v_IoInterface1 = p_LookupTable->at(v_Node1Idx); // TODO: Remove these; use something like v_DelayedSources instead
            cIoInterface* v_IoInterface2 = p_LookupTable->at(v_Node2Idx); // TODO: Remove these; use something like v_DelayedSources instead
            v_Connection->SetNodeId1(v_IoInterface1->GetID());
            v_Connection->SetNodeId2(v_IoInterface2->GetID());

            //
            // Add connection as source to the nodes
            //
            std::vector<unsigned int> v_Pair1;
            v_Pair1.push_back(v_Node1Idx);
            v_Pair1.push_back(v_Idx);
            p_DelayedSources->push_back(v_Pair1);

            std::vector<unsigned int> v_Pair2;
            v_Pair2.push_back(v_Node2Idx);
            v_Pair2.push_back(v_Idx);
            p_DelayedSources->push_back(v_Pair2);

            //
            // No source adding here; connections do not have sources
            //

            // Add to lookup table
//            p_LookupTable->push_back(v_Connection);
            if(p_LookupTable->size() < (v_Idx+1))
            {
                p_LookupTable->resize(v_Idx+1, NULL);
            }
            p_LookupTable->at(v_Idx) = v_Connection;
        }
        else
        {
            std::cout << "ERROR in cFileIo::XmlReadConnections: unknown child name: " << v_Child->GetName() << std::endl;
        }

        v_Child = v_Child->GetNext();
    }
}

void cFileIo::XmlReadSignalGenerators(wxXmlNode *p_Parent, std::vector<cIoInterface*> *p_LookupTable, std::vector< std::vector<unsigned int> > *p_DelayedSources)
{
    wxXmlNode *v_Child = p_Parent->GetChildren();
    while(v_Child)
    {
        if(v_Child->GetName() == wxT("SIGNALGENERATOR"))
        {
            unsigned int v_Idx  = wxAtoi(v_Child->GetAttribute(wxT("ID"), wxT("0")));
            double v_Amplitude  = wxAtof(v_Child->GetAttribute(wxT("Amplitude"), wxT("0")));
            double v_Frequency  = wxAtof(v_Child->GetAttribute(wxT("Frequency"), wxT("0")));
            double v_PhaseShift = wxAtof(v_Child->GetAttribute(wxT("PhaseShift"), wxT("0")));
            double v_Offset     = wxAtof(v_Child->GetAttribute(wxT("Offset"), wxT("0")));

            // Read Directional Vector
            cVector v_Direction;
            v_Direction.push_back(wxAtof(v_Child->GetAttribute(wxT("X"), wxT("0"))));
            v_Direction.push_back(wxAtof(v_Child->GetAttribute(wxT("Y"), wxT("0"))));
            v_Direction.push_back(wxAtof(v_Child->GetAttribute(wxT("Z"), wxT("0"))));

            cSignalGenerator* v_SignalGenerator = new cSignalGenerator(v_Offset, v_Amplitude, v_Frequency, v_PhaseShift, v_Direction);
            m_Object->AddSignalGenerator(v_SignalGenerator);

            // Add to lookup table
//            p_LookupTable->push_back(v_SignalGenerator);
            if(p_LookupTable->size() < (v_Idx+1))
            {
                p_LookupTable->resize(v_Idx+1, NULL);
            }
            p_LookupTable->at(v_Idx) = v_SignalGenerator;
        }
        else
        {
            std::cout << "ERROR in cFileIo::XmlReadSignalGenerators: unknown child name: " << v_Child->GetName() << std::endl;
        }


        v_Child = v_Child->GetNext();
    }
}

void cFileIo::XmlReadControlLoops(wxXmlNode *p_Parent, std::vector<cIoInterface*> *p_LookupTable, std::vector< std::vector<unsigned int> > *p_DelayedSources)
{
    wxXmlNode *v_Child = p_Parent->GetChildren();
    while(v_Child)
    {
        if(v_Child->GetName() == wxT("CONTROLLOOP"))
        {
            unsigned int v_Idx                  = wxAtoi(v_Child->GetAttribute(wxT("ID"), wxT("0")));
            unsigned int v_CommandIdx           = wxAtoi(v_Child->GetAttribute(wxT("CommandID"), wxT("0")));
            unsigned int v_FeedbackIdx          = wxAtoi(v_Child->GetAttribute(wxT("FeedbackID"), wxT("0")));
            double       v_MassGain             = wxAtof(v_Child->GetAttribute(wxT("MassGain"), wxT("1")));
            double       v_ProportionalGain     = wxAtof(v_Child->GetAttribute(wxT("ProportionalGain"), wxT("0")));
            double       v_IntegralGain         = wxAtof(v_Child->GetAttribute(wxT("IntegralGain"), wxT("0")));
            double       v_DampingGain          = wxAtof(v_Child->GetAttribute(wxT("DampingGain"), wxT("0")));
            double       v_DerivativeGain       = wxAtof(v_Child->GetAttribute(wxT("DerivativeGain"), wxT("0")));
            double       v_ServovalveLoopGain   = wxAtof(v_Child->GetAttribute(wxT("ServovalveLoopGain"), wxT("0")));
            double       v_ServovalveOffset     = wxAtof(v_Child->GetAttribute(wxT("ServovalveOffset"), wxT("0")));
            double       v_ServovalveMaxCurrent = wxAtof(v_Child->GetAttribute(wxT("ServovalveMaxCurrent"), wxT("0")));

            cControlLoop* v_ControlLoop = new cControlLoop(v_MassGain, v_ProportionalGain, v_IntegralGain, v_DampingGain, v_DerivativeGain, v_ServovalveLoopGain, v_ServovalveOffset, v_ServovalveMaxCurrent);
            m_Object->AddControlLoop(v_ControlLoop);

            /*
            ** Add input sources to control loop
            */
            std::vector<unsigned int> v_Pair1;
            v_Pair1.push_back(v_Idx);
            v_Pair1.push_back(v_CommandIdx);
            p_DelayedSources->push_back(v_Pair1);

            std::vector<unsigned int> v_Pair2;
            v_Pair2.push_back(v_Idx);
            v_Pair2.push_back(v_FeedbackIdx);
            p_DelayedSources->push_back(v_Pair2);

            // Add control loop to lookup table
//            p_LookupTable->push_back(v_ControlLoop);
            if(p_LookupTable->size() < (v_Idx+1))
            {
                p_LookupTable->resize(v_Idx+1, NULL);
            }
            p_LookupTable->at(v_Idx) = v_ControlLoop;
        }
        else
        {
            std::cout << "ERROR in cFileIo::XmlReadControlLoops: unknown child name: " << v_Child->GetName() << std::endl;
        }

        v_Child = v_Child->GetNext();
    }
}

void cFileIo::XmlReadActuators(wxXmlNode *p_Parent, std::vector<cIoInterface*> *p_LookupTable, std::vector< std::vector<unsigned int> > *p_DelayedSources)
{
    wxXmlNode *v_Child = p_Parent->GetChildren();
    while(v_Child)
    {
        if(v_Child->GetName() == wxT("ACTUATOR"))
        {
            unsigned int v_Idx            = wxAtoi(v_Child->GetAttribute(wxT("ID"), wxT("0")));
            unsigned int v_SourceIdx      = wxAtoi(v_Child->GetAttribute(wxT("InputID"), wxT("0")));
            unsigned int v_Node1Idx       = wxAtoi(v_Child->GetAttribute(wxT("NodeId1"), wxT("0")));
            unsigned int v_Node2Idx       = wxAtoi(v_Child->GetAttribute(wxT("NodeId2"), wxT("0")));
            double v_BoreArea             = wxAtof(v_Child->GetAttribute(wxT("BoreArea"), wxT("0")));
            double v_RodArea              = wxAtof(v_Child->GetAttribute(wxT("RodArea"), wxT("0")));
            double v_MaxStroke            = wxAtof(v_Child->GetAttribute(wxT("MaxStroke"), wxT("0")));
            double v_ServovalveMaxFlow    = wxAtof(v_Child->GetAttribute(wxT("ServovalveMaxFlow"), wxT("0")))/6E4;    // Convert LPM to m3/s
            double v_ServovalveMaxCurrent = wxAtof(v_Child->GetAttribute(wxT("ServovalveMaxCurrent"), wxT("0")));
            double v_HydraulicPressure    = wxAtof(v_Child->GetAttribute(wxT("HydraulicPressure"), wxT("0")))*1E5;    // Convert Bar to N/m2

//            cAdvancedActuator* v_Actuator = new cAdvancedActuator(m_Object, v_BoreArea, v_RodArea, v_MaxStroke, v_ServovalveMaxFlow, v_ServovalveMaxCurrent, v_HydraulicPressure);
            cSimpleActuator* v_Actuator = new cSimpleActuator(m_Object, v_BoreArea, v_RodArea, v_MaxStroke, v_ServovalveMaxFlow, v_ServovalveMaxCurrent, v_HydraulicPressure);
            m_Object->AddActuator(v_Actuator);

            cIoInterface* v_IoInterface1 = p_LookupTable->at(v_Node1Idx);
            cIoInterface* v_IoInterface2 = p_LookupTable->at(v_Node2Idx);

            v_Actuator->SetNodeId1(v_IoInterface1->GetID());
            v_Actuator->SetNodeId2(v_IoInterface2->GetID());

            //
            // Add inputsource to actuator
            //
            std::vector<unsigned int> v_Pair1;
            v_Pair1.push_back(v_Idx);
            v_Pair1.push_back(v_SourceIdx);
            p_DelayedSources->push_back(v_Pair1);

            //
            // Add actuator as source for nodes
            //
            std::vector<unsigned int> v_Pair2;
            v_Pair2.push_back(v_Node1Idx);
            v_Pair2.push_back(v_Idx);
            p_DelayedSources->push_back(v_Pair2);

            std::vector<unsigned int> v_Pair3;
            v_Pair3.push_back(v_Node2Idx);
            v_Pair3.push_back(v_Idx);
            p_DelayedSources->push_back(v_Pair3);

            // Add to lookup table
//            p_LookupTable->push_back(v_Actuator);
            if(p_LookupTable->size() < (v_Idx+1))
            {
                p_LookupTable->resize(v_Idx+1, NULL);
            }
            p_LookupTable->at(v_Idx) = v_Actuator;
        }
        else
        {
            std::cout << "ERROR in cFileIo::XmlReadActuators: unknown child name: " << v_Child->GetName() << std::endl;
        }

        v_Child = v_Child->GetNext();
    }
}

void cFileIo::XmlReadScus(wxXmlNode *p_Parent, std::vector<cIoInterface*> *p_LookupTable, std::vector< std::vector<unsigned int> > *p_DelayedSources)
{
    wxXmlNode *v_Child = p_Parent->GetChildren();
    while(v_Child)
    {
        if(v_Child->GetName() == wxT("SCU"))
        {
            unsigned int v_Idx       = wxAtoi(v_Child->GetAttribute(wxT("ID"), wxT("0")));
            unsigned int v_SourceIdx = wxAtoi(v_Child->GetAttribute(wxT("InputID"), wxT("0")));

            cScu* v_Scu = new cScu();
            m_Object->AddScu(v_Scu);

            //
            // Add inputsource to SCU
            //
            std::vector<unsigned int> v_Pair;
            v_Pair.push_back(v_Idx);
            v_Pair.push_back(v_SourceIdx);
            p_DelayedSources->push_back(v_Pair);

            // Add to lookup table
//            p_LookupTable->push_back(v_Scu);
            if(p_LookupTable->size() < (v_Idx+1))
            {
                p_LookupTable->resize(v_Idx+1, NULL);
            }
            p_LookupTable->at(v_Idx) = v_Scu;
        }
        else
        {
            std::cout << "ERROR in cFileIo::XmlReadScus: unknown child name: " << v_Child->GetName() << std::endl;
        }

        v_Child = v_Child->GetNext();
    }
}

void cFileIo::XmlReadWings(wxXmlNode *p_Parent)
{
    wxXmlNode *v_Child = p_Parent->GetChildren();
    while(v_Child)
    {
        if(v_Child->GetName() == wxT("WING"))
        {
            unsigned int v_Idx             = wxAtoi(v_Child->GetAttribute(wxT("ID"), wxT("0")));
            double       v_NodeMass        = wxAtof(v_Child->GetAttribute(wxT("NodeMass"), wxT("0")));
            double       v_SpringConstant  = wxAtof(v_Child->GetAttribute(wxT("SpringConstant"), wxT("0")));
            double       v_DampingConstant = wxAtof(v_Child->GetAttribute(wxT("DampingConstant"), wxT("0")));
            unsigned int v_NumberNodes     = wxAtoi(v_Child->GetAttribute(wxT("NumberNodes"), wxT("0")));

            cWing *v_Wing = new cWing(m_Object, v_NodeMass, v_SpringConstant, v_DampingConstant);
            this->XmlReadWingSections(v_Child, v_Wing, v_NumberNodes);

            v_Wing->Wing2Object();
        }
        else
        {
            std::cout << "ERROR in cFileIo::XmlReadWings: unknown child name: " << v_Child->GetName() << std::endl;
        }

        v_Child = v_Child->GetNext();
    }
}

void cFileIo::XmlReadWingSections(wxXmlNode *p_Parent, cWing *p_Wing, unsigned int p_NumberNodes)
{
    wxXmlNode *v_Child = p_Parent->GetChildren();
    while(v_Child)
    {
        if(v_Child->GetName() == wxT("SECTION"))
        {
            unsigned int v_Idx         = wxAtoi(v_Child->GetAttribute(wxT("ID"), wxT("0")));
            wxString     v_NacaCode    = v_Child->GetAttribute(wxT("NACA"), wxT("0"));
            double       v_Chord       = wxAtof(v_Child->GetAttribute(wxT("Chord"), wxT("0")));
            double       v_TiltAngle   = wxAtof(v_Child->GetAttribute(wxT("TiltAngle"), wxT("0")));
            double       v_X           = wxAtof(v_Child->GetAttribute(wxT("X"), wxT("0")));
            double       v_Y           = wxAtof(v_Child->GetAttribute(wxT("Y"), wxT("0")));
            double       v_Z           = wxAtof(v_Child->GetAttribute(wxT("Z"), wxT("0")));

            // Trim leading and trailing spaces
            v_NacaCode.Trim(true);
            v_NacaCode.Trim(false);

            // Create airfoil
            cNACA4* v_Airfoil = new cNACA4(std::string(v_NacaCode.mb_str()), v_Chord, v_TiltAngle);
            cVector v_Offset(v_X, v_Y, v_Z);

            v_Airfoil->CreateAirfoil(p_NumberNodes);
            p_Wing->AddSection(v_Airfoil, v_Offset);
        }
        else
        {
            std::cout << "ERROR in cFileIo::XmlReadWingSections: unknown child name: " << v_Child->GetName() << std::endl;
        }

        v_Child = v_Child->GetNext();
    }
}
