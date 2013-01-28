// Copyright 2012 Yandex

#ifndef LTR_CLIENT_CONFIGURATOR_H_
#define LTR_CLIENT_CONFIGURATOR_H_

#include <string>
#include <list>
#include <map>
#include <memory>

#include "tinyxml/tinyxml.h"

#include "ltr/parameters_container/parameters_container.h"

#include "ltr_client/utility/crossvalidation_launch_info.h"
#include "ltr_client/utility/data_info.h"
#include "ltr_client/utility/train_launch_info.h"
#include "ltr_client/utility/parameterized_info.h"

using std::string;
using std::auto_ptr;
using std::map;

class TagHandler;

typedef map<string, TagHandler*> TagHandlers;

/**
 * Performs parsing of the XML config file
 */
class ConfigParser {
 public:
  typedef map<string, DataInfo> DataInfos;
  typedef map<string, ObjectInfo> ObjectInfos;
  typedef map<string, TrainLaunchInfo> TrainInfos;
  typedef map<string, CrossvalidationLaunchInfo>
    CrossvalidationInfos;
  /**
   * Basic constructor for ConfigParser
   */
  ConfigParser();
  /**
   * Basic destructor for ConfigParser
   */
  ~ConfigParser();
  /**
   * Performs the parsing of a given file.
   * @param file_name - string containing file name.
   */
  void parseConfig(const string& file_name);

  /**
   * Constant accessor to the data_info_ field
   * @returns data_info_ constant link
   */
  const DataInfos& dataInfos() const;
  /**
   * Accessor to the data_info_ field
   * @returns data_info_ link
   */
  DataInfos& dataInfos();
  /**
   * Constant accessor to the object_infos_ field
   * @returns object_infos_ constant  link
   */
  const ObjectInfos& objectInfos() const;
  /**
   * Accessor to the object_infos_ field
   * @returns object_infos_ link
   */
  ObjectInfos& objectInfos();
  /**
   * Constant accessor to the train_infos_ field
   * @returns train_infos_ constant link
   */
  const TrainInfos& trainInfos() const;
  /**
   * Accessor to the train_infos_ field
   * @returns train_infos_ link
   */
  TrainInfos& trainInfos();
  /**
   * Constant accessor to the crossvalidation_infos_ field
   * @returns crossvalidation_infos_ constant link
   */
  const CrossvalidationInfos& crossvalidationInfos() const;
  /**
   * Accessor to the crossvalidation_infos_ field
   * @returns crossvalidation_infos_ link
   */
  CrossvalidationInfos& crossvalidationInfos();
  /**
   * Performs a search in the xml tokens for the Parameterized
   * object with given name
   * @param name - string with the name of the learner
   * @returns ObjectInfo constant link
   */
  const ObjectInfo& findObject(const string& name) const;
  /**
   * Performs a search in the xml tokens for the dataset
   * with given name
   * @param name - string with the name of the dataset
   * @returns TDataInfo constant link
   */
  const DataInfo& findData(const string& name) const;
  /**
   * Root path constant accessor
   * @returns constant link to the root_path_ variable
   */
  const string& rootPath() const;

 private:
  TagHandlers tag_handlers_;
  TagHandler* general_xml_token_;

  auto_ptr<TiXmlDocument> document_;
  TiXmlElement* root_;
  string root_path_;
  ConfigParser::DataInfos data_infos_;
  ConfigParser::ObjectInfos object_infos_;
  ConfigParser::TrainInfos train_infos_;
  ConfigParser::CrossvalidationInfos crossvalidation_infos_;
};

class ObjectInfo;

#endif  // LTR_CLIENT_CONFIGURATOR_H_
