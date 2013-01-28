// Copyright 2012 Yandex

#include <rlog/rlog.h>

#include <stdexcept>
#include <memory>
#include <sstream>

#include "ltr_client/configurator.h"

#include "ltr/utility/container_utility.h"

#include "ltr_client/utility/common_utility.h"
#include "ltr_client/utility/parameterized_info.h"
#include "ltr_client/utility/tag_handlers.h"

#include "tinyxml/tinyxml.h"

using std::string;
using std::stringstream;
using std::logic_error;
using std::list;
using std::cout;
using std::auto_ptr;
using std::endl;

using ltr::utility::ToString;

// ==========================  XML tokens  =====================================

// =========================== various helpers =================================

// ====================== ConfiguratorPrivate impl =============================

ConfigParser::ConfigParser() {
  root_ = NULL;
  general_xml_token_ = new OnGeneralParameterized(this);
  tag_handlers_[CONFIG] = new OnConfigParser(this);
  tag_handlers_[DATA] = new TOnDataTag(this);
  tag_handlers_[LAUNCH] = new OnLaunchTag(this);
}
ConfigParser::~ConfigParser() {
  DeleteAllFromMap(&tag_handlers_);
  delete general_xml_token_;
}

void ConfigParser::
parseConfig(const string& file_name) {
  document_ = auto_ptr<TiXmlDocument>(new TiXmlDocument(file_name));
  if (!document_->LoadFile()) {
    throw logic_error("not valid config in " + file_name + " or file not found");
  }

  root_ = document_->FirstChildElement(ROOT);
  if (!root_) {
    throw logic_error("can't find <LTR_experiment>");
  }

  TiXmlElement* config = root_->FirstChildElement(CONFIG);
  if (!config) {
    throw logic_error("can't find <config>");
  }

  TiXmlElement* root_dir = config->FirstChildElement(ROOT_DIR);
  if (!root_dir || !root_dir->GetText()) {
      throw logic_error("no root directory specified");
  }

  root_path_ = root_dir->GetText();

  rInfo(" LTR Client. Copyright 2011 Yandex");
  rInfo(" Experiment started ");

  GenericParse(tag_handlers_,
               root_->FirstChildElement(),
               general_xml_token_);

  rInfo("\n\nEnd of loadConfig. Collected data:\n");
  rInfo("data_infos\n%s\n", ToString(dataInfos()).c_str());
  rInfo("object_infos\n%s\n", ToString(objectInfos()).c_str());
  rInfo("train_infos\n%s\n", ToString(trainInfos()).c_str());
  rInfo("crossvalidation_infos\n%s\n",
    ToString(crossvalidationInfos()).c_str());

  for (ObjectInfos::iterator it = objectInfos().begin();
      it != objectInfos().end();
      ++it) {
    ObjectInfo& info = it->second;
    info.fill_dependency_list(objectInfos());
  }
}

const ConfigParser::DataInfos& ConfigParser::dataInfos() const {
  return data_infos_;
}
ConfigParser::DataInfos& ConfigParser::dataInfos() {
  return data_infos_;
}

const ConfigParser::ObjectInfos& ConfigParser::objectInfos() const {
  return object_infos_;
}

ConfigParser::ObjectInfos& ConfigParser::objectInfos() {
  return object_infos_;
}

const ConfigParser::TrainInfos& ConfigParser::trainInfos() const {
  return train_infos_;
}

ConfigParser::TrainInfos& ConfigParser::trainInfos() {
  return train_infos_;
}

const ConfigParser::CrossvalidationInfos&
                                  ConfigParser::crossvalidationInfos() const {
  return crossvalidation_infos_;
}

ConfigParser::CrossvalidationInfos& ConfigParser::crossvalidationInfos() {
  return crossvalidation_infos_;
}

const ObjectInfo& ConfigParser::findObject(
    const string& name) const {
  for (ObjectInfos::const_iterator it = object_infos_.begin();
       it != object_infos_.end();
       ++it) {
    const ObjectInfo& object_info = it->second;
    if (object_info.get_name() == name) {
      return object_info;
    }
  }
  throw logic_error("Can not find parameterized object " + name);
}

const DataInfo& ConfigParser::findData(const string& name) const {
  for (DataInfos::const_iterator it = dataInfos().begin();
       it != dataInfos().end();
       ++it) {
    const DataInfo& data_info = it->second;
    if (data_info.name == name) {
      return data_info;
    }
  }
  throw logic_error("Can not find data " + name);
}

const string& ConfigParser::rootPath() const {
  return root_path_;
}
