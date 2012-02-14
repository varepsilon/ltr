// Copyright 2011 Yandex

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <cmath>

#include "ltr_client/ltr_client.h"
#include "ltr_client/visitors.h"
#include "ltr_client/train_visitor.h"
#include "utility/timer.h"
#include "ltr/data/utility/io_utility.h"

template <> std::string Approach<ltr::Object>::name() {return PW;}
template <> std::string Approach<ltr::ObjectPair>::name() {return PRW;}
template <> std::string Approach<ltr::ObjectList>::name() {return LW;}

using boost::algorithm::to_upper;
using std::string;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "config file missing";
        return 1;
    }
    LtrClient client;
    try {
        client.loadConfig(argv[1]);
        client.loadData();
        client.loadMeasures();
        client.loadSplitters();
        client.loadLearners();
        client.launch();
        client.saveWarnings();
    } catch(std::logic_error err) {
        std::cerr << "Failed: " << err.what();
    }
    return 0;
}

LtrClient::LtrClient() {
}

void LtrClient::loadConfig(std::string config_file) {
    config_ = boost::shared_ptr<TiXmlDocument>
                                    (new TiXmlDocument(config_file.c_str()));
    if (!config_->LoadFile())
        throw std::logic_error("not valid config in " + config_file);
    root_ = config_->FirstChildElement("LTR_experiment");
    if (!root_)
        throw std::logic_error("can't find <LTR_experiment>");

    TiXmlElement* config = root_->FirstChildElement("config");
    if (!config)
        throw std::logic_error("can't find <config>");

    TiXmlElement* root_dir = config->FirstChildElement("root_directory");
    if (!root_dir || !root_dir->GetText())
        throw std::logic_error("no root directory specified");
    root_path_ = root_dir->GetText();
    logger::Logger::Get().Init(root_path_ + "ltr_client.log");

    client_logger_.info() << std::endl << std::string(80, '-') << std::endl
                          << " LTR Client. Copyright 2011 Yandex" << std::endl
                          << " Experiment started "
                          << timer::formatTime() << std::endl
                          << std::string(80, '-') << std::endl;
}

void LtrClient::loadData() {
    TiXmlElement* data_element = root_->FirstChildElement("data");
    while (data_element) {
        const char* name = data_element->Attribute("name");
        const char* format = data_element->Attribute("format");
        const char* approach = data_element->Attribute("approach");
        const char* data_path = data_element->GetText();

        if (!name)
            throw std::logic_error("<data> with no 'name' attribute");
        if (!format)
            throw std::logic_error("<data> with no 'format' attribute");
        if (!data_path)
            throw std::logic_error("data '" +
                                std::string(name) + "' has no file path");
        if (!approach) {
            client_logger_.warning() << "No approach defined for data '"
                    << name << "'. It will be used as listwise." << std::endl;
            approach = "listwise";
        }

        if (datas.find(name) != datas.end())
            throw std::logic_error("dublicate data name " + std::string(name));
        // This is a temporary object. It will be converted into the right type
        // in the loadDataImpl<type>()
        DataInfo<ltr::Object> info;
        info.approach = approach;
        info.data_file = data_path;
        info.format = format;
        datas[name] = info;
        data_element = data_element->NextSiblingElement("data");
    }
    loadDataImpl<ltr::Object>();
    loadDataImpl<ltr::ObjectPair>();
    loadDataImpl<ltr::ObjectList>();
}

void LtrClient::loadMeasures() {
    TiXmlElement* measures_elem = root_->FirstChildElement("measures");
    if (!measures_elem) {
        client_logger_.warning() << "no measures found" << std::endl;
        return;
    }
    TiXmlElement* measure_elem = measures_elem->FirstChildElement("measure");
    if (!measure_elem) {
        client_logger_.warning() << "no measures found" << std::endl;
        return;
    }
    while (measure_elem) {
        const char* name = measure_elem->Attribute("name");
        const char* type = measure_elem->Attribute("type");

        if (!name)
            throw std::logic_error("<measure> with no 'name' attribute");
        if (!type)
            throw std::logic_error("<measure> with no 'type' attribute");

        if (measures.find(name) != measures.end())
            throw std::logic_error("dublicate measure name "
                                                        + std::string(name));

        ltr::ParametersContainer parameters =
              loadParameters(measure_elem->FirstChildElement("parameters"));

        // This is a temporary object. It will be converted into the right type
        // in the loadMeasuresImpl<type>()
        MeasureInfo<ltr::Object> info;
        info.type = type;
        info.approach = measure_initer.getApproach(type);
        info.parameters = parameters;
        measures[name] = info;

        measure_elem = measure_elem->NextSiblingElement("measure");
    }
    loadMeasuresImpl<ltr::Object>();
    loadMeasuresImpl<ltr::ObjectPair>();
    loadMeasuresImpl<ltr::ObjectList>();
}

