// Copyright 2013 Yandex

#include <memory>

#include "gtest/gtest.h"

#include "ltr_client/config_parser.h"
#include "ltr/utility/numerical.h"
#include "ltr/utility/boost/shared_ptr.h"

using ltr::utility::DoubleEqual;
using ltr::utility::shared_ptr;

TEST(ConfigParserTest, TestingParse) {
  ConfigParser parser;
  Configuration::Ptr configuration =
      parser.parse("../ltr_client/config/tests/testing_parse.cfg");

  EXPECT_EQ(configuration->root_path, "ltr1");
  EXPECT_EQ(configuration->data_infos.size(), 2);
  EXPECT_EQ(configuration->object_infos.size(), 8);
  EXPECT_EQ(configuration->train_infos.size(), 1);
  EXPECT_EQ(configuration->crossvalidation_infos.size(), 1);
}

TEST(ConfigParserTest, TestingParseConfig) {
  shared_ptr<TiXmlDocument> document(
      new TiXmlDocument("../ltr_client/config/tests/testing_parse_config.cfg"));
  document->LoadFile();
  TiXmlElement* root = document->FirstChildElement(ROOT);
  const TiXmlElement* element = root->FirstChildElement();

  ConfigParser parser;
  parser.parseConfig(*element);
  Configuration::Ptr configuration(parser.configuration_);
  EXPECT_EQ(configuration->root_path, string("Abacaba"));

  element = element->NextSibling()->ToElement();
  parser.parseConfig(*element);
  configuration = parser.configuration_;
  EXPECT_EQ(configuration->root_path, string("Abacaba"));

  element = element->NextSibling()->ToElement();
  EXPECT_ANY_THROW(parser.parseConfig(*element));
}

TEST(ConfigParserTest, TestingParseData) {
  shared_ptr<TiXmlDocument> document(
      new TiXmlDocument("../ltr_client/config/tests/testing_parse_data.cfg"));
  document->LoadFile();
  TiXmlElement* root = document->FirstChildElement(ROOT);
  const TiXmlElement* element = root->FirstChildElement();

  ConfigParser parser;
  parser.parseData(*element);
  Configuration::Ptr configuration(parser.configuration_);

  DataInfo::Ptr info = configuration->data_infos["learn"];
  EXPECT_EQ(info->name, string("learn"));
  EXPECT_EQ(info->approach, string("listwise"));
  EXPECT_EQ(info->format, string("yandex"));
  EXPECT_EQ(info->file, string("imat2009_learning.txt"));
}

TEST(ConfigParserTest, TestingParseParameterized) {
  shared_ptr<TiXmlDocument> document(
      new TiXmlDocument(
          "../ltr_client/config/tests/testing_parse_parameterized.cfg"));
  document->LoadFile();
  TiXmlElement* root = document->FirstChildElement(ROOT);
  const TiXmlElement* element = root->FirstChildElement();

  ConfigParser parser;
  parser.parseParameterized(*element);
  Configuration::Ptr configuration(parser.configuration_);
  ObjectInfo::Ptr info(configuration->object_infos["gp"]);

  EXPECT_EQ(info->tag_name, string("learner"));
  EXPECT_EQ(info->name, string("gp"));
  EXPECT_EQ(info->type, string("gplearner"));
  EXPECT_EQ(info->approach, string("listwise"));

  ParametersContainer parameters = info->parameters;
  bool bool_value = parameters.Get<bool>("bool_value");
  int int_value = parameters.Get<int>("int_value");
  double double_value = parameters.Get<double>("double_value");
  string string_value = parameters.Get<string>("string_value");
  vector<bool> vector_bool_value =
      parameters.Get<vector<bool> >("vector_bool_value");
  vector<int> vector_int_value =
      parameters.Get<vector<int> >("vector_int_value");
  vector<string> vector_string_value =
      parameters.Get<vector<string> >("vector_string_value");

  EXPECT_TRUE(bool_value);
  EXPECT_EQ(int_value, 42);
  EXPECT_TRUE(DoubleEqual(double_value, 3.14));
  EXPECT_EQ(string_value, "abacaba");

  EXPECT_TRUE(vector_bool_value[0]);
  EXPECT_TRUE(!vector_bool_value[1]);
  EXPECT_TRUE(vector_bool_value[2]);

  EXPECT_EQ(vector_int_value[0], -1);
  EXPECT_EQ(vector_int_value[1], 123456789);
  EXPECT_EQ(vector_int_value[2], 0);

  EXPECT_EQ(vector_string_value[0], "Hello");
  EXPECT_EQ(vector_string_value[1], "world!");
}

TEST(ConfigParserTest, TestingParseLaunch) {
  shared_ptr<TiXmlDocument> document(
      new TiXmlDocument("../ltr_client/config/tests/testing_parse_launch.cfg"));
  document->LoadFile();
  TiXmlElement* root = document->FirstChildElement(ROOT);
  const TiXmlElement* element = root->FirstChildElement();

  ConfigParser parser;
  parser.parseLaunch(*element);
  Configuration::Ptr configuration(parser.configuration_);
  TrainLaunchInfo::Ptr train_info = configuration->train_infos["test1"];
  CrossvalidationLaunchInfo::Ptr cv_info =
      configuration->crossvalidation_infos["k_fold1"];

  EXPECT_EQ(train_info->name, "test1");
  EXPECT_EQ(train_info->data, "learn");
  EXPECT_EQ(train_info->learner, "bf2");
  EXPECT_TRUE(train_info->gen_cpp);
  EXPECT_TRUE(train_info->predicts.contains("predict_1"));
  EXPECT_TRUE(train_info->predicts.contains("predict_2"));
  EXPECT_EQ(train_info->predicts.size(), 2);

  EXPECT_EQ(cv_info->splitter, "k_fold1");
  EXPECT_TRUE(cv_info->learners.contains("learner_1"));
  EXPECT_TRUE(cv_info->learners.contains("learner_2"));
  EXPECT_EQ(cv_info->learners.size(), 2);
  EXPECT_TRUE(cv_info->measures.contains("measure_1"));
  EXPECT_TRUE(cv_info->measures.contains("measure_2"));
  EXPECT_EQ(cv_info->measures.size(), 2);
  EXPECT_TRUE(cv_info->datas.contains("data_1"));
  EXPECT_TRUE(cv_info->datas.contains("data_2"));
  EXPECT_EQ(cv_info->datas.size(), 2);
}

TEST(ConfigParserTest, TestingNextTiXmlElement) {
  shared_ptr<TiXmlDocument> document(
      new TiXmlDocument(
          "../ltr_client/config/tests/testing_next_tixml_element.cfg"));
  document->LoadFile();
  const TiXmlElement* root = document->FirstChildElement("a");

  ConfigParser parser;
  EXPECT_EQ(parser.nextTiXmlElement(root)->Value(), string("b"));
  root = parser.nextTiXmlElement(root);
  EXPECT_TRUE(parser.nextTiXmlElement(root) == NULL);
}
