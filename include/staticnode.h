#ifndef CSTATICNODE_H
#define CSTATICNODE_H

#include "node.h"
#include "object.h"
#include "vector.h"

class cStaticNode : public cNode
{
    public:
        cStaticNode();
        cStaticNode(cObject* p_Parent, double p_Mass, cVector p_Position);
        cStaticNode(cObject* p_Parent, double p_Mass, double p_X, double p_Y, double p_Z);
        virtual ~cStaticNode();

        virtual void  CalculateState(double p_TimeStep);

    protected:

    private:
};

#endif // CSTATICNODE_H
