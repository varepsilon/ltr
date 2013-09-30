// Copyright 2013 Yandex

#ifndef CONTRIB_RANDOM_CAUCHYDISTRIBUTION_HPP_
#define CONTRIB_RANDOM_CAUCHYDISTRIBUTION_HPP_

#include <cmath>

namespace RandomLib {
  /**
   * \brief The Cauchy distribution.
   *
   * Example \code
   #include <RandomLib/CauchyDistribution.hpp>

     RandomLib::Random r;
     std::cout << "Seed set to " << r.SeedString() << "\n";
     RandomLib::CauchyDistribution<double> cauchy_dist;
     std::cout << "Select from Cauchy distribution:";
     for (size_t i = 0; i < 10; ++i)
         std::cout << " " << cauchy_dist(r, 0.0, 1.0);
     std::cout << "\n";
   \endcode
   *
   * @tparam RealType the real type of the results (default double).
   **********************************************************************/
  template<typename RealType = double> class CauchyDistribution {
  public:
    /**
     * The type returned by CauchyDistribution::operator()(Random&)
     **********************************************************************/
    typedef RealType result_type;

    template<class Random>
    RealType operator()(Random& r, RealType location = RealType(0.0),  // NOLINT
                        RealType scale = RealType(1.0)) const throw();
  };

  template<typename RealType>  template<class Random> inline RealType
  CauchyDistribution<RealType>::operator()(Random& r, RealType location,  // NOLINT
                                                RealType scale) const throw() {
    return location + scale * std::tan(
        M_PI * (r.template FloatU<RealType>() - 0.5));
  }

}  // namespace RandomLib

#endif  // CONTRIB_RANDOM_CAUCHYDISTRIBUTION_HPP_
