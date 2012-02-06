// Copyright 2011 Yandex

#ifndef LTR_CLIENT_LTR_CLIENT_H_
#define LTR_CLIENT_LTR_CLIENT_H_

#include <boost/format.hpp>

#include <string>
#include <map>
#include <fstream>
#include <vector>

#include "utility/logger.h"
#include "tinyxml/tinyxml.h"

#include "ltr/parameters_container/parameters_container.h"

#include "ltr_client/constants.h"
#include "ltr_client/visitors.h"

#include "ltr_client/datas_info.h"
#include "ltr_client/learners_info.h"
#include "ltr_client/measures_info.h"

#include "ltr_client/measure_factory.h"
#include "ltr_client/learner_factory.h"
#include "ltr_client/splitter_factory.h"

#include "ltr/data/utility/io_utility.h"
#include "ltr/crossvalidation/crossvalidation.h"
#include "ltr/crossvalidation/tk_fold_simple_splitter.h"

using ltr::cv::Splitter;
using ltr::cv::Validate;
using ltr::cv::ValidationResult;

using boost::format;
using std::map;

class TrainVisitor;
/**
@class LtrClient
This class contains all information and methods for runing Ltr Client.
*/
class LtrClient {
    public:
        LtrClient();
        /** Function loads config from file into programm memory.
        */
        void loadConfig(std::string config_file);
        /** Function loads all data objects given in config.
        */
        void loadData();
        /** Function loads all measures given in config.
        */
        void loadMeasures();
        /** Function loads all splitters given in config.
        */
        void loadSplitters();
        /** Function loads all learners given in config.
        */
        void loadLearners();
        /** Function, which starts learning and making reports.
        */
        void launch();
        /** Function saves warning and error into file
        */
        void saveWarnings();

    private:
        friend class TrainVisitor;
        boost::shared_ptr<TiXmlDocument> config_;
        TiXmlElement *root_;
        std::string root_path_;
        logger::PrintLogger client_logger_;

        MeasureFactory measure_initer;
        LearnerFactory learner_initer;
        SplitterFactory splitter_initer;

        std::map<std::string, VDataInfo> datas;
        std::map<std::string, VLearnerInfo> learners;
        std::map<std::string, VMeasureInfo> measures;
        std::map<std::string, VSplitterInfo> splitters;
        std::map<std::string, ltr::Scorer::Ptr> scorers;

        typedef map<std::string, VDataInfo>::iterator data_iterator;
        typedef map<std::string, VLearnerInfo>::iterator learner_iterator;
        typedef map<std::string, VMeasureInfo>::iterator measure_iterator;
        typedef map<std::string, VSplitterInfo>::iterator splitter_iterator;

        /** Function loads data objects for particular approach.
        */
        template <class TElement> void loadDataImpl();
        /** Function loads learners for particular approach.
        */
        template <class TElement> void loadLearnersImpl();
        /** Function loads measures for particular approach.
        */
        template <class TElement> void loadMeasuresImpl();
        /** Function loads splitters for particular approach.
        */
        template <class TElement> void loadSplittersImpl();
        /**
        Function loads one learner with given name and info.
        @param name - learner name
        @param info - information about learner
        */
        template <class TElement>
        void loadLearnerImpl(const std::string& name);

        /** Function checks learners graph for cycles and approaches conflicts
        */
        void checkLearnersInfo();
        /** Function used in checkLearnersInfo().
         * Checks learner and its weak_learner if exists
         */
        void dfsCheck(std::string name, std::string approach = "");
        std::map<std::string, int> dfs_colors_;

        /** Inits Measure with given name
         * @param type - type of measure to init
         * @param parameters - parameters to init measure
         */
        VMeasureInfo initMeasure(std::string type,
                                 ltr::ParametersContainer parameters);

        /** Functions loads parameters from <parameters> tag
        */
        ltr::ParametersContainer loadParameters(TiXmlElement* params);
        /** Function parses string into int
        */
        int parseInt(std::string val);
        /** Function parses string into bool
        */
        bool parseBool(std::string val);
        /** Function parses string into double
        */
        double parseDouble(std::string val);
        /** Function parses string into list of int
        */
        vector<int> parseList(std::string val);

        // Functions for training and generating report

        /** Function executes one <train> command
         * @param command - Xml Element for command to execute
         */
        void makeTrain(TiXmlElement* command);

        /** Function executes one <report> command
         * @param command - Xml Element for command to execute
         */
        void makeCrossvalidation(TiXmlElement* command);

        /** Function executes one <report> command
         * @param command - Xml Element for command to execute
         */
        template<class TElement>
        void makeCrossvalidationImpl(
            std::string fold,
            std::map<string, VLearnerInfo> r_learners,
            std::map<string, VMeasureInfo> r_measures,
            std::map<string, VDataInfo> r_datas);
        /**
         * Function builds text table of given data.
         */
        std::string buildTable(std::vector<std::vector<std::string> > table);

