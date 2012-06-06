// Copyright 2011 Yandex

#ifndef LTR_FEATURE_CONVERTERS_FEATURE_CONVERTER_H_
#define LTR_FEATURE_CONVERTERS_FEATURE_CONVERTER_H_

#include <boost/shared_ptr.hpp>
#include <vector>
#include <string>
#include <stdexcept>

#include "ltr/data/object.h"
#include "ltr/data/feature_info.h"
#include "ltr/data/data_set.h"
#include "ltr/interfaces/parameterized.h"
#include "ltr/interfaces/serializable.h"
#include "ltr/interfaces/aliaser.h"

using std::logic_error;

namespace ltr {

/**
* \brief A base class for feature converters.
* Preprocesses Object, e.g. sample or linear transform features.
*
* Can be applied to each Object in DataSet but before doing this
* FeatureConverter should be trained by FeatureConverterLearner.
* This can be usefull for better training of Scorer
* (see BaseLearner<TElement>::addFeatureConverterLearner).
*
* Feature converters wich were used during training of Scorer 
* will be added in the result Scorer (see Scorer::addFeatureConverter) 
* and consequently must be Serializable.
* 
* \note In most cases you don't need to create FeatureConverter directly,
* normally it is the result of FeatureConverterLearner work.
* 
* \sa FeatureConverterLearner, Learner, Scorer
*/
class FeatureConverter : public Serializable {
 public:
  typedef boost::shared_ptr<FeatureConverter> Ptr;
  typedef boost::shared_ptr<const FeatureConverter> ConstPtr;

  FeatureConverter(const FeatureInfo& input_feature_info = FeatureInfo()) {
    input_feature_info_ =  input_feature_info;
  }
  virtual ~FeatureConverter() {}

  GET(FeatureInfo, input_feature_info);
  void set_input_feature_info(const FeatureInfo &input_feature_info) { 
    input_feature_info_ = input_feature_info;
    fillOutputFeatureInfo();
  }
  GET(FeatureInfo, output_feature_info);

  /**
   * Converts object features
   * \param input object to be converted
   * \param output coverted object
   */
  void apply(const Object& input, Object* output) const {
    CHECK(input.feature_info() == input_feature_info_);
    applyImpl(input, output);
  }
 private:
  /**
  * Fills output_feature_info_ (Object may have another FeatureInfo
  * after convertion, e.g. FeatureConverter may change the number of Object features)
  */
  virtual void fillOutputFeatureInfo() = 0;
  virtual void applyImpl(const Object& input, Object* output) const = 0;
 protected:
  /// A FeatureInfo that an input Object is supposed to have
  FeatureInfo input_feature_info_;
  /// A FeatureInfo that an output Object will have
  FeatureInfo output_feature_info_;
};

typedef std::vector<FeatureConverter::Ptr> FeatureConverterArray;
};

#endif  // LTR_FEATURE_CONVERTERS_FEATURE_CONVERTER_H_
