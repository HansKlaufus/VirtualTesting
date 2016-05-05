#include <cstddef>
#include <iostream>

#include "connection.h"
#include "forcenode.h"
#include "node.h"
#include "staticnode.h"
#include "wing.h"

unsigned int cWing::m_NextID = 0;

cWing::cWing()
{
    m_ID              = cWing::m_NextID++;
    m_Object          = NULL;

    m_NodeMass        = 1.0f;
    m_SpringConstant  = 1.0f;
    m_DampingConstant = 1.0f;
}

cWing::cWing(cObject* p_Object)
{
    m_ID              = cWing::m_NextID++;
    m_Object          = p_Object;

    m_NodeMass        = 1.0f;
    m_SpringConstant  = 1.0f;
    m_DampingConstant = 1.0f;
}

cWing::cWing(cObject* p_Object, double p_NodeMass, double p_SpringConstant, double p_DampingConstant)
{
    m_ID              = cWing::m_NextID++;
    m_Object          = p_Object;

    m_NodeMass        = p_NodeMass;
    m_SpringConstant  = p_SpringConstant;
    m_DampingConstant = p_DampingConstant;
}

cWing::~cWing()
{
    //dtor
}

void cWing::AddSection(cAirfoil* p_Airfoil, cVector p_Offset)
{
    p_Airfoil->TranslateCoordinates(p_Offset);
    m_Sections.push_back(p_Airfoil);
}

