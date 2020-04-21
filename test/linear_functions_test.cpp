#include "gtest/gtest.h"
#include "linear_function.h"

TEST(LinearFunctions, evaluation) {
  double t_minus = 2.0;
  double t_plus = 3.0;

  LinearFunction f_x_plus_3(t_minus + 3, t_plus + 3);
  LinearFunction f_2x_plus_3(2 * t_minus + 3, 2 * t_plus + 3);
  LinearFunction f_2x(2 * t_minus, 2 * t_plus);
  LinearFunction f_x(t_minus, t_plus);
  LinearFunction f_3(3.0, 3.0);

  // Test that addition/subtraction/multiplcation work as expected

  // operator*
  auto f_test = 2 * f_x;
  EXPECT_DOUBLE_EQ(f_test.t_minus, f_2x.t_minus);
  EXPECT_DOUBLE_EQ(f_test.t_plus, f_2x.t_plus);

  // operator+
  f_test = f_x + f_x_plus_3;
  EXPECT_DOUBLE_EQ(f_test.t_minus, f_2x_plus_3.t_minus);
  EXPECT_DOUBLE_EQ(f_test.t_plus, f_2x_plus_3.t_plus);

  f_test = f_x + f_3;
  EXPECT_DOUBLE_EQ(f_test.t_minus, f_x_plus_3.t_minus);
  EXPECT_DOUBLE_EQ(f_test.t_plus, f_x_plus_3.t_plus);

  // operator-
  f_test = f_2x_plus_3 - f_x;
  EXPECT_DOUBLE_EQ(f_test.t_minus, f_x_plus_3.t_minus);
  EXPECT_DOUBLE_EQ(f_test.t_plus, f_x_plus_3.t_plus);

  // operator +=
  f_test = f_x + f_3;
  f_test += f_x;
  EXPECT_DOUBLE_EQ(f_test.t_minus, f_2x_plus_3.t_minus);
  EXPECT_DOUBLE_EQ(f_test.t_plus, f_2x_plus_3.t_plus);

  // operator-=
  f_test = 2 * f_x + f_3;
  f_test -= f_x;
  EXPECT_DOUBLE_EQ(f_test.t_minus, f_x_plus_3.t_minus);
  EXPECT_DOUBLE_EQ(f_test.t_plus, f_x_plus_3.t_plus);

  // operator*=
  f_test = f_x;
  f_test *= 2.0;
  EXPECT_DOUBLE_EQ(f_test.t_minus, f_2x.t_minus);
  EXPECT_DOUBLE_EQ(f_test.t_plus, f_2x.t_plus);

  auto slope = f_x_plus_3.slope(t_minus, t_plus);
  EXPECT_DOUBLE_EQ(slope, 1.0);
  slope = f_2x.slope(t_minus, t_plus);
  EXPECT_DOUBLE_EQ(slope, 2.0);
  slope = f_3.slope(t_minus, t_plus);
  EXPECT_DOUBLE_EQ(slope, 0.0);

  auto offset = f_x_plus_3.offset(t_minus, t_plus);
  EXPECT_DOUBLE_EQ(offset, 3.0);
  offset = f_2x.offset(t_minus, t_plus);
  EXPECT_DOUBLE_EQ(offset, 0.0);
  offset = f_3.offset(t_minus, t_plus);
  EXPECT_DOUBLE_EQ(offset, 3.0);
}
