// Copyright Yandex 2012
#ifndef LTR_CLIENT_UTILITY_TAG_HANDLERS_H_
#define LTR_CLIENT_UTILITY_TAG_HANDLERS_H_

#include <iostream>
#include <string>

using std::cout;
using std::endl;

typedef string ParameterizedDependency;

namespace { // NOLINT
static const char* const CONFIG               = "config";
static const char* const ROOT                 = "LTR_experiment";
static const char* const ROOT_DIR             = "root_directory";
static const char* const DATA                 = "data";
static const char* const LAUNCH               = "launch";
static const char* const NAME_ATTR            = "name";
static const char* const FORMAT_ATTR          = "format";
static const char* const TYPE_ATTR            = "type";
static const char* const APPROACH_ATTR        = "approach";
static const char* const PARAMETERS           = "parameters";
static const char* const WEAK_LEARNER         = "weak_learner";
static const char* const TRAIN                = "train";
static const char* const CROSSVALIDATION      = "crossvalidation";
static const char* const CPP_GEN              = "cpp";
static const char* const PREDICT              = "predict";
static const char* const FOLD_ATTR            = "fold";

static const char* const LEARNER              = "learner";
static const char* const MEASURE              = "measure";
static const char* const PREDICTIONS_AGGREGATOR =
  "predictions_aggregator";
}

/**
 * Common tag handler class. It is a parent class to
 * all of the handlers.
 */
class TagHandler {
 public:
  /**
   * Basic TagHandler constructor
   * @param configuratorInstance - the ConfigParser instance.
   */
  explicit TagHandler(ConfigParser* configuratorInstance)
    : d(configuratorInstance) {}
  /**
   * Basic TagHandler destructor
   */
  virtual ~TagHandler() {}
  /**
   * Virtual () operator. Must be overloaded to use.
   * @param xmlElement - TiXmlElement to parse.
   */
  virtual void operator()(TiXmlElement* xmlElement) = 0;
 protected:
  ConfigParser* d;
};

typedef boost::unordered_map<string, TagHandler*> TagHandlers;

/**
 * Parses the XML file representation on the first level.
 * @param parsers - the map of the functors that handles opened but
 * yet not closed tags
 * @param xmlNode - the node to be parsed
 * @param on_unknown_token - the functor that will be called if the function
 * encounters an unknown token.
 */
static inline void GenericParse(const TagHandlers& parsers,
                                TiXmlNode* xmlNode,
                                TagHandler* on_unknown_token = NULL) {
  for (; xmlNode; xmlNode = xmlNode->NextSibling()) {
    if (xmlNode->Type() != TiXmlNode::TINYXML_ELEMENT) {
      continue;
    }

    TiXmlElement* element = xmlNode->ToElement();
    if (!element) {
      throw logic_error("Cannot convert node to element");
    }

    const TagHandlers::const_iterator it = parsers.find(xmlNode->Value());
    if (it == parsers.end()) {
      if (on_unknown_token) {
        (*on_unknown_token)(element);
      } else {
        throw logic_error(string("Unknown token Value: ") +
                          xmlNode->Value() + ", Type: " +
                          boost::lexical_cast<string>(xmlNode->Type()) +
                          "\n");
      }
    } else {
      (*it->second)(element);
    }
  }
}
// =============================== Config parsing ============================

/**
 * The tag handlers that handles all the config file.
 * Inherits TagHandler
 */
class OnConfigParser: public TagHandler {
 public:
  explicit OnConfigParser(ConfigParser* impl): TagHandler(impl) { }
  virtual void operator()(TiXmlElement* element) {
    rInfo("TOnConfigExecutor");
  }  // We already read it
};

// =============================== Data parsing ==============================

/**
 * Handler that handles dataset tag (data). Inherits TagHandler.
 */
class TOnDataTag: public TagHandler {
 public:
  /**
   * Basic constructor.
   * @param impl - Config Parser object to initialize the TagHandler object.
   */
  explicit TOnDataTag(ConfigParser* impl): TagHandler(impl) { }
  virtual void operator()(TiXmlElement* element) {
    rInfo("TOnDataExecutor");

    assert(element);
    const char* name = element->Attribute(NAME_ATTR);
    const char* format = element->Attribute(FORMAT_ATTR);
    const char* approach = element->Attribute(APPROACH_ATTR);
    const char* file_name = element->GetText();

    if (!name) {
        throw logic_error("<data> with no 'name' attribute");
    }
    if (!format) {
        throw logic_error("<data> with no 'format' attribute");
    }
    if (!file_name) {
        throw logic_error("data '" +
                            string(name) + "' has no file path");
    }
    if (!approach) {
      rWarning(
        "No approach defined for data '%s'. It will be used as listwise.",
        name);
      approach = "listwise";
    }

    if (d->dataInfos().find(name) != d->dataInfos().end()) {
        throw logic_error("dublicate data name " + string(name));
    }

    d->dataInfos()[name] = DataInfo(name, approach, format, file_name);
  }
};

