// Copyright 2012 Yandex

#include "ltr/learners/decision_tree/utility/utility.h"

namespace ltr {
namespace decision_tree {
void split(DataSet<Object> data,
           std::vector<Condition::Ptr> conditions,
           std::vector<DataSet<Object> >* datas) {
  datas->clear();
  for (int j = 0; j < (int)conditions.size(); j++) {
    datas->push_back(DataSet<Object>((data.feature_info())));
  }

  for (int i = 0; i < (int)data.size(); ++i) {
    int best_index = -1;
    double best_value = 0;
    for (int j = 0; j < (int)conditions.size(); j++) {
      try {
        double value = conditions[j]->value(data[i]);
        if (best_index < 0 || value > best_value) {
          best_index = j;
          best_value = value;
        }
      } catch(std::logic_error error) {
      }
    }
    if (best_index >= 0) {
      (*datas)[best_index] << data[i];
    }
  }
}

template<> string ClassName<int>() {return "int";}
template<> string ClassName<double>() {return "double";}
template<> string ClassName<bool>() {return "bool";}
};
};
