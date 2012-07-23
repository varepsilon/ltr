// Copyright 2012 Yandex

#ifndef LTR_FEATURE_CONVERTERS_FEATURE_CONVERTER_H_
#define LTR_FEATURE_CONVERTERS_FEATURE_CONVERTER_H_

#include <vector>
#include <string>
#include <stdexcept>

#include "ltr/data/data_set.h"
#include "ltr/data/feature_info.h"
#include "ltr/data/object.h"
#include "ltr/data/per_object_accessor.h"

#include "ltr/interfaces/parameterized.h"
#include "ltr/interfaces/serializable.h"
#include "ltr/interfaces/aliaser.h"

#include "ltr/utility/shared_ptr.h"

using std::logic_error;

namespace ltr {
/**
 * \brief A base class for feature converters.
 *
 * Preprocesses Object, e.g. sample or linear transform features.
 *
 * Can be applied to each Object in DataSet.
 * But before doing this FeatureConverter should be trained by some FeatureConverterLearner.
 * This can be usefull for better training of Scorer
 * (Learner<TElement>::addFeatureConverterLearner).
 *
 * Feature converters which were used during training of Scorer
 * will be added in the result Scorer (see Scorer::addFeatureConverter)
 * and consequently must be Serializable.
 *
 * \note In most cases you don't need to create FeatureConverter directly,
 * normally it is the result of some FeatureConverterLearner work.
 *
 * \see FeatureConverterLearner, BaseFeatureConverterLearner, BaseLearner, Scorer
 */
class FeatureConverter : public Serializable,
                         public Aliaser {
 public:
  typedef ltr::utility::shared_ptr<FeatureConverter> Ptr;
  typedef ltr::utility::shared_ptr<const FeatureConverter> ConstPtr;

  FeatureConverter(const FeatureInfo& input_feature_info = FeatureInfo()) {
    input_feature_info_ =  input_feature_info;
  }

  virtual ~FeatureConverter() {}

  GET(FeatureInfo, input_feature_info);
  GET(FeatureInfo, output_feature_info);

  void set_input_feature_info(const FeatureInfo& input_feature_info) {
    input_feature_info_ = input_feature_info;

    // TO DO (what for?)
    fillOutputFeatureInfo();
  }
  /**
  * Fills output_feature_info_ (Object may have another FeatureInfo
  * after convertion, e.g. FeatureConverter may change the number of Object features)
  */
  virtual void fillOutputFeatureInfo() = 0;
  /**
   * Converts object features
   * \param input object to be converted
   * \param output converted object
   */
  void apply(const Object& input, Object* output) const {
    CHECK(input.feature_info() == input_feature_info_);
    applyImpl(input, output);
  }
  /**
   * Converts features of all objects in dataset
   * \param input object to be converted
   * \param output converted object
   */
  template<class TElement>
  void apply(const DataSet<TElement>& input, DataSet<TElement>* output) const;

 private:
  virtual void applyImpl(const Object& input, Object* output) const = 0;

 protected:
  /**
  * \brief A FeatureInfo that an input Object is supposed to have
  */
  FeatureInfo input_feature_info_;
  /**
  * \brief A FeatureInfo that an output Object will have
  */
  FeatureInfo output_feature_info_;
};

typedef std::vector<FeatureConverter::Ptr> FeatureConverterArray;

// template realization

template<class TElement>
void FeatureConverter::apply(const DataSet<TElement>& input,
                             DataSet<TElement>* output) const {
  *output = input.deepCopy();
  for (int element_index = 0; element_index < input.size(); ++element_index) {
    PerObjectAccessor<const TElement> input_element(&input.at(element_index));
    PerObjectAccessor<TElement> output_element(&output->at(element_index));
    for (int object_index = 0;
         object_index < input_element.object_count(); ++object_index) {
      apply(input_element.object(object_index),
            &output_element.object(object_index));
    }
  }
  output->set_feature_info(output_feature_info());
}
};
#endif  // LTR_FEATURE_CONVERTERS_FEATURE_CONVERTER_H_
