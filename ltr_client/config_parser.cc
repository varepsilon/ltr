// Copyright 2012 Yandex

#include "ltr_client/config_parser.h"

#include <stdexcept>

#include "rlog/rlog.h"
#include "tinyxml/tinyxml.h"

#include "ltr/utility/macros.h"
#include "ltr/utility/boost/string_utils.h"

using std::string;
using std::logic_error;

using ltr::utility::to_lower;
using ltr::utility::iequals;

SafeSet<string> ConfigParser::valid_tags_;

Configuration::Ptr ConfigParser::parse(const string& file_name) {
  shared_ptr<TiXmlDocument> document = new TiXmlDocument(file_name);
  CHECK_MSG(document->LoadFile(), "not valid config in " + file_name +
      " or file not found");

  TiXmlElement* root = document->FirstChildElement(ROOT);
  CHECK_MSG(root, "can't find <LTR_experiment>");

  parseTags(*(root->FirstChildElement()));

  rInfo("Configuration file loaded. Collected data:\n"
        "Data:\n%s"
        "Objects:\n%s"
        "Train launches:\n%s"
        "Crossvalidation launches:\n%s",
        configuration_->data_infos.toString().c_str(),
        configuration_->object_infos.toString().c_str(),
        configuration_->train_infos.toString().c_str(),
        configuration_->crossvalidation_infos.toString().c_str());

  for (ObjectInfo::ObjectInfos::iterator it =
       configuration_->object_infos.begin();
       it != configuration_->object_infos.end();
       ++it) {
    it->second->fill_dependency_list(configuration_->object_infos);
  }

  return configuration_;
}

void ConfigParser::parseTags(const TiXmlElement& element) {
  for (const TiXmlElement* current_element = &element;
       current_element;
       current_element = nextTiXmlElement(current_element)) {
    string tag_name = to_lower(current_element->Value());
    CHECK_MSG(valid_tags_.find(tag_name) != valid_tags_.end(),
        "Tag " + tag_name + " is not valid");
    if (tag_name == CONFIG) {
      parseConfig(*current_element);
    } else if (tag_name == DATA) {
      parseData(*current_element);
    } else if (tag_name == LAUNCH) {
      parseLaunch(*current_element);
    } else {
      parseParameterized(*current_element);
    }
  }
}

void ConfigParser::parseConfig(const TiXmlElement& element) {
  const TiXmlElement* root_dir = element.FirstChildElement();
  CHECK_MSG(root_dir, "no root directory specified");
  CHECK_MSG(to_lower(root_dir->Value()) == string(ROOT_DIR),
      string("unexpected tag ") + root_dir->Value() + " in <" + CONFIG +
      "> section");
  CHECK_MSG(root_dir->GetText(), "no root directory specified");

  configuration_->root_path = root_dir->GetText();
}

void ConfigParser::parseData(const TiXmlElement& element) {
  const char* name = element.Attribute(NAME_ATTR);
  const char* format = element.Attribute(FORMAT_ATTR);
  const char* approach = element.Attribute(APPROACH_ATTR);
  const char* file_name = element.GetText();

  CHECK_MSG(name, "<data> with no 'name' attribute");
  CHECK_MSG(format, "<data> with no 'format' attribute");
  CHECK_MSG(file_name, "data '" + string(name) + "' has no file path");
  if (!approach) {
    rWarning(
      "No approach defined for data '%s'. It will be used as listwise.", name);
    approach = LISTWISE;
  }

  configuration_->data_infos.safeInsert(to_lower(name)) =
    new DataInfo(to_lower(name),
                 to_lower(approach),
                 to_lower(format),
                 file_name);
}

void ConfigParser::parseParameterized(const TiXmlElement& element) {
  const char* tag_name = element.Value();
  const char* name = element.Attribute(NAME_ATTR);
  const char* type = element.Attribute(TYPE_ATTR);
  const char* approach = element.Attribute(APPROACH_ATTR);

  CHECK(tag_name);
  CHECK_MSG(name, string(tag_name) + " with no 'name' attribute");
  CHECK_MSG(type, string(tag_name) + " with no 'type' attribute");
  if (!approach) {
    rWarning("No approach defined for '%s'. Trying to define automatically.",
             name);
    approach = "";
  }

  ParametersContainer parameters;
  parseParameters(*(element.FirstChildElement()), &parameters);
  configuration_->object_infos.safeInsert(to_lower(name)) =
      new ObjectInfo(to_lower(tag_name),
                     to_lower(name),
                     to_lower(type),
                     to_lower(approach),
                     parameters);
}

