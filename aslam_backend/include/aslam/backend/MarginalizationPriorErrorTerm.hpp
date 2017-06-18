/*
 * FLSPriorErrorTerm.hpp
 *
 *  Created on: Apr 26, 2013
 *      Author: mbuerki
 */

#ifndef MARGINALIZATIONPRIORERRORTERM_HPP_
#define MARGINALIZATIONPRIORERRORTERM_HPP_

#include <aslam/backend/ErrorTermDs.hpp>
#include <aslam/backend/DesignVariable.hpp>

#include <Eigen/Core>

#include <boost/shared_ptr.hpp>

#include <list>
#include <vector>

namespace aslam {
namespace backend {

class MarginalizationPriorErrorTerm : public aslam::backend::ErrorTermDs {
public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW

	typedef boost::shared_ptr<aslam::backend::MarginalizationPriorErrorTerm> Ptr;

  // creates the marginalization error term of the form e(x) = d - R*x
  // designVariables: 	the design variables of this marginalization prior error term
  MarginalizationPriorErrorTerm(const std::vector<DesignVariable*>& designVariables,
                                const Eigen::VectorXd& d,
                                const Eigen::MatrixXd& R);

  ~MarginalizationPriorErrorTerm() override;

  int numDesignVariables() { return _designVariables.size(); }
  aslam::backend::DesignVariable* getDesignVariable(int i);

private:
  MarginalizationPriorErrorTerm();

  double evaluateErrorImplementation() override;
  void evaluateJacobiansImplementation(JacobianContainer & outJ) override;

  // computes the minimal difference of all design variables between the linearization point at marginalization and the current guess (i.e. log(x_bar - x))
  Eigen::VectorXd getDifferenceSinceMarginalization();

  std::vector<DesignVariable*> _designVariables;
  Eigen::VectorXd _d;
  Eigen::MatrixXd _R; // R from the QR decomposition!!!
  int _dimensionDesignVariables;
  // store values of design variables at time of marginalization
  std::vector<Eigen::MatrixXd> _designVariableValuesAtMarginalization;
};

} /* namespace backend */
} /* namespace aslam */
#endif /* MARGINALIZATIONPRIORERRORTERM_HPP_ */
