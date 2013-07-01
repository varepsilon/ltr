// Copyright 2012 Yandex

#ifndef LTR_CLIENT_CONFIG_PARSER_H_
#define LTR_CLIENT_CONFIG_PARSER_H_

#include <string>

#include "tinyxml/tinyxml.h"
#include "gtest/gtest.h"

#include "ltr_client/configuration.h"
#include "ltr_client/config_constants.h"
#include "ltr_client/utility/object_info.h"
#include "ltr_client/utility/train_launch_info.h"
#include "ltr_client/utility/crossvalidation_launch_info.h"
#include "ltr_client/utility/data_info.h"

#include "ltr/parameters_container/parameters_container.h"
#include "ltr/utility/macros.h"
#include "ltr/utility/safe_set.hpp"

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
  ConfigParser();
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
   * Initialize set of valid tags
   */
  void initValidTags();

  /**
   * Variable to store parsed information
   */
  Configuration::Ptr configuration_;
  /**
   * Set of tags that may be used in config
   */
  static SafeSet<string> valid_tags_;

  FRIEND_TEST(ConfigParserTest, TestingParseConfig);
  FRIEND_TEST(ConfigParserTest, TestingParseData);
  FRIEND_TEST(ConfigParserTest, TestingParseParameterized);
  FRIEND_TEST(ConfigParserTest, TestingParseLaunch);
  FRIEND_TEST(ConfigParserTest, TestingNextTiXmlElement);
};

#endif  // LTR_CLIENT_CONFIG_PARSER_H_