void LtrClient::loadSplitters() {
    TiXmlElement* splitters_elem = root_->FirstChildElement("splitters");
    if (!splitters_elem) {
        client_logger_.warning() << "no splitters found" << std::endl;
        return;
    }
    TiXmlElement* splitter_elem = splitters_elem->FirstChildElement("splitter");
    if (!splitter_elem) {
        client_logger_.warning() << "no splitters found" << std::endl;
        return;
    }
    while (splitter_elem) {
        const char* name = splitter_elem->Attribute("name");
        const char* type = splitter_elem->Attribute("type");
        const char* approach = splitter_elem->Attribute("approach");

        if (!name)
            throw std::logic_error("<splitter> with no 'name' attribute");
        if (!type)
            throw std::logic_error("<splitter> with no 'type' attribute");
        if (!approach) {
            client_logger_.warning() << "No approach defined for splitter '"
                          << name
                          << "'. It will be defined automatically if possible."
                          << std::endl;
            approach = "";
        }

        if (splitters.find(name) != splitters.end())
            throw std::logic_error("dublicate splitter name "
                                                        + std::string(name));

        ltr::ParametersContainer parameters =
              loadParameters(splitter_elem->FirstChildElement("parameters"));

        // This is a temporary object. It will be converted into the right type
        // in the loadSplittersImpl<type>()
        SplitterInfo<ltr::Object> info;
        info.type = type;
        info.approach = splitter_initer.getApproach(type, approach);
        info.parameters = parameters;
        splitters[name] = info;

        splitter_elem = splitter_elem->NextSiblingElement("splitter");
    }
    loadSplittersImpl<ltr::Object>();
    loadSplittersImpl<ltr::ObjectPair>();
    loadSplittersImpl<ltr::ObjectList>();
}


void LtrClient::loadLearners() {
    TiXmlElement* learners_elem = root_->FirstChildElement("learners");
    if (!learners_elem) {
        client_logger_.warning() << "no learners found" << std::endl;
        return;
    }
    TiXmlElement* learner_elem = learners_elem->FirstChildElement("learner");
    if (!learner_elem) {
        client_logger_.warning() << "no learners found" << std::endl;
        return;
    }
    while (learner_elem) {
        const char* name = learner_elem->Attribute("name");
        const char* type = learner_elem->Attribute("type");
        const char* approach = learner_elem->Attribute("approach");
        const char* weak_learner_name = 0;
        const char* measure = "";

        if (!name)
            throw std::logic_error("<learner> with no 'name' attribute");
        if (!type)
            throw std::logic_error("<learner> with no 'type' attribute");

        if (!approach) {
            client_logger_.warning() << "No approach defined for learner '"
                          << name
                          << "'. It will be defined automatically if possible."
                          << std::endl;
            approach = "";
        }

        if (learners.find(name) != learners.end())
            throw std::logic_error("dublicate learner name "
                                                        + std::string(name));

        TiXmlElement* weak_learner_elem =
                               learner_elem->FirstChildElement("weak_learner");
        if (weak_learner_elem)
            weak_learner_name = weak_learner_elem->GetText();

        TiXmlElement* measure_elem =
                               learner_elem->FirstChildElement("measure");
        if (measure_elem)
            measure = measure_elem->GetText();

        ltr::ParametersContainer parameters =
                loadParameters(learner_elem->FirstChildElement("parameters"));

        LearnerInfo<ltr::Object> info;
        info.type = type;
        boost::to_upper(info.type);
        info.approach = learner_initer.getApproach(info.type, approach);
        info.parameters = parameters;
        if (measure)
            info.measure_name = measure;
        if (weak_learner_name) {
            info.weak_learner_name = weak_learner_name;
            boost::to_upper(info.weak_learner_name);
        }
        learners[name] = info;
        client_logger_.info() << "found learner '"
                              << name << "': "
                              << info.type << "(" << info.approach
                              << ") parameters: "
                              << parameters.getString() << std::endl;

        learner_elem = learner_elem->NextSiblingElement("learner");
    }
    checkLearnersInfo();
    loadLearnersImpl<ltr::Object>();
    loadLearnersImpl<ltr::ObjectPair>();
    loadLearnersImpl<ltr::ObjectList>();
}

