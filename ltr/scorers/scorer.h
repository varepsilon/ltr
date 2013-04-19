// Copyright 2011 Yandex

#ifndef LTR_SCORERS_SCORER_H_
#define LTR_SCORERS_SCORER_H_

#include "ltr/utility/boost/lexical_cast.h"
#include <vector>
#include <string>

#include "ltr/interfaces/aliaser.h"
#include "ltr/data/object.h"
#include "ltr/feature_converters/feature_converter.h"
#include "ltr/interfaces/serializable_functor.h"
#include "ltr/interfaces/printable.h"
#include "ltr/utility/boost/shared_ptr.h"

using std::string;

namespace ltr {
/**
 * \brief Class that can for every object evaluate a value - its rank, or score.
 */
class Scorer : public SerializableFunctor<double>,
               public Printable,
               public Aliaser {
 public:
  typedef ltr::utility::shared_ptr<Scorer> Ptr;
  typedef ltr::utility::shared_ptr<Scorer> BasePtr;

  Scorer(const FeatureConverterArray&
            feature_converter = FeatureConverterArray()):
        feature_converter_(feature_converter) {}

  double value(const Object& object) const {
    return score(object);
  }

  double score(const Object& object) const;

  GET_SET_VECTOR_OF_PTR(FeatureConverter, feature_converter);

  using SerializableFunctor<double>::generateCppCode;

  string generateCppCode(const string& function_name) const;

  template <class TElement>
  void predict(const DataSet<TElement>& elements) const;

  template <class TElement>
  void predict(const TElement& element) const;

  string generateLocalClassName(size_t index) const;

  virtual ~Scorer() {}

  virtual string toString() const;
 private:
  virtual double scoreImpl(const Object& obj) const = 0;

  virtual string generateCppCodeImpl(const string& function_name) const = 0;

  FeatureConverterArray feature_converter_;

  virtual string toStringImpl() const;
};

template <class TElement>
void Scorer::predict(const DataSet<TElement>& elements) const {
  for (int i = 0; i < elements.size(); ++i) {
    predict(elements[i]);
  }
}

template <class TElement>
void Scorer::predict(const TElement& element) const {
  PerObjectAccessor<const TElement> per_object_accessor(&element);
  for (int object_index = 0;
       object_index < (int)per_object_accessor.object_count();
       ++object_index) {
    per_object_accessor.object(object_index).set_predicted_label(
      score(per_object_accessor.object(object_index)));
  }
}
}
#endif  // LTR_SCORERS_SCORER_H_
