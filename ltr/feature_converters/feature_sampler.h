// Copyright 2012 Yandex

#ifndef LTR_FEATURE_CONVERTERS_FEATURE_SAMPLER_H_
#define LTR_FEATURE_CONVERTERS_FEATURE_SAMPLER_H_

#include <string>
#include <vector>
#include <algorithm>

#include "ltr/data/object.h"
#include "ltr/data/feature_info.h"
#include "ltr/data/data_set.h"

#include "ltr/utility/indices.h"
#include "ltr/utility/shared_ptr.h"

#include "ltr/feature_converters/feature_converter.h"

using std::max_element;
using std::vector;
using std::string;

using ltr::utility::Indices;
using ltr::utility::IndicesPtr;

namespace ltr {
/**
 * \brief Samples features with specified indices.
 * \param indices indices of features to sample
 */
class FeatureSampler : public FeatureConverter {
  public:
  typedef ltr::utility::shared_ptr<FeatureSampler> Ptr;
  /**
   * \param indices indices of features to sample
   */
  explicit FeatureSampler(const ParametersContainer& parameters) {
  }

  explicit FeatureSampler(const Indices& indices = Indices(),
                          const FeatureInfo& feature_info = FeatureInfo())
    : FeatureConverter(feature_info),
      indices_(indices) {}

  virtual FeatureInfo convertFeatureInfo() const;

  virtual string generateCppCode(const string& function_name) const;

  GET(Indices, indices);

  void set_indices(const Indices& indices);

  virtual string toString() const;
 private:
  void applyImpl(const Object& input, Object* output) const;

  virtual string getDefaultAlias() const;

  Indices indices_;
};
};
#endif  // LTR_FEATURE_CONVERTERS_FEATURE_SAMPLER_H_
