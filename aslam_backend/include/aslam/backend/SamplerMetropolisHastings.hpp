/*
 * SamplerMcmc.hpp
 *
 *  Created on: 24.07.2015
 *      Author: sculrich
 */

#ifndef INCLUDE_ASLAM_BACKEND_SAMPLERMETROPOLISHASTINGS_HPP_
#define INCLUDE_ASLAM_BACKEND_SAMPLERMETROPOLISHASTINGS_HPP_

#include <limits>

#include <sm/BoostPropertyTree.hpp>

#include "SamplerBase.hpp"
//#include "OptimizationProblemBase.hpp"
//#include "DesignVariable.hpp"
//#include "ErrorTerm.hpp"
//#include "ScalarNonSquaredErrorTerm.hpp"

namespace aslam {
namespace backend {

struct SamplerMetropolisHastingsOptions {
  SamplerMetropolisHastingsOptions();
  SamplerMetropolisHastingsOptions(const sm::PropertyTree& config);
  double transitionKernelSigma;  /// \brief Standard deviation for the Gaussian Markov transition kernel \f$ \\mathcal{N(\mathbf 0, \text{diag{\sigma^2})} f$
};

std::ostream& operator<<(std::ostream& out, const aslam::backend::SamplerMetropolisHastingsOptions& options);

/**
 * @class SamplerMetropolisHastings
 * @brief The sampler returns samples (design variables) of a probability distribution that cannot be directly sampled.
 * It interprets the objective value of an optimization problem as the negative log density of a probability distribution.
 * The log density has to be defined up to proportionality of the true negative log density.
 */
class SamplerMetropolisHastings : public SamplerBase {

 public:
  typedef boost::shared_ptr<SamplerMetropolisHastings> Ptr;
  typedef boost::shared_ptr<const SamplerMetropolisHastings> ConstPtr;

 public:
  /// \brief Default constructor with default options
  SamplerMetropolisHastings();
  /// \brief Constructor
  SamplerMetropolisHastings(const SamplerMetropolisHastingsOptions& options);
  /// \brief Destructor
  ~SamplerMetropolisHastings() { }

  /// \brief Mutable getter for options
  SamplerMetropolisHastingsOptions& options() { return _options; }

 private:
  virtual void runImplementation(const std::size_t nStepsMax, const std::size_t nAcceptedSamples, Statistics& statistics) override;

 private:
   SamplerMetropolisHastingsOptions _options; /// \brief Configuration options

};

} /* namespace aslam */
} /* namespace backend */

#endif /* INCLUDE_ASLAM_BACKEND_SAMPLERMETROPOLISHASTINGS_HPP_ */