void LtrClient::checkLearnersInfo() {
    for (learner_iterator i = learners.begin(); i != learners.end(); i++)
        if (dfs_colors_[i->first] == 0)
            dfsCheck(i->first);
}

void LtrClient::dfsCheck(std::string name, std::string approach) {
    if (learners.find(name) == learners.end())
        throw std::logic_error("unknown learner " + name);
    if (dfs_colors_[name] == 1)
        throw std::logic_error("learners cycle found in " + name + "learner");
    dfs_colors_[name] = 1;

    LearnerInfo<ltr::Object> info =
                        boost::get<LearnerInfo<ltr::Object> >(learners[name]);

    if (info.measure_name != "" &&
        measures.find(info.measure_name) == measures.end())
        throw std::logic_error("unknown measure "
                                + info.measure_name + " for learner " + name);

    if (approach == "")
        approach = info.approach;
    if (info.approach != approach)
        throw std::logic_error("approach conflict: " + name
                                            + " learner must be " + approach);
    if (info.weak_learner_name != "")
        dfsCheck(info.weak_learner_name, approach);

    dfs_colors_[name] = 2;
}

int LtrClient::parseInt(std::string val) {
    try {
        return boost::lexical_cast<int>(val);
    } catch(boost::bad_lexical_cast) {
        throw std::logic_error(val + std::string("is not int"));
    }
}

double LtrClient::parseDouble(std::string val) {
    try {
        return boost::lexical_cast<double>(val);
    } catch(boost::bad_lexical_cast) {
        throw std::logic_error(val + std::string("is not double"));
    }
}

bool LtrClient::parseBool(std::string val) {
    if (val == "true")
        return true;
    else if (val == "false")
        return false;
    else
        throw std::logic_error(val + std::string("is not bool"));
}

vector<int> LtrClient::parseList(std::string val) {
    vector<int> res;
    std::stringstream str(val);
    int tm, tm2;
    char tmch;
    while (str >> tm) {
      if (str.peek() == '-') {
        if (!(str >> tmch >> tm2))
          throw std::logic_error("list format error: " + val);
        for (int a = tm; a <= tm2; a++)
          res.push_back(a);
      } else {
        res.push_back(tm);
      }
      if (!str.eof() && str.peek() != ' ')
        throw std::logic_error("list format error: " + val);
    }
    return res;
}

ltr::ParametersContainer LtrClient::loadParameters(TiXmlElement* params) {
    ltr::ParametersContainer res = ltr::ParametersContainer();
    if (!params)
        return res;
    TiXmlElement *param = params->FirstChildElement();
    while (param) {
        std::string name = param->Value();
        if (!param->GetText()) {
            client_logger_.warning() << "parameter "
                                     << name << " has no value" << std::endl;
            param = param->NextSiblingElement();
            continue;
        }
        std::string val = param->GetText();
        const char* type_ = param->Attribute("type");
        try {
            if (type_) {
                std::string type = type_;
                if (type == "int")
                    res.setInt(name, parseInt(val));
                else if (type == "double")
                    res.setDouble(name, parseDouble(val));
                else if (type == "list")
                    res.setList(name, parseList(val));
                else if (type == "bool")
                    res.setBool(name, parseBool(val));
                else
                    throw std::logic_error("Unknown parameter type " + type);
            } else {
                // Parameter is list if it has a ' '
                if (val.find(' ') < val.size()) {
                    res.setList(name, parseList(val));
                } else if (val[0] >= '0' && val[0] <= '9') {
                    // parameter is number
                    if (val.find('.') < val.size())
                        res.setDouble(name, parseDouble(val));
                    else
                        res.setInt(name, parseInt(val));
                } else {  // parameter is bool
                    res.setBool(name, parseBool(val));
                }
            }
        } catch(std::logic_error err) {
          throw std::logic_error("can't parse parameter " +
                                  name + ": " + err.what());
        }
        param = param->NextSiblingElement();
    }
    return res;
}