// =========================== General XML token parsing ======================

/**
 * Handler that parses the parameters of any parameterized tag.
 * Inherits TagHandler.
 */

class TOnParameterTag: public TagHandler {
 public:
  /**
   * Basic constructor for the TOnParameterTag object.
   * @param impl - ConfigParser pointer to initialize
   *the TagHandler object
   */
  explicit TOnParameterTag(ConfigParser* impl)
    : TagHandler(impl)
    , container(NULL) {}
  /**
   * Resets the ParametersContainer* container pointer to
   * the given one.
   * @param cont - ParametersContainer instance. Pointer to
   * the new container.
   */
  void setContainer(ltr::ParametersContainer* cont) {
    container = cont;
  }
  /**
   * Performing the parsing of the XML token. Adds the
   * parameter element to the container of parameterized object.
   * @param element - the parameter specification.
   * @returns bool value
   */
  virtual void operator()(TiXmlElement* element) {
    rInfo("TOnParametersExecutor");

    assert(container);

    const string name = element->Value();
    const string val = element->GetText();

    if (element->FirstChildElement()) {
      assert(false && "parameters in parameters is not implemented yet...");
      abort();
    }

    if (val.empty()) {
        rWarning("parameter %s has no value", name.c_str());
        return;
    }
    const string* type = element->Attribute(string(TYPE_ATTR));
    rInfo("TOnParametersExecutor: name:%s val:%s type:%d\n", name.c_str(),
         val.c_str(), type);
    addParameter(name, type ? *type : guessType(val), val);
  }

 private:
  /**
   * Converts a given string to the boolean value. If it is
   * not possible, throws an error.
   * @param value - string to convert
   */
  static bool toBool(const string& value) {
    if (boost::iequals(value, string("true"))) {
      return true;
    } else if (boost::iequals(value, string("false"))) {
      return false;
    } else {
      throw logic_error("Can not convert " + value + " to bool!");
    }
  }

  /**
   * Converts a given string to the int value.
   * If it is not possible, throws an error.
   * @param value - a string to convert
   * @returns int value
   */
  static int toInt(const string& value) {
    try {
      return boost::lexical_cast<int>(value);
    } catch(boost::bad_lexical_cast&) {
      throw logic_error("Can not convert " + value + " to int!");
    }
  }

  /**
   * Converts a given string to the double value.
   * If it is not possible throws an error.
   * @param value - string to convert
   * @returns double value.
   */
  static double toDouble(const string& value) {
    try {
      return boost::lexical_cast<double>(value);
    } catch(boost::bad_lexical_cast&) {
      throw logic_error("Can not convert " + value + " to double!");
    }
  }
  static const char* XML_TOKEN_DEPENDENCY_TYPE;

  /**
   * Adds a parameter to the container.
   * @param name - name of the parameter.
   * @param type - type of the parameter.
   * @param value - value of the parameter.
   */
  void addParameter(const string& name,
                    const string& type,
                    const string& value) {
    if (type == "bool") {
      container->AddNew(name, toBool(value));
      rInfo("TOnParametersExecutor: Added bool %s %b\n",
           name.c_str(), toBool(value));
    } else if (type == "double") {
      container->AddNew(name, toDouble(value));
      rInfo("TOnParametersExecutor: Added double %s %lf\n",
           name.c_str(), toDouble(value));
    } else if (type == "int") {
      container->AddNew(name, toInt(value));
      rInfo("TOnParametersExecutor: Added int %s %d\n",
           name.c_str(), toInt(value));
    } else if (type == XML_TOKEN_DEPENDENCY_TYPE) {
      container->AddNew(name, ParameterizedDependency(value));
      rInfo("TOnParametersExecutor: Added TXmlTokenDependency %s %s\n",
           name.c_str(), value.c_str());
    } else {
      assert(false && ("Adding " + type +
                          " is not implemented yet...").c_str());
    }
  }

