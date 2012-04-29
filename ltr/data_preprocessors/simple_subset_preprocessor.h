// Copyright 2011 Yandex

#ifndef LTR_DATA_PREPROCESSORS_SIMPLE_SUBSET_PREPROCESSOR_H_
#define LTR_DATA_PREPROCESSORS_SIMPLE_SUBSET_PREPROCESSOR_H_

#include <boost/lexical_cast.hpp>
#include <boost/shared_ptr.hpp>

#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>

#include "ltr/data/data_set.h"
#include "ltr/data_preprocessors/data_preprocessor.h"

using std::max_element;
using std::vector;
using std::logic_error;
using std::string;
using ltr::utility::lightSubset;

namespace ltr {
  /**
   * Choses an element subset from inputted dataset. Throws while applying
   * if inputted dataset has too little elements (if one of elements to be chosen
   * has index with no element for this index in inputted dataset). Duplication of
   * resulted element's indices is allowed. (So e. g. a subset dataset can have
   * more elements than original dataset. Is used in begging.)
   */
  template <class TElement>
  class SimpleSubsetPreprocessor : public DataPreprocessor<TElement> {
  public:
    typedef boost::shared_ptr<SimpleSubsetPreprocessor> Ptr;

    explicit SimpleSubsetPreprocessor(
        const ParametersContainer& parameters = ParametersContainer()) {
      this->setDefaultParameters();
      this->copyParameters(parameters);
    }

    void setDefaultParameters();
    void checkParameters() const;

    void apply(const DataSet<TElement>& input,
      DataSet<TElement>* output) const;
  };

  // template realizations
  template <class TElement>
  void SimpleSubsetPreprocessor<TElement>::apply(
      const DataSet<TElement>& input_dataset,
      DataSet<TElement>* output_dataset) const {
    vector<int> indices = this->getListParameter("INDICES");
    if (indices.size() != 0) {
      int max_used_element = *max_element(indices.begin(), indices.end());

      if (max_used_element > input_dataset.size()) {
        throw logic_error("Current dataset has "
        + boost::lexical_cast<string>(input_dataset.size())
        + " elements while number "
        + boost::lexical_cast<string>(max_used_element)
        + " was requested");
      } else {
        *output_dataset = lightSubset(input_dataset, indices);
      }
    } else {
      *output_dataset = input_dataset;
    }
  }

  template <typename TElement>
  void SimpleSubsetPreprocessor<TElement>::setDefaultParameters() {
    this->clearParameters();
    vector<int> empty;
    this->addListParameter("INDICES", empty);
  }

  template <typename TElement>
  void SimpleSubsetPreprocessor<TElement>::checkParameters() const {
    vector<int> indices = this->getListParameter("INDICES");
    set<int> used_elements;
    for (int index = 0; index < indices.size(); ++index) {
      int current_object = indices[index];
      if (used_elements.find(current_object) == used_elements.end()) {
        used_elements.insert(current_object);
      } else {
        throw logic_error("Indicies array contains equal elements");
      }
    }
  }
};

#endif  // LTR_DATA_PREPROCESSORS_SIMPLE_SUBSET_PREPROCESSOR_H_