        /** Function saves scorer code and predictions for given data.
         * @param command - Xml Element for command
         * @param name - filename to save code and predictictions
         */
        void saveCodeAndPredicts(TiXmlElement* command, const char* name);

        /** Function starts training for given learner and data.
         * @param name - scorer name
         * @param l_info - information about learner
         * @param d_info - information about data
         */
        template<class TElement>
        void train(std::string name,
                   LearnerInfo<TElement> l_info,
                   DataInfo<TElement> d_info);
};

template <class TElement>
void LtrClient::loadMeasuresImpl() {
  for (measure_iterator i = measures.begin(); i != measures.end(); i++) {
    if (boost::apply_visitor(GetApproachVisitor(), i->second) ==
                                                Approach<TElement>::name()) {
      MeasureInfo<TElement> info;
      info.type = boost::apply_visitor(GetTypeVisitor(), i->second);
      info.parameters = boost::apply_visitor(GetParametersVisitor(), i->second);
      info.measure = measure_initer.init<TElement>(info.type, info.parameters);
      info.approach = Approach<TElement>::name();
      i->second = info;

      client_logger_.info() << "created measure '"
                            << i->first << "', type: " << info.type
                            << " approach = " << info.approach
                            << " parameters: " << info.parameters.getString()
                            << std::endl;
    }
  }
}

template <class TElement>
void LtrClient::loadSplittersImpl() {
  for (splitter_iterator i = splitters.begin(); i != splitters.end(); i++) {
    if (boost::apply_visitor(GetApproachVisitor(), i->second) ==
                                                Approach<TElement>::name()) {
      SplitterInfo<TElement> info;
      info.type = boost::apply_visitor(GetTypeVisitor(), i->second);
      info.parameters = boost::apply_visitor(GetParametersVisitor(), i->second);
      info.splitter =
          splitter_initer.init<TElement>(info.type, info.parameters);
      info.approach = Approach<TElement>::name();
      i->second = info;

      client_logger_.info() << "created splitter '"
                            << i->first << "', type: " << info.type
                            << " parameters: " << info.parameters.getString()
                            << std::endl;
    }
  }
}

template <class TElement>
void LtrClient::loadDataImpl() {
    for (data_iterator i = datas.begin(); i != datas.end(); i++) {
      if (boost::apply_visitor(GetApproachVisitor(), i->second) ==
                                                  Approach<TElement>::name()) {
        DataInfo<ltr::Object> tm_info =
                                 boost::get<DataInfo<ltr::Object> >(i->second);
        DataInfo<TElement> info;
        info.approach = tm_info.approach;
        info.data_file = tm_info.data_file;
        info.format = tm_info.format;

        info.data = ltr::io_utility::loadDataSet<TElement>(info.data_file,
                                                               info.format);

        client_logger_.info() << "Loaded data '" << i->first <<
            "' from " << info.data_file << " as " << info.format
            << "(" << info.approach << ")" << std::endl;
        i->second = info;
      }
    }
}

template <class TElement>
void LtrClient::loadLearnersImpl() {
    for (learner_iterator i = learners.begin(); i != learners.end(); i++) {
        try {
            LearnerInfo<ltr::Object> tm_info =
                           boost::get<LearnerInfo<ltr::Object> >(i->second);
            if (tm_info.approach == Approach<TElement>::name()
                && tm_info.learner == NULL) {
                    loadLearnerImpl<TElement>(i->first);
            }
        } catch(boost::bad_get) {
        }
    }
}

template <class TElement>
void LtrClient::loadLearnerImpl(const std::string& name) {
    try {
        VLearnerInfo vinfo = learners[name];
        LearnerInfo<ltr::Object> tm_info =
                           boost::get<LearnerInfo<ltr::Object> >(vinfo);
        if (tm_info.approach == Approach<TElement>::name()
            && tm_info.learner == NULL) {
                LearnerInfo<TElement> info;
                info.measure_name = tm_info.measure_name;
                info.approach = tm_info.approach;
                info.type = tm_info.type;
                info.weak_learner_name = tm_info.weak_learner_name;
                info.parameters = tm_info.parameters;

                if (info.measure_name != "") {
                    try {
                        info.measure = boost::get<MeasureInfo<TElement> >
                                      (measures[tm_info.measure_name]).measure;
                    } catch(boost::bad_get) {
                        throw std::logic_error
                            ("measure approach conflict in learner " + name);
                    }
                }
                if (info.weak_learner_name != "") {
                    loadLearnerImpl<TElement>(info.weak_learner_name);
                    info.weak_learner = (boost::get<LearnerInfo<TElement> >
                      (learners[info.weak_learner_name])).learner;
                }
                info.learner = learner_initer.init<TElement>
                    (info.type, info.parameters);
                info.learner->setMeasure(info.measure);
                info.learner->setWeakLearner(info.weak_learner);
                info.learner->checkParameters();
                learners[name] = info;
                client_logger_.info() << "created learner "
                                      << name << std::endl;
        }
    } catch(boost::bad_get) {
    }
}

