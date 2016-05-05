#include <iostream>
#include <time.h>
#include <typeinfo>

#include "staticnode.h"

cStaticNode::cStaticNode()
{
    //ctor
}

cStaticNode::cStaticNode(cObject* p_ParentObject, double p_Mass, cVector p_Position) : cNode(p_ParentObject, p_Mass, p_Position)
{
}

cStaticNode::cStaticNode(cObject* p_ParentObject, double p_Mass, double p_X, double p_Y, double p_Z) : cNode(p_ParentObject, p_Mass, p_X, p_Y, p_Z)
{
}

cStaticNode::~cStaticNode()
{
    //dtor
}

void cStaticNode::CalculateState(double p_TimeStep)
{
    /*
    ** Static Node; nothing to do
    */
}
