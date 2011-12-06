// Copyright 2011 Yandex

#ifndef LTR_SCORERS_SCORER_H_
#define LTR_SCORERS_SCORER_H_

#include <boost/shared_ptr.hpp>
#include <vector>
#include <string>

#include "ltr/interfaces.h"
#include "ltr/data/object.h"
#include "ltr/feature_converters/feature_converter.h"

namespace ltr {

class Scorer : public Aliaser, public IBriefer {
  public:
  typedef boost::shared_ptr<Scorer> Ptr;

  Scorer(const std::string& alias,
      const FeatureConverterArray& featureConverters = FeatureConverterArray()):
        Aliaser(alias),
        featureConverters_(featureConverters) {}
  virtual ~Scorer() {}

  double score(const Object& obj) const;
  double operator() (const Object& obj) const;

  virtual std::string generateCppCode(const std::string& class_name,
      int tabbing = 0) const = 0;
  virtual std::string generateJavaCode(const std::string& class_name,
      int tabbing = 0, bool is_static = false) const = 0;

  private:
  virtual double scoreImpl(const Object& obj) const = 0;

  virtual std::string generateCppCodeImpl(const std::string& class_name,
      int tabbing = 0) const = 0;
  virtual std::string generateJavaCodeImpl(const std::string& class_name,
      int tabbing = 0, bool is_static = false) const = 0;

  FeatureConverterArray featureConverters_;
};
}
#endif  // LTR_SCORERS_SCORER_H_