  /**
   * Tries to get the parameter's type by the parameter.
   * @param value - string with the value with unknown type.
   * @returns string with type specification.
   * Yet is under construction.
   */
  static string guessType(const string& value) {
    const string::size_type pos_of_space = value.find(' ');
    rInfo("TOnParametersExecutor: guessing type of %s %d\n",
         value.c_str(), pos_of_space);

    if (pos_of_space != string::npos) {  // some kind of list
      const string first_element = value.substr(0, pos_of_space);
      // TODO(dimanne) check type of other elements and promote it if needed
      return "list of " + guessTypeOfOneElement(first_element);
    }
    return guessTypeOfOneElement(value);
  }

  /**
   * Tries to get the type of the value by only value format.
   * Guesses int, bool and double types.
   * @param value - a value to guess the type.
   * @returns string with a type specification.
   */
  static string guessTypeOfOneElement(const string& value) {
    if (boost::iequals(value, string("true")) ||
       boost::iequals(value, string("false"))) {
      return "bool";
    }

    const string::size_type pos_of_decimal_point = value.find('.');
    if (pos_of_decimal_point != string::npos) {
      try {
        boost::lexical_cast<double>(value);
      } catch(boost::bad_lexical_cast&) {
        return XML_TOKEN_DEPENDENCY_TYPE;
      }
      return "double";
    }

    try {
      boost::lexical_cast<int>(value);
    } catch(boost::bad_lexical_cast&) {
      return XML_TOKEN_DEPENDENCY_TYPE;
    }
    return "int";
  }
  ltr::ParametersContainer* container;
};

const char* TOnParameterTag::XML_TOKEN_DEPENDENCY_TYPE =
    "TXmlTokenDependency";

/**
 * Handles the general xml token.
 * Inherits TagHandler.
 */

class OnGeneralParameterized: public TagHandler {
 public:
  /**
   * Basic OnGeneralXmlToken constructor.
   * @param impl - ConfigParser pointer to initialize TagHandler.
   */
  explicit OnGeneralParameterized(ConfigParser* impl): TagHandler(impl) {
    parameters_executor = new TOnParameterTag(impl);
  }
  /**
   * Basic OnGeneralXmlToken destructor.
   */
  ~OnGeneralParameterized() {
    delete parameters_executor;
  }
  /**
   * () operator. If all the parameters of the tag are correct,
   * it performs parsing. Else throws logic error.
   * @param element - TiXmlElement pointer to parse.
   */
  virtual void operator()(TiXmlElement* element) {
    rInfo("TOnGeneralXmlToken\n");

    const char* name = element->Attribute(NAME_ATTR);
    ParametrizedInfo& spec = ltr::utility::SafeInsert(d->xmlTokenSpecs(),
                                                      name);
    const char* type = element->Attribute(TYPE_ATTR);
    if (!type) {
      throw logic_error("no 'type' attribute");
    }

    const char* approach = element->Attribute(APPROACH_ATTR);
    if (!approach) {
      rWarning("No approach defined '%s'. Try to define automatically.", name);
      approach = "";
    }
    const char* tag_name = element->Value();
    if (!tag_name) {
      throw logic_error("no tag name");
    }

    spec.tag_name_ = tag_name;
    spec.object_name_ = name;
    spec.object_type_ = type;
    spec.approach_ = approach;

    parameters_executor->setContainer(&spec.parameters_);
    GenericParse(TagHandlers(),
                 element->FirstChildElement(),
                 parameters_executor);
  }

 private:
  TOnParameterTag* parameters_executor;
};

//============================= Launch parsing =================================

/**
 * Handles crossvalidation tag.
 * Inherits TagHandler.
 */

class OnCVLearnerTag: public TagHandler {
  public:
  /**
   * Basic OnCVLearnerTag constructor.
   * @param impl - a ConfigParser pointer to initialize TagHandler.
   */
  explicit OnCVLearnerTag(ConfigParser* impl): TagHandler(impl) {
    info = NULL;
  }
  /**
   * Basic OnCVLearnerTag destructor.
   */
  ~OnCVLearnerTag() { }
  void setInfo(CrossvalidationLaunchInfo* ti) {
    info = ti;
  }

  /**
   * () operator. Inserts the leaner into crossvalidator.
   * @param element - TiXmlElement pointer. Must contain
   * leaner.
   */
  virtual void operator()(TiXmlElement* element) {
    rInfo("TOnCVLearnerExecutor\n");
    assert(info);
    ltr::utility::SafeInsert(info->learners, element->GetText());
  }

 private:
  CrossvalidationLaunchInfo* info;
};

