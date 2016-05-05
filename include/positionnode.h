#ifndef CPOSITIONNODE_H
#define CPOSITIONNODE_H

#include "node.h"
#include "object.h"
#include "vector.h"

class cPositionNode : public cNode
{
    public:
        cPositionNode();
        cPositionNode(cObject* p_Parent, double p_Mass, cVector p_Position);
        cPositionNode(cObject* p_Parent, double p_Mass, double p_X, double p_Y, double p_Z);
        virtual ~cPositionNode();

        virtual void  CalculateState(double p_TimeStep);

    protected:

    private:
};

#endif // CPOSITIONNODE_H
