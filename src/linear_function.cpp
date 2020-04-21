#include "linear_function.h"

LinearFunction operator+(const LinearFunction& lhs, const LinearFunction& rhs) {
  return LinearFunction{lhs.t_minus + rhs.t_minus, lhs.t_plus + rhs.t_plus};
}

LinearFunction operator-(const LinearFunction& lhs, const LinearFunction& rhs) {
  return LinearFunction{lhs.t_minus - rhs.t_minus, lhs.t_plus - rhs.t_plus};
}

LinearFunction operator*(double scalar, const LinearFunction& rhs) {
  return LinearFunction{scalar * rhs.t_minus, scalar * rhs.t_plus};
}