void ConfigParser::parseParameters(const TiXmlElement& element,
                                   ParametersContainer* parameters) {
  for (const TiXmlElement* current_element = &element;
       current_element;
       current_element = nextTiXmlElement(current_element)) {
    const string name = current_element->Value();
    const string value = current_element->GetText() != NULL ?
        current_element->GetText() : "";

    CHECK_MSG(!value.empty(), "parameter " + string(name.c_str()) +
        " has no value");

    parameters->AddNew(to_lower(name), lexical_cast<Any>(value));
  }
}

void ConfigParser::parseLaunch(const TiXmlElement& xmlElement) {
  for (const TiXmlElement* current_element = xmlElement.FirstChildElement();
       current_element;
       current_element = nextTiXmlElement(current_element)) {
    string tag_name = current_element->Value();
    if (iequals(tag_name, TRAIN)) {
      TrainLaunchInfo::Ptr info(new TrainLaunchInfo());
      parseTrain(*current_element, info.get());
      configuration_->train_infos.safeInsert(info->name) = info;
    } else if (iequals(tag_name, CROSSVALIDATION)) {
      CrossvalidationLaunchInfo::Ptr info(new CrossvalidationLaunchInfo());
      parseCrossvalidation(*current_element, info.get());
      configuration_->crossvalidation_infos.safeInsert(info->splitter) = info;
    } else {
      throw logic_error("Undefined tag " + tag_name + " in launch section");
    }
  }
}

void ConfigParser::parseTrain(const TiXmlElement& element,
                              TrainLaunchInfo* info) {
  const char* name = element.Attribute("name");
  const char* data = element.Attribute("data");
  const char* learner = element.Attribute("learner");

  CHECK_MSG(name, "<train> tag with no name");
  CHECK_MSG(data, string(name) + " with no \"data\" attribute");
  CHECK_MSG(learner, string(name) + " with no \"learner\" attribute");

  info->name = to_lower(name);
  info->data = to_lower(data);
  info->learner = to_lower(learner);

  for (const TiXmlElement* current_element = element.FirstChildElement();
       current_element;
       current_element = nextTiXmlElement(current_element)) {
    string tag_name = to_lower(current_element->Value());
    if (tag_name == PREDICT) {
      const char* predict_name = current_element->GetText();
      CHECK_MSG(predict_name,
                string("empty <") + PREDICT + "> tag in train section");
      info->predicts.safeInsert(to_lower(predict_name));
    } else if (tag_name == CPP_GEN) {
      info->gen_cpp = true;
    } else {
      throw logic_error("Undefined tag " + tag_name + " in train "
          + name + " section");
    }
  }
}

void ConfigParser::parseCrossvalidation(const TiXmlElement& element,
                                        CrossvalidationLaunchInfo* info) {
  const char* fold = element.Attribute(FOLD_ATTR);
  CHECK_MSG(fold, "<crossvalidation> tag with no fold");
  info->splitter = to_lower(fold);

  for (const TiXmlElement* current_element = element.FirstChildElement();
       current_element;
       current_element = nextTiXmlElement(current_element)) {
    string tag_name = to_lower(current_element->Value());
    if (tag_name == LEARNER) {
      const char* learner_name = current_element->GetText();
      CHECK_MSG(learner_name, string("empty <") + LEARNER +
          "> tag in crossvalidation section");
      info->learners.safeInsert(to_lower(learner_name));
    } else if (tag_name == MEASURE) {
      const char* measure_name = current_element->GetText();
      CHECK_MSG(measure_name, string("empty <") + MEASURE +
          "> tag in crossvalidation section");
      info->measures.safeInsert(to_lower(measure_name));
    } else if (tag_name == DATA) {
      const char* data_name = current_element->GetText();
      CHECK_MSG(data_name, string("empty <") + DATA +
          "> tag in crossvalidation section");
      info->datas.safeInsert(to_lower(data_name));
    } else {
      throw logic_error("Undefined tag " + tag_name +
          " in crossvalidation fold " + fold + ".");
    }
  }
}

const TiXmlElement* ConfigParser::nextTiXmlElement(const TiXmlNode* node) {
  for (const TiXmlNode* next_node = node->NextSibling();
       next_node;
       next_node = node->NextSibling()) {
    const TiXmlElement* next_element = next_node->ToElement();
    if (next_element) {
      return next_element;
    }
  }
  return NULL;
}

void ConfigParser::initValidTags() {
  valid_tags_.insert(CONFIG);
  valid_tags_.insert(DATA);
  valid_tags_.insert(LAUNCH);
  valid_tags_.insert(LEARNER);
  valid_tags_.insert(MEASURE);
  valid_tags_.insert(SPLITTER);
  valid_tags_.insert(METRIC);
  valid_tags_.insert(PREDICTIONS_AGGREGATOR);
  valid_tags_.insert(NEIGHBOUR_WEIGHTER);
}

ConfigParser::ConfigParser(): configuration_(new Configuration()) {
  if (valid_tags_.empty())
    initValidTags();
}

