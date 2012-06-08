// Copyright 2012 Yandex

#include "ltr/learners/decision_tree/utility/utility.h"

namespace ltr {
namespace decision_tree {

void split(DataSet<Object> data,
           std::vector<Condition::Ptr> conditions,
           std::vector<DataSet<Object> >* datas) {
  datas->clear();
  for (size_t j = 0; j < conditions.size(); j++)
    datas->push_back(DataSet<Object>((data.feature_info())));
  for (size_t i = 0; i < data.size(); i++) {
    int best_idx = -1;
    double best_value = 0;
    for (int j = 0; j < conditions.size(); j++) {
      try {
        double value = conditions[j]->value(data[i]);
        if (best_idx < 0 || value > best_value) {
          best_idx = j;
          best_value = value;
        }
      } catch(std::logic_error err) {
      }
    }
    if (best_idx >= 0)
      (*datas)[best_idx] << data[i];
  }
}

template<> string ClassName<int>() {return "int";}
template<> string ClassName<double>() {return "double";}
template<> string ClassName<bool>() {return "bool";}
}
}