void LtrClient::saveWarnings() {
    std::string warns_file = root_path_ + "ltr_client.warn";
    std::ofstream warns(warns_file.c_str());
    client_logger_.warning().Output(&warns);
}

void LtrClient::launch() {
    TiXmlElement* launch = root_->FirstChildElement("launch");
    if (!launch)
        throw std::logic_error("<launch> not found");
    TiXmlElement* command = launch->FirstChildElement();
    while (command) {
        const char* com_ = command->Value();
        if (!com_) {
            command = command->NextSiblingElement();
            continue;
        }
        std::string com = com_;
        if (com == "train")
            makeTrain(command);
        else if (com == "crossvalidation")
            makeCrossvalidation(command);

        command = command->NextSiblingElement();
    }
}

void LtrClient::makeTrain(TiXmlElement* command) {
    const char* name = command->Attribute("name");
    const char* data = command->Attribute("data");
    const char* learner = command->Attribute("learner");
    if (!name) {
        client_logger_.error() << "Failed: <train> without name attribute"
                               << std::endl;
        return;
    }
    if (!data) {
        client_logger_.error() << "Failed: <train> without data attribute"
                               << std::endl;
        return;
    }
    if (!learner) {
        client_logger_.error() << "Failed: <train> without learner attribute"
                               << std::endl;
        return;
    }
    client_logger_.info() << "Train command, learner:"
                          << learner << " data:" << data
                          << " name:" << name << std::endl;
    if (learners.find(learner) == learners.end()) {
        client_logger_.error() << "Failed: unknown learner "
                               << learner << std::endl;
        return;
    }
    if (datas.find(data) == datas.end()) {
        client_logger_.error() << "Failed: unknown data "
                               << data << std::endl;
        return;
    }
    if (scorers.find(name) != scorers.end()) {
        client_logger_.error() << "Failed: dublicate train name "
                               << name << std::endl;
        return;
    }

    std::string learner_approach = boost::apply_visitor(GetApproachVisitor(),
                                                        learners[learner]);
    std::string data_approach = boost::apply_visitor(GetApproachVisitor(),
                                                     datas[data]);

    if (learner_approach != data_approach) {
        client_logger_.error() << "Failed: approach conflict" << std::endl;
        return;
    }
    boost::apply_visitor(TrainVisitor(name, this),
                         learners[learner], datas[data]);

    saveCodeAndPredicts(command, name);
}

void LtrClient::saveCodeAndPredicts(TiXmlElement* command,
                                    const char* name) {
    TiXmlElement* predict = command->FirstChildElement("predict");
    while (predict) {
        const char* predict_data = predict->GetText();
        if (!predict_data) {
            client_logger_.error() << "<predict> with no data" << std::endl;
            predict = predict->NextSiblingElement("predict");
            continue;
        }
        if (datas.find(predict_data) == datas.end()) {
          client_logger_.error() << "Can't predict. Unknown data "
                                 << predict_data << std::endl;
          predict = predict->NextSiblingElement("predict");
          continue;
        }

        std::string file_path = root_path_ + name + "."
                                            + predict_data + ".predicts";
        boost::apply_visitor(SavePredictionsVisitor(scorers[name], file_path),
                             datas[predict_data]);

        client_logger_.info() << "saved predictions for '" << predict_data
                                        << "' into " << file_path << std::endl;
        predict = predict->NextSiblingElement("predict");
    }
    if (command->FirstChildElement("cpp")) {
        std::string file_path = root_path_ + name + ".cpp";
        std::ofstream fout(file_path.c_str());
        fout << scorers[name]->generateCppCode(name);
        fout.close();
        client_logger_.info() << "cpp code saved into "
                              << file_path << std::endl;
    }
    client_logger_.info() << "command completed" << std::endl;
}

