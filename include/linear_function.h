#pragma once
#include <memory>
#include "graph.h"
#include "subset.h"

// Defines a linear function by storing f(t_minus) and f(t_plus)
// Since we only evaluate the functions at these times, this avoids needing to compute f(t) = a*t + b
struct LinearFunction {
  double t_minus;
  double t_plus;

  LinearFunction() : t_minus(0.), t_plus(0.) {}
  LinearFunction(double tminus, double tplus) : t_minus(tminus), t_plus(tplus) {}

  inline LinearFunction& operator+=(const LinearFunction& rhs) {
    this->t_minus += rhs.t_minus;
    this->t_plus += rhs.t_plus;
    return *this;
  }

  inline LinearFunction& operator-=(const LinearFunction& rhs) {
    this->t_minus -= rhs.t_minus;
    this->t_plus -= rhs.t_plus;
    return *this;
  }

  inline LinearFunction& operator*=(double scalar) {
    this->t_minus *= scalar;
    this->t_plus *= scalar;
    return *this;
  }

  inline double offset(double t_minus, double t_plus) const {
    // f(0) =  f(t1) - t1 * slope;
    return this->t_minus - t_minus * this->slope(t_minus, t_plus);

  }

  inline double slope(double t_minus, double t_plus) const {
    // slope = (f(t1) - f(t2) )/ (t1 - t2)
    return (this->t_plus - this->t_minus) / (t_plus - t_minus);
  }
};

LinearFunction operator*(double scalar, const LinearFunction& rhs);
LinearFunction operator+(const LinearFunction& lhs, const LinearFunction& rhs);
LinearFunction operator-(const LinearFunction& lhs, const LinearFunction& rhs);


// Used to store a pair of linear functions
struct LinearFunctionPair {
  LinearFunction first;
  LinearFunction second;
};

// TODO: Rename LinearFunctions to represent their relationship to dual constraints.
struct EdgeFunctions {
  std::shared_ptr<Edge> edge;
  LinearFunction first;
  LinearFunction second;
  std::shared_ptr<Subset> p1;
  std::shared_ptr<Subset> p2;
};

