// Copyright 2011 Yandex

#ifndef LTR_DATA_UTILITY_NOMINAL_FEATURE_HANDLER_H_
#define LTR_DATA_UTILITY_NOMINAL_FEATURE_HANDLER_H_

#include <boost/shared_ptr.hpp>

#include <vector>
#include <map>
#include <string>

#include "ltr/data/feature_info.h"

using std::string;
using std::vector;
using std::map;

namespace ltr {
namespace io_utility {
  enum NominalFeatureType {
    NUMERIC,
    NOMINAL,
    BOOLEAN,
    META,
    CLASS,
  };
  struct NominalFeatureInfo {
    public:
      map<int, NominalFeatureType> feature_type_;
      map<int, string> feature_name_;
      map<int, vector<string> > feature_values_;
  };

  class NominalFeatureHandler {
    public:
      typedef boost::shared_ptr<NominalFeatureHandler> Ptr;
      virtual void init(const NominalFeatureInfo& info) = 0;
      virtual const FeatureInfo& featureInfo() = 0;
      virtual void process(map<int, string> features,
                           vector<double>* result) = 0;
      virtual ~NominalFeatureHandler() {}
  };

  class RemoveHandler : public NominalFeatureHandler {
    public:
      RemoveHandler();
      void init(const NominalFeatureInfo& info);
      void process(map<int, string> features, vector<double>* result);
      const FeatureInfo& featureInfo() {return feature_info_;}
    private:
      FeatureInfo feature_info_;
      map<int, int> feature_id;
      int max_feature_id;
      int nominal_cnt;
  };
};
};

#endif  // LTR_DATA_UTILITY_NOMINAL_FEATURE_HANDLER_H_
