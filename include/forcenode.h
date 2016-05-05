#ifndef CFORCENODE_H
#define CFORCENODE_H

#include "node.h"
#include "object.h"
#include "vector.h"

class cForceNode : public cNode
{
    public:
        cForceNode();
        cForceNode(cObject* p_Parent, double p_Mass, cVector p_Position);
        cForceNode(cObject* p_Parent, double p_Mass, double p_X, double p_Y, double p_Z);
        virtual ~cForceNode();

        virtual void  CalculateState(double p_TimeStep);

    protected:

    private:
};

#endif // CFORCENODE_H
