#include <iostream>

#include "connection.h"
#include "node.h"
#include "object.h"

using namespace std;

int main()
{
    cObject*        v_Object = new cObject();
    cNode*          v_Node1 = new cNode(v_Object, 1, 0, 0, 0);
    cNode*          v_Node2 = new cNode(v_Object, 10, 0, 0, 1);
    cNode*          v_Node3 = new cNode(v_Object, 0, 0, 0, 2);
    cConnection*    v_Connection1 = new cConnection(v_Object, 1000, 1000);
    cConnection*    v_Connection2 = new cConnection(v_Object, 0, 1000);

    v_Node1->AddConnection(v_Connection1->GetID());
    v_Node2->AddConnection(v_Connection1->GetID());
    v_Node2->AddConnection(v_Connection2->GetID());
    v_Node3->AddConnection(v_Connection2->GetID());

    v_Connection1->SetNodeId1(v_Node1->GetID());
    v_Connection1->SetNodeId2(v_Node2->GetID());
    v_Connection2->SetNodeId1(v_Node2->GetID());
    v_Connection2->SetNodeId2(v_Node3->GetID());

    v_Object->AddNode(v_Node1);
    v_Object->AddNode(v_Node2);
    v_Object->AddNode(v_Node3);
    v_Object->AddConnection(v_Connection1);
    v_Object->AddConnection(v_Connection2);

    v_Object->Initialise();
    v_Object->Start();

    return 0;
}
