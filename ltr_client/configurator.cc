// Copyright 2012 Yandex

#include "ltr_client/configurator.h"

#include <stdexcept>
#include <memory>
#include <sstream>

#include "boost/lexical_cast.hpp"
#include "boost/algorithm/string/predicate.hpp"

#include "logog/logog.h"

#include "ltr/utility/container_utility.h"

#include "tinyxml/tinyxml.h"

using std::string;
using std::stringstream;
using std::logic_error;
using std::list;
using std::cout;
using std::auto_ptr;
using std::endl;

// ==========================  XML tokens  =====================================
namespace {
static const char* const CONFIG          = "config";
static const char* const ROOT            = "LTR_experiment";
static const char* const ROOT_DIR        = "root_directory";
static const char* const DATA            = "data";
static const char* const LAUNCH          = "launch";
static const char* const NAME_ATTR       = "name";
static const char* const FORMAT_ATTR     = "format";
static const char* const TYPE_ATTR       = "type";
static const char* const APPROACH_ATTR   = "approach";
static const char* const PARAMETERS      = "parameters";
static const char* const WEAK_LEARNER    = "weak_learner";
static const char* const TRAIN           = "train";
static const char* const CROSSVALIDATION = "crossvalidation";
static const char* const CPP_GEN         = "cpp";
static const char* const PREDICT         = "predict";
static const char* const FOLD_ATTR       = "fold";

static const char* const LEARNER         = "learner";
static const char* const MEASURE         = "measure";
}

// =========================== various helpers =================================

template <class Key, class Value>
static inline void DeleteAllFromUnorderedMap(
    boost::unordered_map<Key, Value>* unorderedMapInstance
    ) {
  assert(unorderedMapInstance);
  // C++11 We have no unique_ptr so we MUST delete pointers manually
  for (typename boost::unordered_map<Key, Value>::const_iterator it =
       unorderedMapInstance->begin();
       it != unorderedMapInstance->end();
       ++it) {
    delete it->second;
  }
}

class TagHandler {
 public:
  explicit TagHandler(ConfigParser* configuratorInstance)
    : d(configuratorInstance) {}
  virtual ~TagHandler() {}
  virtual void operator()(TiXmlElement* xmlElement) = 0;
 protected:
  ConfigParser* d;
};

typedef boost::unordered_map<string, TagHandler*> TagHandlers;

