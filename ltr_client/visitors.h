// Copyright 2011 Yandex

#ifndef LTR_CLIENT_VISITORS_H_
#define LTR_CLIENT_VISITORS_H_

#include <string>

#include "ltr_client/constants.h"
#include "ltr_client/learners_info.h"
#include "ltr_client/datas_info.h"
#include "ltr_client/measures_info.h"
#include "ltr/data/utility/io_utility.h"

#include "boost/variant.hpp"
/**
@class GetApproachVisitor
This class is used to get Approach for VMeasureInfo, VDataInfo or VLearnerInfo.
*/
class GetApproachVisitor : public boost::static_visitor<std::string> {
    public:
        template<class TElement>
        std::string operator()(MeasureInfo<TElement>& info) const {
            return info.approach;
        }

        template<class TElement>
        std::string operator()(DataInfo<TElement>& info) const {
            return info.approach;
        }

        template<class TElement>
        std::string operator()(LearnerInfo<TElement>& info) const {
            return info.approach;
        }
};

/**
@class GetApproachVisitor
This class is used to get Approach for VMeasureInfo, VDataInfo or VLearnerInfo.
*/
class SavePredictionsVisitor : public boost::static_visitor<void> {
  private:
    ltr::Scorer::Ptr scorer;
    std::string file_path;
  public:
    SavePredictionsVisitor(ltr::Scorer::Ptr scorer, std::string file_path) :
        scorer(scorer), file_path(file_path) {}
    template<class TElement>
    void operator()(DataInfo<TElement>& info) const {
      ltr::io_utility::savePredictions(info.data, scorer, file_path);
    }
};

/**
@class GetTypeVisitor
This class is used to get type for VMeasureInfo or VLearnerInfo.
*/
class GetTypeVisitor : public boost::static_visitor<std::string> {
    public:
        template<class TElement>
        std::string operator()(MeasureInfo<TElement>& info) const {
            return info.type;
        }

        template<class TElement>
        std::string operator()(LearnerInfo<TElement>& info) const {
            return info.type;
        }
};

/**
@class GetParametersVisitor
This class is used to get Approach for VMeasureInfo or VLearnerInfo.
*/
class GetParametersVisitor
    : public boost::static_visitor<ltr::ParametersContainer> {
  public:
    template<class TElement>
    ltr::ParametersContainer operator()(MeasureInfo<TElement>& info) const {
      return info.parameters;
    }

    template<class TElement>
    ltr::ParametersContainer operator()(LearnerInfo<TElement>& info) const {
      return info.approach;
    }
};

/**
@class DataInfoVisitor
This class is used to get DataInfo from VDataInfo.
*/
class DataInfoVisitor : public boost::static_visitor<DataInfo<ltr::Object> > {
    public:
        template<class TElement>
        DataInfo<ltr::Object> operator()(DataInfo<TElement>& d_info) const {
            DataInfo<ltr::Object> res;
            res.data_file = d_info.data_file;
            res.format = d_info.format;
            res.approach = d_info.approach;
            return res;
        }
};

/**
@class SetApproachVisitor
This class is used to set right Approach for VMeasureInfo, VDataInfo
or VLearnerInfo
*/
class SetApproachVisitor : public boost::static_visitor<void> {
    public:
    template<class TElement>
    void operator()(MeasureInfo<TElement>& info) const {
        info.approach = Approach<TElement>::name();
    }

    template<class TElement>
    void operator()(DataInfo<TElement>& info) const {
        info.approach = Approach<TElement>::name();
    }

    template<class TElement>
    void operator()(LearnerInfo<TElement>& info) const {
        info.approach = Approach<TElement>::name();
    }
};

/**
@class SetTypeVisitor
This class is used to set type for VMeasureInfo or VLearnerInfo
*/
class SetTypeVisitor : public boost::static_visitor<void> {
    std::string type;

    public:
    explicit SetTypeVisitor(std::string type) : type(type) {}
    template<class TElement>
    void operator()(MeasureInfo<TElement>& info) const {
        info.type = type;
    }

    template<class TElement>
    void operator()(LearnerInfo<TElement>& info) const {
        info.type = type;
    }
};

#endif  // LTR_CLIENT_VISITORS_H_
