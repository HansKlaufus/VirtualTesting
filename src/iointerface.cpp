#include "iointerface.h"

#include <iostream>

unsigned int cIoInterface::m_NextID = 0;

cIoInterface::cIoInterface()
{
    m_ID = cIoInterface::m_NextID++;
}

cIoInterface::~cIoInterface()
{
    //dtor
}

tIoVector cIoInterface::GetOutput(cIoInterface* p_Requester)
{
    tIoVector v_Output;

    cVector v_Force;
    cVector v_Acceleration;
    cVector v_Velocity;
    cVector v_Position;

    v_Force.assign(3, 0);
    v_Acceleration.assign(3, 0);
    v_Velocity.assign(3, 0);
    v_Position.assign(3, 0);

    v_Output.push_back(v_Force);
    v_Output.push_back(v_Acceleration);
    v_Output.push_back(v_Velocity);
    v_Output.push_back(v_Position);

    return v_Output;
}

cIoInterface* cIoInterface::GetSourcePointer(unsigned int p_Index)
{
    if(p_Index>m_SourceArray.size()-1)
    {
        std::cout << "ERROR in cIoInterface::GetSourcePointer: p_Index out of bounds [" << p_Index << "/" << m_SourceArray.size() << "]" << std::endl;
    }

    return m_SourceArray.at(p_Index);
}
