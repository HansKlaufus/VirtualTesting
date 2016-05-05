#include <iostream>
#include <cmath>
#include <wx/app.h>

#include "vector.h"

unsigned int cVector::m_NextID = 0;

cVector::cVector()
{
    m_ID = cVector::m_NextID++;
}

cVector::cVector(double p_X, double p_Y)
{
    m_ID = cVector::m_NextID++;

    this->push_back(p_X);
    this->push_back(p_Y);
}

cVector::cVector(double p_X, double p_Y, double p_Z)
{
    m_ID = cVector::m_NextID++;

    this->push_back(p_X);
    this->push_back(p_Y);
    this->push_back(p_Z);
}

cVector::~cVector()
{
    //dtor
}

double cVector::GetLength()
{
    double v_Length = 0.0f;
    for(unsigned int v_Index=0; v_Index<this->size(); v_Index++)
    {
        v_Length += this->at(v_Index)*this->at(v_Index);
    }
    v_Length = sqrt(v_Length);

    return v_Length;
}

cVector cVector::GetNormalised()
{
    double v_Length = this->GetLength();

    cVector v_Vector;
    if(v_Length==0)
    {
        std::cout << "ERROR in cVector::GetNormalised: Zero length vector." << std::endl;
    }
    else
    {
        for(unsigned int v_Index=0; v_Index<this->size(); v_Index++)
        {
            v_Vector.push_back(this->at(v_Index) / v_Length);
        }
    }

    // DEBUG: Check for NaN
    v_Vector.IsNan();

    return v_Vector;
}

double cVector::GetAngle(cVector p_Vector)
{
    double v_Length1 = this->GetLength();
    double v_Length2 = p_Vector.GetLength();

    double v_Angle = 0;

    if(v_Length1 >0 && v_Length2>0)
    {
        double v_Temp = this->InnerProduct(p_Vector) / (this->GetLength() * p_Vector.GetLength());
        if(v_Temp >= -1 && v_Temp <= 1)
        {
            v_Angle = acos(v_Temp);
        }
    }

    return v_Angle;
}

cVector cVector::Add(cVector p_Vector)
{
    cVector v_Vector;
    for(unsigned int v_Index=0; v_Index<this->size(); v_Index++)
    {
        v_Vector.push_back(this->at(v_Index) + p_Vector.at(v_Index));
    }

    v_Vector.IsNan();

    return v_Vector;
}

cVector cVector::Subtract(cVector p_Vector)
{
    cVector v_Vector;
    for(unsigned int v_Index=0; v_Index<this->size(); v_Index++)
    {
        v_Vector.push_back(this->at(v_Index) - p_Vector.at(v_Index));
    }

    v_Vector.IsNan();

    return v_Vector;
}

double cVector::InnerProduct(cVector p_Vector)
{
    double v_Result = 0;
    for(unsigned int v_Index=0; v_Index<this->size(); v_Index++)
    {
        v_Result += this->at(v_Index) * p_Vector.at(v_Index);
    }

    return v_Result;
}

cVector cVector::OuterProduct(cVector p_Vector)
{
    cVector v_Vector;

    if(this->size() == 3)
    {
        v_Vector.push_back(this->at(1)*p_Vector.at(2) - this->at(2)*p_Vector.at(1));
        v_Vector.push_back(this->at(2)*p_Vector.at(0) - this->at(0)*p_Vector.at(2));
        v_Vector.push_back(this->at(0)*p_Vector.at(1) - this->at(1)*p_Vector.at(0));
    }

    v_Vector.IsNan();

    return v_Vector;
}

cVector cVector::ScalarProduct(double p_Scalar)
{
    cVector v_Vector;

    for(unsigned int v_Index=0; v_Index<this->size(); v_Index++)
    {
        v_Vector.push_back(this->at(v_Index) * p_Scalar);
    }

    v_Vector.IsNan();

    return v_Vector;
}

cVector cVector::Abs()
{
    cVector v_Vector;

    for(unsigned int v_Index=0; v_Index<this->size(); v_Index++)
    {
        v_Vector.push_back(std::abs(this->at(v_Index)));
    }

    v_Vector.IsNan();

    return v_Vector;
}

cVector cVector::operator+(cVector p_Vector)
{
    return this->Add(p_Vector);
}

cVector cVector::operator-(cVector p_Vector)
{
    return this->Subtract(p_Vector);
}

double cVector::operator*(cVector p_Vector)
{
    return this->InnerProduct(p_Vector);
}

cVector cVector::operator*(double p_Scalar)
{
    return this->ScalarProduct(p_Scalar);
}

cVector cVector::operator/(double p_Scalar)
{
    return this->ScalarProduct(1.0/p_Scalar);
}

void cVector::Print(void)
{
    std::cout << "[ ";
    for(unsigned int v_Index=0; v_Index<this->size(); v_Index++)
    {
        std::cout << this->at(v_Index) << "; ";
    }
    std::cout << " ]"<< std::endl;
}

bool cVector::IsNan(void)
{
    bool v_Result = false;

    for(unsigned int v_Index=0; v_Index<this->size(); v_Index++)
    {
        if(this->at(v_Index)!=this->at(v_Index))
        {
            v_Result = true;
            std::cout << "ERROR in cVector::IsNan: cVector is NaN." << std::endl;
        }
    }

    return v_Result;
}
