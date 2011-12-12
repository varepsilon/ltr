// Copyright 2011 Yandex

#include <boost/lexical_cast.hpp>

#include <stdexcept>

#include "ltr/data/utility/nominal_feature_handler.h"

using boost::lexical_cast;

namespace ltr {
namespace io_utility {
  RemoveHandler::RemoveHandler() {
    max_feature_id = nominal_cnt = 0;
  }

  void RemoveHandler::init(const NominalFeatureInfo& info) {
    int cn = 0;
    if (info.feature_type_.size() == 0)
      return;
    max_feature_id = info.feature_type_.rbegin()->first;
    for (int i = 0; i <= max_feature_id; i++) {
      if (info.feature_type_.find(i) == info.feature_type_.end()) {
        feature_id[i] = cn++;
        continue;
      }
      NominalFeatureType type = info.feature_type_.find(i)->second;
      if (type != NUMERIC) {
        nominal_cnt++;
        feature_id[i] = -1;
      } else {
        feature_id[i] = cn++ - 1;
      }
    }

    feature_info_.setFeatureCount(max_feature_id - nominal_cnt, ltr::NUMERIC);
  }

  void RemoveHandler::process(map<int, string> features,
                              vector<double>* result) {
    if (max_feature_id < features.rbegin()->first) {
      max_feature_id = features.rbegin()->first;
      feature_info_.setFeatureCount(max_feature_id - nominal_cnt, ltr::NUMERIC);
    }
    result->clear();
    result->resize(max_feature_id - nominal_cnt, 0);

    for (int i = 0; i <= max_feature_id; i++) {
      if (feature_id.find(i) == feature_id.end())
        feature_id[i] = i - nominal_cnt - 1;
      if (feature_id[i] != -1 && features.find(i) != features.end())
        try {
          result->at(feature_id[i]) = lexical_cast<double>(features[i]);
      } catch(boost::bad_lexical_cast err) {
        throw std::logic_error("can't parse " + features[i] + " as double");
      }
    }
  }
};
};
