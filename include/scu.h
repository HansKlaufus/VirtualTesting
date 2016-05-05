#ifndef CSCU_H
#define CSCU_H

#include "iointerface.h"
#include "object.h"

class cScu : public cIoInterface
{
    public:
        cScu();
        virtual ~cScu();

        void                CalculateState(double p_TimeStep);
        virtual double      GetScalar(cIoInterface* p_Requester = NULL);

    protected:

    private:
        cObject             *m_ParentObject;
};

#endif // CSCU_H
