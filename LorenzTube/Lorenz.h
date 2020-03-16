#ifndef LORENZ_H
#define LORENZ_H

#include "Equation.h"

class Lorenz : public Equation {
public:
    Lorenz();
    ~Lorenz();

    void RHS(double t, const double * q, double * dq) const;

private:
    double & sigma_, & r_, & b_;
};

#endif 
