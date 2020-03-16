#include "Lorenz.h"

Lorenz::Lorenz() : Equation(3, 3), sigma_(parameters_[0]), 
            r_(parameters_[1]), b_(parameters_[2]) {
    sigma_ = 10.0;
    r_ = 28.0;
    b_ = 8.0 / 3.0;
}

Lorenz::~Lorenz() {

}

void Lorenz::RHS(double t, const double * q, double * dq) const {
    dq[0] = sigma_ * (q[1] - q[0]);
    dq[1] = q[0] * (r_ - q[2]) - q[1];
    dq[2] = q[0] * q[1] - b_ * q[2];
}