void LtrClient::makeCrossvalidation(TiXmlElement *command) {
    TiXmlElement* elem;
    std::string approach;
    std::map<string, VLearnerInfo> r_learners;
    std::map<string, VMeasureInfo> r_measures;
    std::map<string, VDataInfo> r_datas;

    const char* fold_ = command->Attribute("fold");
    if (!fold_) {
        client_logger_.error() << "Failed: <crossvalidation> with no fold"
                               << std::endl;
        return;
    }
    std::string fold = fold_;
    if (splitters.find(fold) == splitters.end()) {
        client_logger_.error() << "Failed: <crossvalidation>"
                               << " with unknown fold '" << fold
                               << "'" << std::endl;
        return;
    }
    elem = command->FirstChildElement("learner");
    while (elem) {
        const char* lname = elem->GetText();
        if (lname && learners.find(lname) != learners.end()) {
            if (r_learners.size() == 0 ||
                boost::apply_visitor(GetApproachVisitor(),
                                    learners[lname]) == approach)
                r_learners[lname] = learners[lname];
            if (r_learners.size() == 1)
                approach = boost::apply_visitor(GetApproachVisitor(),
                                                learners[lname]);
        }
        elem = elem->NextSiblingElement("learner");
    }
    if (r_learners.size() == 0) {
        client_logger_.error() << "Failed: <crossvalidation> with no learners"
                               << std::endl;
        return;
    }

    elem = command->FirstChildElement("measure");
    while (elem) {
        const char* mname = elem->GetText();
        if (mname && measures.find(mname) != measures.end())
            if (boost::apply_visitor(GetApproachVisitor(),
                                     measures[mname]) == approach)
                r_measures[mname] = measures[mname];
        elem = elem->NextSiblingElement("measure");
    }
    if (r_measures.size() == 0) {
        client_logger_.error() << "Failed: <crossvalidation> with no measures"
                               << std::endl;
        return;
    }

    elem = command->FirstChildElement("data");
    while (elem) {
        const char* dname = elem->GetText();
        if (dname && datas.find(dname) != datas.end())
            if (boost::apply_visitor(GetApproachVisitor(),
                                     datas[dname]) == approach)
                r_datas[dname] = datas[dname];
        elem = elem->NextSiblingElement("data");
    }
    if (r_datas.size() == 0) {
        client_logger_.error() << "Failed: <crossvalidation> with no datas"
                               << std::endl;
        return;
    }

    if ((r_learners.size() > 1) +
        (r_measures.size() > 1) +
        (r_datas.size() > 1) > 2) {
      client_logger_.error() << "Failed: <crossvalidation> "
                             << "can create only one table"
                             << std::endl;
      return;
    }

    if (approach == Approach<ltr::Object>::name())
      return makeCrossvalidationImpl<ltr::Object>
                          (fold, r_learners, r_measures, r_datas);
    else if (approach == Approach<ltr::ObjectPair>::name())
      return makeCrossvalidationImpl<ltr::ObjectPair>
                          (fold, r_learners, r_measures, r_datas);
    if (approach == Approach<ltr::ObjectList>::name())
      return makeCrossvalidationImpl<ltr::ObjectList>
                          (fold, r_learners, r_measures, r_datas);
}

std::string LtrClient::buildTable(std::vector<std::vector<string> > table) {
  std::string str;

  std::vector<size_t> colWidth;
  for (size_t i = 0; i < table[0].size(); i++) {
    size_t wd = 0;
    for (size_t j = 0; j < table.size(); j++)
      wd = std::max(wd, table[j][i].size());
    colWidth.push_back(wd);
  }

  str.append(string(colWidth[0], ' '))
     .append("|");
  for (size_t i = 1; i < table[0].size(); i++)
    str.append(table[0][i])
       .append(string(colWidth[i] - table[0][i].size(), ' '))
       .append("|");
  client_logger_.info() << str << std::endl;
  str.clear();

  str.append(string(colWidth[0], '-'))
     .append("+");
  for (size_t i = 1; i < table[0].size(); i++)
    str.append(string(colWidth[i], '-'))
       .append("+");
  client_logger_.info() << str << std::endl;
  str.clear();

  for (size_t i = 1; i < table.size(); i++) {
    str.append(table[i][0])
       .append(string(colWidth[0] - table[i][0].size(), ' '))
       .append("|");
    for (size_t j = 1; j < table[0].size(); j++)
      str.append(table[i][j])
         .append(string(colWidth[j] - table[i][j].size(), ' '))
         .append("|");
    client_logger_.info() << str << std::endl;
    str.clear();
  }
  return "";
}