void cWing::Wing2Object()
{
    if(m_Object==NULL)
    {
        std::cout << "ERROR in cWing::Wing2Object: m_Object not set." << std::endl;
    }
    else
    {
        // Loop over all sections
        for(unsigned int v_Index=0; v_Index<this->GetNumberSections(); v_Index++)
        {
            cAirfoil* v_Airfoil = this->GetAirfoilPointer(v_Index);

            // For each co-ordinate in the airfoil, create a node and add it to the object
            unsigned int v_Node0Idx = 0;
            for(unsigned int v_Index2=0; v_Index2<v_Airfoil->GetNumberCoordinates(); v_Index2++)
            {
                cVector     v_Coordinates = v_Airfoil->GetCoordinates(v_Index2);

                // Make first section fixed to the world
                if(v_Index==0)
                {
                    cStaticNode* v_Node = new cStaticNode(m_Object, m_NodeMass, v_Coordinates);
                    m_Object->AddNode(v_Node);
                } else {
                    cForceNode* v_Node = new cForceNode(m_Object, m_NodeMass, v_Coordinates);
                    m_Object->AddNode(v_Node);
                }

                // Determine what the index of the first airfoil node is with respect to the full object, and store it in in v_Node0Idx
                if(v_Index2==0)
                {
                    v_Node0Idx = m_Object->GetNumberNodes()-1;
                }
            }

            // Loop over the airfoil nodes, but use the index wrt the object. Next, create connections for all nodes within one airfoil
            for(unsigned int v_Index2=v_Node0Idx; v_Index2<m_Object->GetNumberNodes(); v_Index2++)
            {
                cNode* v_Node1 = NULL;
                if(v_Index2==v_Node0Idx)
                {
                    // If the current node equals the first node in this section, then set v_Node1 to the last node in this section...
                    v_Node1 = m_Object->GetNodePointerByIndex(m_Object->GetNumberNodes()-1);
                }
                else
                {
                    // ...otherwise set v_Node1 to the previoous node in the section
                    v_Node1 = m_Object->GetNodePointerByIndex(v_Index2-1);
                }
                cNode* v_Node2 = m_Object->GetNodePointerByIndex(v_Index2);

                unsigned int v_Node1Id = v_Node1->GetID();
                unsigned int v_Node2Id = v_Node2->GetID();

                // Create the connection...
                cConnection* v_Connection = new cConnection(m_Object, m_DampingConstant, m_SpringConstant);
                v_Connection->SetNodeId1(v_Node1Id);
                v_Connection->SetNodeId2(v_Node2Id);

                // ...and add the connection to the object
                m_Object->AddConnection(v_Connection);
            }

            // Create connections between upper and lower surfaces of airfoil; create first half of a truss structure
            for(unsigned int v_Index2=1; v_Index2<(v_Airfoil->GetNumberCoordinates()/2-1); v_Index2=v_Index2+2)
            {
                cNode* v_Node1 = m_Object->GetNodePointerByIndex(v_Node0Idx + v_Index2);
                cNode* v_Node2 = m_Object->GetNodePointerByIndex(m_Object->GetNumberNodes() - v_Index2 - 1);

                unsigned int v_Node1Id = v_Node1->GetID();
                unsigned int v_Node2Id = v_Node2->GetID();

                cConnection* v_Connection = new cConnection(m_Object, m_DampingConstant, m_SpringConstant);
                v_Connection->SetNodeId1(v_Node1Id);
                v_Connection->SetNodeId2(v_Node2Id);

                m_Object->AddConnection(v_Connection);
            }

            // Create connections between upper and lower surfaces of airfoil; create second half of a truss structure
            for(unsigned int v_Index2=3; v_Index2<v_Airfoil->GetNumberCoordinates()/2; v_Index2=v_Index2+2)
            {
                cNode* v_Node1 = m_Object->GetNodePointerByIndex(v_Node0Idx + v_Index2);
                cNode* v_Node2 = m_Object->GetNodePointerByIndex(m_Object->GetNumberNodes() - v_Index2 + 1);

                unsigned int v_Node1Id = v_Node1->GetID();
                unsigned int v_Node2Id = v_Node2->GetID();

                cConnection* v_Connection = new cConnection(m_Object, m_DampingConstant, m_SpringConstant);
                v_Connection->SetNodeId1(v_Node1Id);
                v_Connection->SetNodeId2(v_Node2Id);

                m_Object->AddConnection(v_Connection);
            }

            // Create connections between upper and lower surfaces of airfoil; create vertical bars of the truss structure
            for(unsigned int v_Index2=1; v_Index2<v_Airfoil->GetNumberCoordinates()/2; v_Index2++)
            {
                cNode* v_Node1 = m_Object->GetNodePointerByIndex(v_Node0Idx + v_Index2);
                cNode* v_Node2 = m_Object->GetNodePointerByIndex(m_Object->GetNumberNodes() - v_Index2);

                unsigned int v_Node1Id = v_Node1->GetID();
                unsigned int v_Node2Id = v_Node2->GetID();

                cConnection* v_Connection = new cConnection(m_Object, m_DampingConstant, m_SpringConstant);
                v_Connection->SetNodeId1(v_Node1Id);
                v_Connection->SetNodeId2(v_Node2Id);

                m_Object->AddConnection(v_Connection);
            }
        }

        // Create connections between airfoils
        for(unsigned int v_Index=0; v_Index<this->GetNumberSections()-1; v_Index++)
        {
            cAirfoil* v_Airfoil1 = this->GetAirfoilPointer(v_Index);
            cAirfoil* v_Airfoil2 = this->GetAirfoilPointer(v_Index+1);

            if(v_Airfoil1->GetNumberCoordinates() != v_Airfoil2->GetNumberCoordinates())
            {
                std::cout << "ERROR in cWing::Wing2Object: incompatible number of coordinates" << std::endl;
            }
            else
            {
                unsigned int v_NumberNodes = v_Airfoil1->GetNumberCoordinates();
                for(unsigned int v_Index2=0; v_Index2<v_NumberNodes; v_Index2++)
                {
                    unsigned int v_Node1Idx = v_Index*v_NumberNodes + v_Index2;
                    unsigned int v_Node2Idx = v_Node1Idx + v_NumberNodes;

                    cNode* v_Node1 = m_Object->GetNodePointerByIndex(v_Node1Idx);
                    cNode* v_Node2 = m_Object->GetNodePointerByIndex(v_Node2Idx);

                    unsigned int v_Node1Id = v_Node1->GetID();
                    unsigned int v_Node2Id = v_Node2->GetID();

                    cConnection* v_Connection = new cConnection(m_Object, m_DampingConstant, m_SpringConstant);
                    v_Connection->SetNodeId1(v_Node1Id);
                    v_Connection->SetNodeId2(v_Node2Id);

                    m_Object->AddConnection(v_Connection);
                }
            }
        }
    }
}