template<class TElement>
void LtrClient::train(std::string name,
                   LearnerInfo<TElement> l_info,
                   DataInfo<TElement> d_info) {
    client_logger_.info() << "Launching train " << name << std::endl;
    l_info.learner->learn(d_info.data);
    client_logger_.info() << "Train " << name << " finished. Report:"
                          << l_info.learner->report() << std::endl;
    scorers[name] = l_info.learner->makeScorerPtr();
}

template<class TElement>
void LtrClient::makeCrossvalidationImpl(
            std::string fold,
            std::map<string, VLearnerInfo> r_learners,
            std::map<string, VMeasureInfo> r_measures,
            std::map<string, VDataInfo> r_datas) {
  std::vector<typename ltr::BaseLearner<TElement>::Ptr> learners;
  std::vector<typename ltr::Measure<TElement>::Ptr> measures;
  std::vector<ltr::DataSet<TElement> > datas;

  std::vector<std::string> learner_names;
  std::vector<std::string> measure_names;
  std::vector<std::string> data_names;
  typename Splitter<TElement>::Ptr splitter;

  if (boost::apply_visitor(GetApproachVisitor(), splitters[fold])
    != Approach<TElement>::name()) {
    client_logger_.error() << "Failed: fold approach conflict"
                           << " in <crossvalidation>"
                           << std::endl;
    return;
  }
  splitter = (boost::get<SplitterInfo<TElement> >(splitters[fold])).splitter;
  for (learner_iterator it = r_learners.begin();
       it != r_learners.end(); it++) {
       learners.push_back
          ((boost::get<LearnerInfo<TElement> >(it->second)).learner);
       learner_names.push_back(it->first);
  }
  for (measure_iterator it = r_measures.begin();
       it != r_measures.end(); it++) {
       measures.push_back
          ((boost::get<MeasureInfo<TElement> >(it->second)).measure);
       measure_names.push_back(it->first);
  }
  for (data_iterator it = r_datas.begin();
       it != r_datas.end(); it++) {
       datas.push_back
          ((boost::get<DataInfo<TElement> >(it->second)).data);
       data_names.push_back(it->first);
  }

  client_logger_.info() << "Running crossvalidation " << std::endl;

  std::vector<double> results;
  for (size_t i = 0; i < learners.size(); i++)
    for (size_t j = 0; j < datas.size(); j++) {
      ValidationResult res =
          Validate(datas[j], measures, learners[i], splitter);
      for (size_t mi = 0; mi < measures.size(); mi++) {
        double sum = 0;
        for (size_t si = 0; si < res.getSplitCount(); si++)
          sum += res.getMeasureValues(si)[mi];

        results.push_back(sum / res.getSplitCount());
      }
    }

  std::vector<std::vector<std::string> > table;
  if (learners.size() == 1) {
    table.resize(datas.size() + 1);
    table[0].push_back(" ");
    for (size_t j = 0; j < measures.size(); j++)
      table[0].push_back(measure_names[j]);
    for (size_t i = 0; i < datas.size(); i++) {
      table[i+1].push_back(data_names[i]);
      for (size_t j = 0; j < measures.size(); j++) {
        table[i+1].push_back
          (str(boost::format("%.4f") % results[j + i * measures.size()]));
      }
    }
  } else if (measures.size() == 1) {
    table.resize(datas.size() + 1);
    table[0].push_back(" ");
    for (size_t j = 0; j < learners.size(); j++)
      table[0].push_back(learner_names[j]);
    for (size_t i = 0; i < datas.size(); i++) {
      table[i+1].push_back(data_names[i]);
      for (size_t j = 0; j < learners.size(); j++) {
        table[i+1].push_back
          (str(boost::format("%.4f") % results[j + i * learners.size()]));
      }
    }
  } else {
    table.resize(learners.size() + 1);
    table[0].push_back(" ");
    for (size_t j = 0; j < measures.size(); j++)
      table[0].push_back(measure_names[j]);
    for (size_t i = 0; i < learners.size(); i++) {
      table[i+1].push_back(learner_names[i]);
      for (size_t j = 0; j < measures.size(); j++) {
        table[i+1].push_back
          (str(boost::format("%.4f") % results[j + i * measures.size()]));
      }
    }
  }
  buildTable(table);
}

#endif  // LTR_CLIENT_LTR_CLIENT_H_
