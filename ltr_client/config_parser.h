// Copyright 2012 Yandex

#ifndef LTR_CLIENT_CONFIG_PARSER_H_
#define LTR_CLIENT_CONFIG_PARSER_H_

#include <string>

#include "tinyxml/tinyxml.h"
#include "gtest/gtest.h"

#include "ltr_client/configuration.h"
#include "ltr_client/utility/object_info.h"
#include "ltr_client/utility/train_launch_info.h"
#include "ltr_client/utility/crossvalidation_launch_info.h"
#include "ltr_client/utility/data_info.h"

#include "ltr/parameters_container/parameters_container.h"
#include "ltr/utility/macros.h"

using std::string;

using ltr::ParametersContainer;

/**
 * Performs parsing of the XML config file
 */
class ConfigParser {
 public:
  /**
   * Basic constructor for ConfigParser
   */
  ConfigParser(): configuration_(new Configuration()) { }
  /**
   * Basic destructor for ConfigParser
   */
  virtual ~ConfigParser() { }
  /**
   * Performs the parsing of a given file.
   *
   * \param file_name - string containing file name.
   */
  Configuration::Ptr parse(const string& file_name);
 private:
  /**
   * Parses tags from given file
   *
   * \param element - TiXmlElement to parse.
   */
  void parseTags(const TiXmlElement& element);
  /**
   * Parses configuration info. Should be called for \<config\> tag.
   *
   * \param element - TiXmlElement to parse.
   */
  void parseConfig(const TiXmlElement& element);
  /**
   * Parses info about datasets. Should be called for \<data\> tag.
   *
   * \param element - TiXmlElement to parse.
   */
  void parseData(const TiXmlElement& element);
  /**
   * Parses info about any parameterized (e.g. learner) object.
   *
   * \param element - TiXmlElement to parse.
   */
  void parseParameterized(const TiXmlElement& element);
  /**
   * Parses launch info. Should be called for \<launch\> tag.
   *
   * \param element - TiXmlElement to parse.
   */
  void parseLaunch(const TiXmlElement& element);
  /**
   * Parses parameters of parameterized object.
   *
   * \param element - TiXmlElement to parse.
   * \param parameters - ParametersContainer for storing parsed info.
   */
  void parseParameters(const TiXmlElement& element,
                       ParametersContainer* parameters);
  /**
   * Parses train section of launch info. Should be called for \<train\> tag.
   *
   * \param element - TiXmlElement to parse.
   * \param info - TrainLaunchInfo for storing parsed info.
   */
  void parseTrain(const TiXmlElement& element, TrainLaunchInfo* info);
  /**
   * Parses crossvalidation section of launch info.
   * Should be called for \<crossvalidation\> tag.
   *
   * \param element - TiXmlElement to parse.
   * \param info - CrossvalidationLaunchInfo for storing parsed info.
   */
  void parseCrossvalidation(const TiXmlElement& element,
                            CrossvalidationLaunchInfo* info);
  /**
   * Returns next element of the list of nodes of same level.
   * For example, you can access to \<b\> and \<c\> tags having pointer
   * to \<a\>, where reading file is such as
   * \code
   *  <root>
   *    <a><\a>
   *    <b><\b>
   *    <c><\c>
   * </root>
   * \endcode
   *
   * \param node - constant pointer to node
   * \return Next element or NULL if given element is the last
   */
  const TiXmlElement* nextTiXmlElement(const TiXmlNode* node);

  /**
   * Variable to store parsed information
   */
  Configuration::Ptr configuration_;

  FRIEND_TEST(ConfigParserTest, TestingParseConfig);
  FRIEND_TEST(ConfigParserTest, TestingParseData);
  FRIEND_TEST(ConfigParserTest, TestingParseParameterized);
  FRIEND_TEST(ConfigParserTest, TestingParseLaunch);
  FRIEND_TEST(ConfigParserTest, TestingNextTiXmlElement);

  /**
   * Name of the tag that containes name of the root directory.
   * Value of the constant see in \ref config_parser_constants.cc
   */
  static const char* const ROOT_DIR;
  /**
   * Name of listwise approach.
   * Value of the constant see in \ref config_parser_constants.cc
   */
  static const char* const LISTWISE;
  /**
   * Name of the tag that containes configuration information.
   * Value of the constant see in \ref config_parser_constants.cc
   */
  static const char* const CONFIG;
  /**
   * Name of the root tag.
   * Value of the constant see in \ref config_parser_constants.cc
   */
  static const char* const ROOT;
  /**
   * Name of the tag that containes information about dataset.
   * Value of the constant see in \ref config_parser_constants.cc
   */
  static const char* const DATA;
  /**
   * Name of the tag that containes launch information.
   * Value of the constant see in \ref config_parser_constants.cc
   */
  static const char* const LAUNCH;
  /**
   * Attribute "name"
   * Value of the constant see in \ref config_parser_constants.cc
   */
  static const char* const NAME_ATTR;
  /**
   * Attribute "format"
   * Value of the constant see in \ref config_parser_constants.cc
   */
  static const char* const FORMAT_ATTR;
  /**
   * Attribute "type"
   * Value of the constant see in \ref config_parser_constants.cc
   */
  static const char* const TYPE_ATTR;
  /**
   * Attribute "approach"
   * Value of the constant see in \ref config_parser_constants.cc
   */
  static const char* const APPROACH_ATTR;
  /**
   * Name of the tag that containes train information.
   * Value of the constant see in \ref config_parser_constants.cc
   */
  static const char* const TRAIN;
  /**
   * Name of the tag that containes crossvalidation information.
   * Value of the constant see in \ref config_parser_constants.cc
   */
  static const char* const CROSSVALIDATION;
  /**
   * Name of the C++ code generation flag.
   * Value of the constant see in \ref config_parser_constants.cc
   */
  static const char* const CPP_GEN;
  /**
   * Name of prediction tag.
   * Value of the constant see in \ref config_parser_constants.cc
   */
  static const char* const PREDICT;
  /**
   * Attribute "fold".
   * Value of the constant see in \ref config_parser_constants.cc
   */
  static const char* const FOLD_ATTR;
  /**
   * Name of tag that containes information about learners.
   * Value of the constant see in \ref config_parser_constants.cc
   */
  static const char* const LEARNER;
  /**
   * Name of tag that containes information about measure.
   * Value of the constant see in \ref config_parser_constants.cc
   */
  static const char* const MEASURE;
};

#endif  // LTR_CLIENT_CONFIG_PARSER_H_
