#include <sm/eigen/gtest.hpp>
#include <aslam/backend/util/ProblemManager.hpp>
#include "SampleDvAndError.hpp"

using namespace std;
using namespace aslam::backend;

TEST(OptimizationProblemTestSuite, testProblemManager)
{
  // Create one squared error term with an inactive design variable
  Point2d dv0(Eigen::Vector2d::Random());
  LinearErr err0(&dv0);
  dv0.setActive(false);

  // Create one non-squared error term with an active design variable
  Point2d dv1(Eigen::Vector2d::Random());
  TestNonSquaredError err1(&dv1, TestNonSquaredError::grad_t::Random());
  dv1.setActive(true);

  boost::shared_ptr<OptimizationProblem> problem(new OptimizationProblem());
  problem->addDesignVariable(&dv0, false);
  problem->addDesignVariable(&dv1, false);
  problem->addErrorTerm(&err0, false);
  problem->addErrorTerm(&err1, false);

  ProblemManager pm;
  pm.setProblem(problem);

  ASSERT_FALSE(pm.isInitialized());
  pm.initialize();

  ASSERT_TRUE(pm.isInitialized());
  ASSERT_EQ(1, pm.numDesignVariables()); // only one active
  ASSERT_EQ(2, pm.numOptParameters()); // one active two-dimensional
  ASSERT_EQ(2, pm.numErrorTerms());

  JacobianContainer jc1(err1.dimension());
  err1.evaluateWeightedJacobians(jc1);
  RowVectorType grad1 = jc1.asDenseMatrix();

  RowVectorType grad = RowVectorType::Zero(pm.numOptParameters());

  // Gradient should be zero, design variable is not activated
  pm.addGradientForErrorTerm(grad, &err0, true);
  EXPECT_TRUE(grad.isApprox(RowVectorType::Zero(pm.numOptParameters()))) << "grad:" << endl << grad << endl;

  // Gradient should be equal to the one of the scalar error term
  grad.setZero();
  pm.addGradientForErrorTerm(grad, &err1, true);
  EXPECT_TRUE(grad.isApprox(grad1)) << "grad:" << endl << grad << endl << "expected:" << endl << grad1 << endl;

  // Full gradient should be equal to the one of the scalar error term
  grad.setZero();
  pm.computeGradient(grad, 1, true);
  EXPECT_TRUE(grad.isApprox(grad1)) << "grad:" << endl << grad << endl << "expected:" << endl << grad1 << endl;

  // Now activate design variable 0
  dv0.setActive(true);
  pm.initialize();
  ASSERT_EQ(2, pm.numDesignVariables());
  ASSERT_EQ(4, pm.numOptParameters());
  grad = RowVectorType::Zero(pm.numOptParameters());

  // Compute expected gradient for squared error
  JacobianContainer jc0(err0.dimension());
  err0.getWeightedJacobians(jc0, true );
  ColumnVectorType ev;
  err0.updateRawSquaredError();
  err0.getWeightedError(ev, true);
  RowVectorType grad0 = 2.0*ev.transpose()*jc0.asDenseMatrix();

  // check gradient for squared error term
  RowVectorType grad_expected = RowVectorType::Zero(pm.numOptParameters());
  grad_expected.segment(0, 2) = grad0;
  pm.addGradientForErrorTerm(grad, &err0, true);
  EXPECT_TRUE(grad.isApprox(grad_expected)) << "grad:" << endl << grad << endl << "expected:" << endl << grad_expected << endl;

  // Full gradient should now be the sum of both
  grad.setZero();
  grad_expected.setZero();
  grad_expected.segment(0, 2) = grad0;
  grad_expected.segment(2, 2) = grad1;
  pm.computeGradient(grad, 1, true);
  EXPECT_TRUE(grad.isApprox(grad_expected)) << "grad:" << endl << grad << endl << "expected:" << endl << grad_expected<< endl;
}