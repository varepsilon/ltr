// Copyright 2012 Yandex

#ifndef LTR_DECISION_TREE_UTILITY_UTILITY_H_
#define LTR_DECISION_TREE_UTILITY_UTILITY_H_

#include <vector>
#include <string>

#include "ltr/data/data_set.h"
#include "ltr/decision_tree/condition.h"

using std::string;

namespace ltr {
namespace decision_tree {

void split(DataSet<Object> data,
           vector<Condition::Ptr> conditions,
           vector<DataSet<Object> >* datas);

template<class T>
struct ClassName {
  static string name() {return "Unknown";}
};

#define CLASS_NAME_INST(T) \
  template<> \
  string ClassName<T>::name() {return #T;}
}
}

#endif  // LTR_DECISION_TREE_UTILITY_UTILITY_H_
