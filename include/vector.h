#ifndef CVECTOR_H
#define CVECTOR_H

#include <vector>

class cVector : public std::vector<double>
{
    public:
        cVector();
        cVector(double p_X, double p_Y);
        cVector(double p_X, double p_Y, double p_Z);
        virtual             ~cVector();

        unsigned int        GetID() { return m_ID; }

        double              GetLength();
        cVector             GetNormalised();
        double              GetAngle(cVector p_Vector);

        cVector             Add(cVector p_Vector);
        cVector             Subtract(cVector p_Vector);
        double              InnerProduct(cVector p_Vector);
        cVector             OuterProduct(cVector p_Vector);
        double              DotProduct(cVector p_Vector) { return InnerProduct(p_Vector); }
        cVector             CrossProduct(cVector p_Vector) { return OuterProduct(p_Vector); }
        cVector             ScalarProduct(double  p_Scalar);
        cVector             Abs();
        bool                IsNan();

        cVector             operator+(cVector p_Vector);
        cVector             operator-(cVector p_Vector);
        double              operator*(cVector p_Vector);
        cVector             operator*(double  p_Scalar);
        cVector             operator/(double  p_Scalar);

        void                Print();

    protected:

    private:
        static unsigned int m_NextID;
        unsigned int        m_ID;
};

#endif // CVECTOR_H