/**
 * Handles measure inside the crossvalidator XML instance.
 * Inherits TagHandler.
 */
class OnCVMeasureTag: public TagHandler {
 public:
  /**
   * Basic OnCVMeasureTag constructor.
   * @param impl - ConfigParser pointer to initialize
   * the TagHandler object.
   */
  explicit OnCVMeasureTag(ConfigParser* impl): TagHandler(impl) {
    info = NULL;
  }
  /**
   * Basic OnCVMeasureTag destructor.
   */
  ~OnCVMeasureTag() { }
  void setInfo(CrossvalidationLaunchInfo* ti) {
    info = ti;
  }

  /**
   * () operator inserts measure into the crossvalidator.
   * @param element - TiXmlElement pointer to the XML tag.
   * Must contain the measure.
   */
  virtual void operator()(TiXmlElement* element) {
    rInfo("TOnCVMeasureExecutor");
    assert(info);
    ltr::utility::SafeInsert(info->measures, element->GetText());
  }

 private:
  CrossvalidationLaunchInfo* info;
};

/**
 * Handles dataset tag (data) inside the crossvalidator XML instance.
 * Inherits TagHandler.
 */
class OnCVDataTag: public TagHandler {
 public:
  /**
   * Basic OnCVDataTag constructor.
   * @param impl - ConfigParser pointer to initialize
   * the TagHandler object.
   */
  explicit OnCVDataTag(ConfigParser* impl)
    : TagHandler(impl)
    , info(NULL) {}
  /**
   * Basic OnCVDataTag destructor.
   */
  ~OnCVDataTag() {}
  void setInfo(CrossvalidationLaunchInfo* trainInfo) {
    info = trainInfo;
  }

  /**
   * () operator inserts dataset into the crossvalidator.
   * @param element - TiXmlElement pointer to the XML tag.
   * Must contain the dataset.
   */
  virtual void operator() (TiXmlElement* element) {
    rInfo("TOnCVDataExecutor\n");
    assert(info);
    ltr::utility::SafeInsert(info->datas, element->GetText());
  }

 private:
  CrossvalidationLaunchInfo* info;
};

/**
 * Handles crossvalidation tag.
 * Inherits TagHandler.
 */

class OnCrossvalidationTag: public TagHandler {
 public:
  /**
   * Basic OnCVCrossvalidationTag constructor.
   * @param impl - ConfigParser pointer to initialize
   * the TagHandler object.
   */
  explicit OnCrossvalidationTag(ConfigParser* impl):
    TagHandler(impl) {
    handlers_[LEARNER] = learner_executor = new OnCVLearnerTag(impl);
    handlers_[MEASURE] = measure_executor = new OnCVMeasureTag(impl);
    handlers_[DATA] = data_executor = new OnCVDataTag(impl);
  }
  /**
   * Basic OnCVDataTag destructor.
   */
  ~OnCrossvalidationTag() {
    DeleteAllFromUnorderedMap(&handlers_);
  }
  /**
   * () operator inserts dataset into the crossvalidator.
   * @param element - TiXmlElement pointer to the XML tag.
   * Must contain the dataset.
   */
  virtual void operator()(TiXmlElement* element) {
    rInfo("TOnCrossvalidationExecutor\n");

    const char* fold = element->Attribute(FOLD_ATTR);
    if (!fold) {
        rError("Failed: <crossvalidation> with no fold");
        return;
    }

    CrossvalidationLaunchInfo new_fold_info(fold);
    d->crossvalidationInfos()[fold] = new_fold_info;
    learner_executor->setInfo(&d->crossvalidationInfos()[fold]);
    measure_executor->setInfo(&d->crossvalidationInfos()[fold]);
    data_executor->setInfo(&d->crossvalidationInfos()[fold]);
    GenericParse(handlers_, element->FirstChildElement());
  }

 private:
  OnCVLearnerTag* learner_executor;
  OnCVMeasureTag* measure_executor;
  OnCVDataTag* data_executor;
  TagHandlers handlers_;
};

/**
 * Handles predict tag.
 * Inherits TagHandler.
 */
class OnPredictTag: public TagHandler {
  public:
  /**
   * Basic OnPredictTag constructor.
   * @param impl - ConfigParser pointer to initialize
   * the TagHandler object.
   */
  explicit OnPredictTag(ConfigParser* impl)
    : TagHandler(impl)
    , info(NULL) {}
  /**
   * Basic OnPredictTag destructor.
   */
  ~OnPredictTag() {}
  void setTrainInfo(TrainLaunchInfo* trainInfo) {
    info = trainInfo;
  }
  /**
   * () operator. Adds element text to the TrainLaunchInfo.
   * @param element - TiXmlElement pointer to the XML tag.
   * Must contain predict tag.
   */
  virtual void operator() (TiXmlElement* element) {
    rInfo("TOnPredictExecutor\n");
    assert(info);
    ltr::utility::SafeInsert(info->predicts, element->GetText());
  }

