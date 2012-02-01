// Copyright 2011 Yandex

#ifndef LTR_CLIENT_LTR_CLIENT_H_
#define LTR_CLIENT_LTR_CLIENT_H_

#include <string>
#include <map>
#include <fstream>
#include <vector>

#include "utility/logger.h"
#include "tinyxml/tinyxml.h"

#include "ltr/parameters_container/parameters_container.h"

#include "ltr_client/constants.h"

#include "ltr_client/datas_info.h"
#include "ltr_client/learners_info.h"
#include "ltr_client/measures_info.h"

#include "ltr_client/measures_initer.h"
#include "ltr_client/learners_initer.h"

#include "ltr/data/utility/io_utility.h"

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
        MeasureIniter measure_initer;
        LearnerIniter learner_initer;

        std::map<std::string, VDataInfo> datas;
        std::map<std::string, VLearnerInfo> learners;
        std::map<std::string, VMeasureInfo> measures;
        std::map<std::string, ltr::Scorer::Ptr> scorers;

        typedef std::map<std::string, VDataInfo>::iterator data_iterator;
        typedef std::map<std::string, VLearnerInfo>::iterator learner_iterator;
        typedef std::map<std::string, VMeasureInfo>::iterator measure_iterator;

        /** Function loads data objects for particular approach.
        */
        template <class TElement> void loadDataImpl();
        /** Function loads learners for particular approach.
        */
        template <class TElement> void loadLearnersImpl();
        /** Function loads measures for particular approach.
        */
        template <class TElement> void loadMeasuresImpl();
        /**
        Function loads one learner with given name and info.
        @param name - learner name
        @param info - information about learner
        */
        template <class TElement>
        void loadLearnerImpl(const std::string& name,
                             const VLearnerInfo& info);

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
        void makeReport(TiXmlElement* command);

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


using boost::apply_visitor;

template <class TElement>
void LtrClient::loadMeasuresImpl() {
    for (measure_iterator i = measures.begin(); i != measures.end(); i++) {
      if (apply_visitor(GetApproachVisitor(), i->second) ==
                                                  Approach<TElement>::name()) {
        i->second = measure_initer.init<TElement>
              (apply_visitor(GetTypeVisitor(), i->second),
              apply_visitor(GetParametersVisitor(), i->second));
      }
    }
}


template <class TElement>
void LtrClient::loadDataImpl() {
    for (data_iterator i = datas.begin(); i != datas.end(); i++) {
        try {
          DataInfo<ltr::Object> tm_info =
                                 boost::get<DataInfo<ltr::Object> >(i->second);
          if (tm_info.approach == Approach<TElement>::name()) {
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
        } catch(boost::bad_get) {}
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
                    loadLearnerImpl<TElement>(i->first, tm_info);
            }
        } catch(boost::bad_get) {
        }
    }
}

template <class TElement>
void LtrClient::loadLearnerImpl(const std::string& name,
                                const VLearnerInfo& info) {
    try {
        LearnerInfo<ltr::Object> tm_info =
                           boost::get<LearnerInfo<ltr::Object> >(info);
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
                    loadLearnerImpl<TElement>(info.weak_learner_name,
                                              learners[info.weak_learner_name]);
                    info.weak_learner = (boost::get<LearnerInfo<TElement> >
                      (learners[info.weak_learner_name])).learner;
                }
                learners[name] = info = learner_initer.init(info);
                info.learner->checkParameters();
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

#endif  // LTR_CLIENT_LTR_CLIENT_H_
