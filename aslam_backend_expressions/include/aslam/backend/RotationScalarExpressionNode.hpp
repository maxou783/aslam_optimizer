/*
 * RotationScalarExpressionNode.hpp
 *
 *  Created on: Oct 5, 2014
 *      Author: hannes
 */

#ifndef ROTATIONSCALAREXPRESSIONNODE_HPP_
#define ROTATIONSCALAREXPRESSIONNODE_HPP_

#include "RotationExpressionNode.hpp"
#include "ScalarExpression.hpp"
#include "ScalarExpressionNode.hpp"

namespace aslam {
  namespace backend {

    class RotationScalarExpressionNode : public RotationExpressionNode {
    public:
      RotationScalarExpressionNode(int axis, const ScalarExpression & s) : _axis(axis), _s(s.root()){
        SM_ASSERT_GE_LT(std::runtime_error, axis, 0, 3, "The axis index must be in {0, 1, 2}");
      }
      ~RotationScalarExpressionNode() override{}

    private:
      Eigen::Matrix3d toRotationMatrixImplementation() const override;
      void evaluateJacobiansImplementation(JacobianContainer & outJacobians) const override;
      void getDesignVariablesImplementation(DesignVariable::set_t & designVariables) const override;

      int _axis;
      boost::shared_ptr<ScalarExpressionNode> _s;
    };

  } // namespace backend
} // namespace aslam


#endif /* ROTATIONSCALAREXPRESSIONNODE_HPP_ */