 private:
  TrainLaunchInfo* info;
};

/**
 * Handles cpp tag.
 * Inherits TagHandler.
 */
class OnCppGenTag: public TagHandler {
  public:
  /**
   * Basic OnCppGenTag constructor.
   * @param impl - ConfigParser pointer to initialize
   * the TagHandler object.
   */
  explicit OnCppGenTag(ConfigParser* impl)
    : TagHandler(impl)
    , info(NULL) {}
  /**
   * Basic OnCppGenTag destructor.
   */
  ~OnCppGenTag() {}
  void setTrainInfo(TrainLaunchInfo* trainInfo) {
    info = trainInfo;
  }
  /**
   * () operator. Sets gen_cpp flag to the true value.
   * @param element - TiXmlElement pointer to the XML tag.
   * Must contain the сpp tag.
   */
  virtual void operator() (TiXmlElement* element) {
    assert(info);
    info->gen_cpp = true;
  }

 private:
  TrainLaunchInfo* info;
};

/**
 * Handles train tag.
 * Inherits TagHandler.
 */
class OnTrainTag: public TagHandler {
 public:
  /**
   * Basic OnTrainTag constructor.
   * @param impl - ConfigParser pointer to initialize
   * the TagHandler object.
   */
  explicit OnTrainTag(ConfigParser* impl): TagHandler(impl) {
    cpp_gen_executor =  new OnCppGenTag(impl);
    predict_executor = new OnPredictTag(impl);
    handlers_[CPP_GEN] = cpp_gen_executor;
    handlers_[PREDICT] = predict_executor;
  }
  /**
   * Basic OnTrainTag destructor
   */
  ~OnTrainTag() {
    DeleteAllFromUnorderedMap(&handlers_);
  }

  /**
   * () operator. Sets the train info according to the tag.
   * @param element - TiXmlElement pointer to the XML tag.
   * Must contain the train tag.
   */
  virtual void operator() (TiXmlElement* element) {
    rInfo("TOnTrainExecutor\n");
    const char* name = element->Attribute("name");
    const char* data = element->Attribute("data");
    const char* learner = element->Attribute("learner");

    if (!name) {
        rError("Failed: <train> without name attribute");
        return;
    }
    if (!data) {
        rError("Failed: <train> without data attribute");
        return;
    }
    if (!learner) {
        rError("Failed: <train> without learner attribute");
        return;
    }
    if (d->trainInfos().find(name) != d->trainInfos().end()) {
        rError("Failed: dublicate train name ");
        return;
    }

    TrainLaunchInfo new_train_info(name, data, learner);
    d->trainInfos()[name] = new_train_info;

    cpp_gen_executor->setTrainInfo(&d->trainInfos()[name]);
    predict_executor->setTrainInfo(&d->trainInfos()[name]);
    GenericParse(handlers_, element->FirstChildElement());
  }

 private:
  TagHandlers handlers_;
  OnCppGenTag* cpp_gen_executor;
  OnPredictTag* predict_executor;
};

/**
 * Handles launch tag.
 * Inherits TagHandler.
 */
class OnLaunchTag: public TagHandler {
 public:
  /**
   * Basic OnLaunchTag constructor.
   * @param impl - ConfigParser pointer to initialize
   * the TagHandler object.
   */
  explicit OnLaunchTag(ConfigParser* impl): TagHandler(impl) {
    handlers_[TRAIN] = new OnTrainTag(impl);
    handlers_[CROSSVALIDATION] = new OnCrossvalidationTag(impl);
  }
  /**
   * Basic OnTrainTag destructor
   */
  ~OnLaunchTag() { DeleteAllFromUnorderedMap(&handlers_); }

  /**
   * () operator. Performs GenericParse from the first element.
   * @param element - TiXmlElement pointer to the XML tag.
   */
  virtual void operator() (TiXmlElement* element) {
    rInfo("TOnLaunchExecutor\n");
    GenericParse(handlers_, element->FirstChildElement());
  }

 private:
  TagHandlers handlers_;
};

#endif  // LTR_CLIENT_UTILITY_TAG_HANDLERS_H_