static inline void GenericParse(const TagHandlers& executors,
                                TiXmlNode* xmlNode,
                                TagHandler* on_unknown_token = NULL) {
  for (; xmlNode; xmlNode = xmlNode->NextSibling()) {
    if (xmlNode->Type() != TiXmlNode::TINYXML_ELEMENT) {
      continue;
    }

    TiXmlElement* element = xmlNode->ToElement();
    if (!element) {
      throw logic_error("Can not convert node to element");
    }

    const TagHandlers::const_iterator it = executors.find(xmlNode->Value());
    if (it == executors.end()) {
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

template <class Key, class Value>
inline string ToString(const boost::unordered_map<Key, Value>& cont) {
  typedef typename boost::unordered_map<Key, Value>::const_iterator TIter;
  stringstream out(stringstream::out);
  size_t i = 0;
  for (TIter it = cont.begin(); it != cont.end(); ++it)
    out << ++i << ")" << it->first << " " << ToString(it->second) << '\n';
  return out.str();
}

template <class Value>
inline string ToString(const boost::unordered_set<Value>& cont) {
  typedef typename boost::unordered_set<Value>::const_iterator TIter;
  stringstream out(stringstream::out);
  out << "set(";
  for (TIter it = cont.begin(); it != cont.end(); ++it)
    out << *it << "; ";
  out << ")";
  return out.str();
}

string ToString(const TDataInfo& info) {;
  stringstream out(stringstream::out);
  out << "TDataInfo(name=" << info.name
      << ", approach=" << info.approach
      << ", format=" << info.format
      << ", file_name=" << info.file
      << ")";
  return out.str();
}

string ToString(const TrainLaunchInfo& info) {
  stringstream out(stringstream::out);
  out << "TTrainInfo(name=" << info.name
      << ", data=" << info.data
      << ", leatner=" << info.learner
      << ", predicts=" << ToString(info.predicts)
      << ", gen_cpp=" << info.gen_cpp
      << ")";
  return out.str();
}

string ToString(const CrossvalidationLaunchInfo& info) {
  stringstream out(stringstream::out);
  out << "TCrossvalidationInfo(fold=" << info.splitter
      << ", learners=" << ToString(info.learners)
      << ", measures=" << ToString(info.measures)
      << ", datas=" << ToString(info.datas)
      << ")";
  return out.str();
}

// =============================== TXmlTokenSpec ============================

void ParametrizedInfo::fillDependencyList(
    const ConfigParser::TXmlTokenSpecs& token_specs) {
  dependency_specs_.clear();

  typedef ltr::ParametersContainer::NameValue<const TXmlTokenDependency>
      TNameValue;
  typedef list<TNameValue> TDependencies;
  const TDependencies& my_dependencies =
      parameters_.getValuesByType<const TXmlTokenDependency>();

  for (TDependencies::const_iterator my_dependency_it = my_dependencies.begin();
      my_dependency_it != my_dependencies.end();
      ++my_dependency_it) {
    const TNameValue& dependency = *my_dependency_it;
    const ParametrizedInfo* found = NULL;

    ConfigParser::TXmlTokenSpecs::const_iterator it = token_specs.find(
          dependency.value.parameter_name);
    found = &it->second;

    if (!found) {
      throw logic_error("TXmlTokenSpec::fillDependency List: "
                        "Could not resolve dependency " +
                        dependency.value.parameter_name);
    }
    dependency_specs_.push_back(found);
  }
}

ParametrizedInfo::ParametrizedInfo()
  : tag_name_()
  , name_()
  , type_()
  , approach_()
  , parameters_()
  , dependency_specs_() {}

ParametrizedInfo::ParametrizedInfo(const string& tag_name,
                 const string& name,
                 const string& type,
                 const string& approach,
                 ltr::ParametersContainer parameters)
  : tag_name_(tag_name)
  , name_(name)
  , type_(type)
  , approach_(approach)
  , parameters_(parameters) {}

ParametrizedInfo::ParametrizedInfo(const ParametrizedInfo& other) {
  *this = other;
}

ParametrizedInfo& ParametrizedInfo::operator= (const ParametrizedInfo& other) {
  if (this == &other) {
    return *this;
  }
  name_ = other.getName();
  type_ = other.getType();
  approach_ = other.getApproach();
  parameters_ = other.getParameters();
  dependency_specs_ = other.dependencySpecs();
  return *this;
}


ParametrizedInfo::~ParametrizedInfo() {
}

const string& ParametrizedInfo::getTagName() const {
  return tag_name_;
}
const string& ParametrizedInfo::getName() const {
  return name_;
}
const string& ParametrizedInfo::getType() const {
  return type_;
}
const string& ParametrizedInfo::getApproach() const {
  return approach_;
}
const ltr::ParametersContainer& ParametrizedInfo::getParameters() const {
  return parameters_;
}
const TXmlTokenSpecList& ParametrizedInfo::dependencySpecs() const {
  return dependency_specs_;
}

string ToString(const ParametrizedInfo& info) {
  stringstream out(stringstream::out);
  out << "TXmlTokenSpec(name=" << info.getName()
      << ", type=" << info.getType()
      << ", approach=" << info.getApproach()
      << ", parameters=" << info.getParameters().toString()
         << ", my dependencies=(";
  for (TXmlTokenSpecList::const_iterator it = info.dependencySpecs().begin();
      it != info.dependencySpecs().end();
      ++it) {
    out << (*it)->getName() << ", ";
  }
  out << "))";
  return out.str();
}

// =============================== Config parsing ============================

class OnConfigParser: public TagHandler {
 public:
  explicit OnConfigParser(ConfigParser* impl): TagHandler(impl) { }
  virtual void operator()(TiXmlElement* element) {
    cout << "TOnConfigExecutor" << endl;
  }  // We already read it
};

// =============================== Data parsing ==============================

class TOnDataTag: public TagHandler {
 public:
  explicit TOnDataTag(ConfigParser* impl): TagHandler(impl) { }
  virtual void operator()(TiXmlElement* element) {
    cout << "TOnDataExecutor" << endl;

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
        WARN("No approach defined for data '%s'. It will be used as listwise.",
             name);
        approach = "listwise";
    }

    if (d->dataInfos().find(name) != d->dataInfos().end()) {
        throw logic_error("dublicate data name " + string(name));
    }

    d->dataInfos()[name] = TDataInfo(name, approach, format, file_name);
  }
};

// =========================== General XML token parsing ======================

class TOnParameterTag: public TagHandler {
 public:
  explicit TOnParameterTag(ConfigParser* impl)
    : TagHandler(impl)
    , container(NULL) {}

  void setContainer(ltr::ParametersContainer* cont) {
    container = cont;
  }

  virtual void operator()(TiXmlElement* element) {
    cout << "TOnParametersExecutor" << endl;

    assert(container);

    const string name = element->Value();
    const string val = element->GetText();

    if (element->FirstChildElement()) {
      assert(false && "parameters in parameters is not implemented yet...");
      abort();
    }

    if (val.empty()) {
        WARN("parameter %s has no value", name.c_str());
        return;
    }
    const string* type = element->Attribute(string(TYPE_ATTR));
    cout << "TOnParametersExecutor: name:" << name <<
            " val: " << val << " type: " << type << endl;
    addParameter(name, type ? *type : guessType(val), val);
  }

 private:
  static bool toBool(const string& value) {
    if (boost::iequals(value, string("true"))) {
      return true;
    } else if (boost::iequals(value, string("false"))) {
      return false;
    } else {
      throw logic_error("Can not convert " + value + " to bool!");
    }
  }
  static int toInt(const string& value) {
    try {
      return boost::lexical_cast<int>(value);
    } catch(boost::bad_lexical_cast&) {
      throw logic_error("Can not convert " + value + " to int!");
    }
  }
  static double toDouble(const string& value) {
    try {
      return boost::lexical_cast<double>(value);
    } catch(boost::bad_lexical_cast&) {
      throw logic_error("Can not convert " + value + " to double!");
    }
  }
  static const char* XML_TOKEN_DEPENDENCY_TYPE;

  void addParameter(const string& name,
                    const string& type,
                    const string& value) {
    if (type == "bool") {
      container->AddNew(name, toBool(value));
      cout << "TOnParametersExecutor: Added bool " <<
                   name << " " << toBool(value) << endl;
    } else if (type == "double") {
      container->AddNew(name, toDouble(value));
      cout << "TOnParametersExecutor: Added double " <<
                   name << " " << toDouble(value) << endl;
    } else if (type == "int") {
      container->AddNew(name, toInt(value));
      cout << "TOnParametersExecutor: Added int " <<
                   name << " " << toInt(value) << endl;
    } else if (type == XML_TOKEN_DEPENDENCY_TYPE) {
      container->AddNew(name, TXmlTokenDependency(value));
      cout << "TOnParametersExecutor: Added TXmlTokenDependency " <<
                   name << " " << value << endl;
    } else {
      assert(false && ("Adding " + type +
                          " is not implemented yet...").c_str());
    }
  }
  static string guessType(const string& value) {
    const string::size_type pos_of_space = value.find(' ');
    cout << "TOnParametersExecutor: guessing type of " <<
                 value << " " << pos_of_space << " " << endl;

    if (pos_of_space != string::npos) {  // some kind of list
      const string first_element = value.substr(0, pos_of_space);
      // TODO(dimanne) check type of other elements and promote it if needed
      return "list of " + guessTypeOfOneElement(first_element);
    }
    return guessTypeOfOneElement(value);
  }
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


class OnGeneralXmlToken: public TagHandler {
 public:
  explicit OnGeneralXmlToken(ConfigParser* impl): TagHandler(impl) {
    parameters_executor = new TOnParameterTag(impl);
  }
  ~OnGeneralXmlToken() {
    delete parameters_executor;
  }

  virtual void operator()(TiXmlElement* element) {
    cout << "TOnGeneralXmlToken" << endl;

    const char* name = element->Attribute(NAME_ATTR);
    ParametrizedInfo& spec = ltr::utility::SafeInsert(d->xmlTokenSpecs(),
                                                      name);
    const char* type = element->Attribute(TYPE_ATTR);
    if (!type) {
      throw logic_error("no 'type' attribute");
    }

    const char* approach = element->Attribute(APPROACH_ATTR);
    if (!approach) {
      WARN("No approach defined '%s'. Try to define automatically.", name);
      approach = "";
    }
    const char* tag_name = element->Value();
    if (!tag_name) {
      throw logic_error("no tag name");
    }

    spec.tag_name_ = tag_name;
    spec.name_ = name;
    spec.type_ = type;
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

class OnCVLearnerTag: public TagHandler {
  public:
  explicit OnCVLearnerTag(ConfigParser* impl): TagHandler(impl) {
    info = NULL;
  }
  ~OnCVLearnerTag() { }
  void setInfo(CrossvalidationLaunchInfo* ti) {
    info = ti;
  }
  virtual void operator()(TiXmlElement* element) {
    cout << "TOnCVLearnerExecutor" << endl;
    assert(info);
    ltr::utility::SafeInsert(info->learners, element->GetText());
  }

 private:
  CrossvalidationLaunchInfo* info;
};

class OnCVMeasureTag: public TagHandler {
  public:
  explicit OnCVMeasureTag(ConfigParser* impl): TagHandler(impl) {
    info = NULL;
  }
  ~OnCVMeasureTag() { }
  void setInfo(CrossvalidationLaunchInfo* ti) {
    info = ti;
  }
  virtual void operator()(TiXmlElement* element) {
    cout << "TOnCVMeasureExecutor" << endl;
    assert(info);
    ltr::utility::SafeInsert(info->measures, element->GetText());
  }

 private:
  CrossvalidationLaunchInfo* info;
};

class OnCVDataTag: public TagHandler {
  public:
  explicit OnCVDataTag(ConfigParser* impl)
    : TagHandler(impl)
    , info(NULL) {}
  ~OnCVDataTag() {}
  void setInfo(CrossvalidationLaunchInfo* trainInfo) {
    info = trainInfo;
  }
  virtual void operator() (TiXmlElement* element) {
    cout << "TOnCVDataExecutor" << endl;
    assert(info);
    ltr::utility::SafeInsert(info->datas, element->GetText());
  }

 private:
  CrossvalidationLaunchInfo* info;
};



class OnCrossvalidationTag: public TagHandler {
 public:
  explicit OnCrossvalidationTag(ConfigParser* impl):
    TagHandler(impl) {
    handlers_[LEARNER] = learner_executor = new OnCVLearnerTag(impl);
    handlers_[MEASURE] = measure_executor = new OnCVMeasureTag(impl);
    handlers_[DATA] = data_executor = new OnCVDataTag(impl);
  }
  ~OnCrossvalidationTag() {
    DeleteAllFromUnorderedMap(&handlers_);
  }
  virtual void operator()(TiXmlElement* element) {
    cout << "TOnCrossvalidationExecutor" << endl;

    const char* fold = element->Attribute(FOLD_ATTR);
    if (!fold) {
        ERR("Failed: <crossvalidation> with no fold");
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

class OnPredictTag: public TagHandler {
  public:
  explicit OnPredictTag(ConfigParser* impl)
    : TagHandler(impl)
    , info(NULL) {}

  ~OnPredictTag() {}
  void setTrainInfo(TrainLaunchInfo* trainInfo) {
    info = trainInfo;
  }

  virtual void operator() (TiXmlElement* element) {
    cout << "TOnPredictExecutor" << endl;
    assert(info);
    ltr::utility::SafeInsert(info->predicts, element->GetText());
  }

 private:
  TrainLaunchInfo* info;
};

class OnCppGenTag: public TagHandler {
  public:
  explicit OnCppGenTag(ConfigParser* impl)
    : TagHandler(impl)
    , info(NULL) {}

  ~OnCppGenTag() {}
  void setTrainInfo(TrainLaunchInfo* trainInfo) {
    info = trainInfo;
  }

  virtual void operator() (TiXmlElement* element) {
    assert(info);
    info->gen_cpp = true;
  }

 private:
  TrainLaunchInfo* info;
};

class OnTrainTag: public TagHandler {
 public:
  explicit OnTrainTag(ConfigParser* impl): TagHandler(impl) {
    cpp_gen_executor =  new OnCppGenTag(impl);
    predict_executor = new OnPredictTag(impl);
    handlers_[CPP_GEN] = cpp_gen_executor;
    handlers_[PREDICT] = predict_executor;
  }
  ~OnTrainTag() { DeleteAllFromUnorderedMap(&handlers_); }
  virtual void operator() (TiXmlElement* element) {
    cout << "TOnTrainExecutor" << endl;
    const char* name = element->Attribute("name");
    const char* data = element->Attribute("data");
    const char* learner = element->Attribute("learner");
    if (!name) {
        ERR("Failed: <train> without name attribute");
        return;
    }
    if (!data) {
        ERR("Failed: <train> without data attribute");
        return;
    }
    if (!learner) {
        ERR("Failed: <train> without learner attribute");
        return;
    }
    if (d->trainInfos().find(name) != d->trainInfos().end()) {
        ERR("Failed: dublicate train name ");
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

class OnLaunchTag: public TagHandler {
 public:
  explicit OnLaunchTag(ConfigParser* impl): TagHandler(impl) {
    handlers_[TRAIN] = new OnTrainTag(impl);
    handlers_[CROSSVALIDATION] = new OnCrossvalidationTag(impl);
  }
  ~OnLaunchTag() { DeleteAllFromUnorderedMap(&handlers_); }
  virtual void operator() (TiXmlElement* element) {
    cout << "TOnLaunchExecutor" << endl;
    GenericParse(handlers_, element->FirstChildElement());
  }

 private:
  TagHandlers handlers_;
};

// ====================== ConfiguratorPrivate impl =============================

ConfigParser::ConfigParser() {
  root_ = NULL;
  general_xml_token_ = new OnGeneralXmlToken(this);
  tag_handlers_[CONFIG] = new OnConfigParser(this);
  tag_handlers_[DATA] = new TOnDataTag(this);
  tag_handlers_[LAUNCH] = new OnLaunchTag(this);
}
ConfigParser::~ConfigParser() {
  DeleteAllFromUnorderedMap(&tag_handlers_);
  delete general_xml_token_;
}

void ConfigParser::
parseConfig(const string& file_name) {
  document_ = auto_ptr<TiXmlDocument>(new TiXmlDocument(file_name));
  if (!document_->LoadFile()) {
    throw logic_error("not valid config in " + file_name);
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

  INFO(" LTR Client. Copyright 2011 Yandex");
  INFO(" Experiment started ");

  GenericParse(tag_handlers_,
               root_->FirstChildElement(),
               general_xml_token_);

  cout << "\n\nEnd of loadConfig. Collected data:\n";
  cout << "data_infos_\n" << ToString(dataInfos()) << endl;
  cout << "xml_token_specs\n" << ToString(xmlTokenSpecs()) << endl;
  cout << "train_infos\n" << ToString(trainInfos()) << endl;
  cout << "crossvalidation_infos\n" << ToString(crossvalidationInfos())
       << endl;

  for (TXmlTokenSpecs::iterator it = xmlTokenSpecs().begin();
      it != xmlTokenSpecs().end();
      ++it) {
    ParametrizedInfo& spec = it->second;
    spec.fillDependencyList(xmlTokenSpecs());
  }
}

const ConfigParser::TDataInfos& ConfigParser::dataInfos() const {
  return data_infos_;
}
ConfigParser::TDataInfos& ConfigParser::dataInfos() {
  return data_infos_;
}

const ConfigParser::TXmlTokenSpecs& ConfigParser::xmlTokenSpecs() const {
  return xml_token_specs;
}

ConfigParser::TXmlTokenSpecs& ConfigParser::xmlTokenSpecs() {
  return xml_token_specs;
}

const ConfigParser::TTrainInfos& ConfigParser::trainInfos() const {
  return train_infos;
}

ConfigParser::TTrainInfos& ConfigParser::trainInfos() {
  return train_infos;
}

const ConfigParser::TCrossvalidationInfos&
                                  ConfigParser::crossvalidationInfos() const {
  return crossvalidation_infos;
}

ConfigParser::TCrossvalidationInfos& ConfigParser::crossvalidationInfos() {
  return crossvalidation_infos;
}

const ParametrizedInfo& ConfigParser::findLearner(const string& name) const {
  for (TXmlTokenSpecs::const_iterator it = xml_token_specs.begin();
       it != xml_token_specs.end();
       ++it) {
    const ParametrizedInfo& spec = it->second;
    if (spec.getTagName() == "learner" && spec.getName() == name) {
      return spec;
    }
  }
  throw logic_error("Can not find learner!");
}
const TDataInfo& ConfigParser::findData(const string& name) const {
  for (TDataInfos::const_iterator it = dataInfos().begin();
       it != dataInfos().end();
       ++it) {
    const TDataInfo& data_info = it->second;
    if (data_info.name == name) {
      return data_info;
    }
  }
  throw logic_error("Can not find data!");
}

const string& ConfigParser::rootPath() const {
  return root_path_;
}



