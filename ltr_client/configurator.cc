// Copyright 2011 Yandex

#include "ltr_client/configurator.h"

#include <stdexcept>
#include <memory>
#include <sstream>

#include "boost/lexical_cast.hpp"
#include "boost/algorithm/string/predicate.hpp"

#include "contrib/logog/include/logog.hpp"

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
static const char * const CONFIG          = "config";
static const char * const ROOT            = "LTR_experiment";
static const char * const ROOT_DIR        = "root_directory";
static const char * const DATA            = "data";
static const char * const LAUNCH          = "launch";
static const char * const NAME_ATTR       = "name";
static const char * const FORMAT_ATTR     = "format";
static const char * const TYPE_ATTR       = "type";
static const char * const APPROACH_ATTR   = "approach";
static const char * const PARAMETERS      = "parameters";
static const char * const WEAK_LEARNER    = "weak_learner";
static const char * const TRAIN           = "train";
static const char * const CROSSVALIDATION = "crossvalidation";
static const char * const CPP_GEN         = "cpp";
static const char * const PREDICT         = "predict";
static const char * const FOLD_ATTR       = "fold";

static const char * const LEARNER         = "learner";
static const char * const MEASURE         = "measure";
}

// =========================== various helpers =================================

template <class Key, class Value>
static inline void deleteAllFromUnorderedMap(
    boost::unordered_map<Key, Value> *container
    ) {
  assert(container);
  // C++11 We have no unique_ptr so we MUST delete pointers manually
  for (typename boost::unordered_map<Key, Value>::const_iterator it =
       container->begin();
       it != container->end();
       ++it) {
    delete it->second;
  }
}

class TExecutor {
 public:
  explicit TExecutor(ConfiguratorPrivate* impl)
    : d(impl) {}
  virtual ~TExecutor() {}
  virtual void operator()(TiXmlElement* element) = 0;
 protected:
  ConfiguratorPrivate* d;
};

typedef boost::unordered_map<string, TExecutor*> TStrExecMap;

static inline void genericParse(const TStrExecMap& handlers,
                                TiXmlNode* node,
                                TExecutor* on_unknown_token = NULL) {
  for (; node; node = node->NextSibling()) {
    if (node->Type() != TiXmlNode::TINYXML_ELEMENT) {
      continue;
    }

    TiXmlElement* element = node->ToElement();
    if (!element) {
      throw logic_error("Can not convert node to element");
    }

    const TStrExecMap::const_iterator it = handlers.find(node->Value());
    if (it == handlers.end()) {
      if (on_unknown_token) {
        (*on_unknown_token)(element);
      } else {
        throw logic_error(string("Unknown token Value: ") +
                          node->Value() + ", Type: " +
                          boost::lexical_cast<string>(node->Type()) +
                          "\n");
      }
    } else {
      (*it->second)(element);
    }
  }
}

template <class TCont>
static inline bool Contains(const TCont& cont,
                            const typename TCont::key_type& key) {
  return cont.find(key) != cont.end();
}

template <class ValueType>
static inline ValueType &SafeInsert(
    boost::unordered_map<string, ValueType> &container,
    const char* key) {
  if (!key) {
    throw logic_error("empty name!!!!");
  }
  if (Contains(container, key)) {
    throw logic_error("Container already contains " + string(key) + "!!!");
  }
  return container[key];
}

static inline void SafeInsert(
    boost::unordered_set<string>& cont,
    const char* key) {
  if (!key) {
    return;
  }
  if (Contains(cont, key)) {
    throw logic_error("Container already contains " + string(key) + "!!!");
  }
  cont.insert(key);
}

template <class Key, class Value>
inline string toString(const boost::unordered_map<Key, Value>& cont) {
  typedef typename boost::unordered_map<Key, Value>::const_iterator TIter;
  stringstream out(stringstream::out);
  size_t i = 0;
  for (TIter it = cont.begin(); it != cont.end(); ++it)
    out << ++i << ")" << it->first << " " << toString(it->second) << '\n';
  return out.str();
}

template <class Value>
inline string toString(const boost::unordered_set<Value>& cont) {
  typedef typename boost::unordered_set<Value>::const_iterator TIter;
  stringstream out(stringstream::out);
  out << "set(";
  for (TIter it = cont.begin(); it != cont.end(); ++it)
    out << *it << "; ";
  out << ")";
  return out.str();
}

string toString(const TDataInfo& info) {;
  stringstream out(stringstream::out);
  out << "TDataInfo(name=" << info.name
      << ", approach=" << info.approach
      << ", format=" << info.format
      << ", file_name=" << info.file_name
      << ")";
  return out.str();
}

string toString(const TTrainInfo& info) {
  stringstream out(stringstream::out);
  out << "TTrainInfo(name=" << info.name
      << ", data=" << info.data
      << ", leatner=" << info.learner
      << ", predicts=" << toString(info.predicts)
      << ", gen_cpp=" << info.gen_cpp
      << ")";
  return out.str();
}

string toString(const TCrossvalidationInfo& info) {
  stringstream out(stringstream::out);
  out << "TCrossvalidationInfo(fold=" << info.fold
      << ", learners=" << toString(info.learners)
      << ", measures=" << toString(info.measures)
      << ", datas=" << toString(info.datas)
      << ")";
  return out.str();
}

// =============================== TXmlTokenSpec ============================

class TXmlTokenSpecPrivate {
 public:
  void checkAvailability(const Configurator::TXmlTokenSpecs& token_specs);

  string tag_name;
  string name;
  string type;
  string approach;
  ltr::ParametersContainer parameters;
  TXmlTokenSpecList dependency_specs;
};
void TXmlTokenSpecPrivate::checkAvailability(
    const Configurator::TXmlTokenSpecs& token_specs) {
  dependency_specs.clear();

  typedef ltr::ParametersContainer::NameValue<const TXmlTokenDependency>
      TNameValue;
  typedef list<TNameValue> TDependencies;
  const TDependencies& my_dependencies =
      parameters.getValuesByType<const TXmlTokenDependency>();

  for (TDependencies::const_iterator my_dependency_it = my_dependencies.begin();
      my_dependency_it != my_dependencies.end();
      ++my_dependency_it) {
    const TNameValue& dependency = *my_dependency_it;
    const TXmlTokenSpec* found = NULL;

    for (Configurator::TXmlTokenSpecs::const_iterator it = token_specs.begin();
        it != token_specs.end();
        ++it) {
      const string& name = it->first;
      const TXmlTokenSpec& spec = it->second;
      if (dependency.value.parameter_name == name) {
        found = &spec;
        break;
      }
    }

    if (!found) {
      throw logic_error("TXmlTokenSpecPrivate::checkAvailability: "
                        "Could not resolve dependency " +
                        dependency.value.parameter_name);
    }
    dependency_specs.push_back(found);
  }
}

TXmlTokenSpec::TXmlTokenSpec()
  : d(new TXmlTokenSpecPrivate) {}

TXmlTokenSpec::TXmlTokenSpec(const TXmlTokenSpec& other)
  : d(new TXmlTokenSpecPrivate) {
  *this = other;
}

TXmlTokenSpec& TXmlTokenSpec::operator= (const TXmlTokenSpec& other) {
  if (this == &other) {
    return *this;
  }
  d->name = other.name();
  d->type = other.type();
  d->approach = other.approach();
  d->parameters = other.parameters();
  d->dependency_specs = other.dependencySpecs();
  return *this;
}


TXmlTokenSpec::~TXmlTokenSpec() {
  delete d;
}

const string& TXmlTokenSpec::tagName() const {
  return d->tag_name;
}
const string& TXmlTokenSpec::name() const {
  return d->name;
}
const string& TXmlTokenSpec::type() const {
  return d->type;
}
const string& TXmlTokenSpec::approach() const {
  return d->approach;
}
const ltr::ParametersContainer& TXmlTokenSpec::parameters() const {
  return d->parameters;
}
const TXmlTokenSpecList& TXmlTokenSpec::dependencySpecs() const {
  return d->dependency_specs;
}

string toString(const TXmlTokenSpec& info) {
  stringstream out(stringstream::out);
  out << "TXmlTokenSpec(name=" << info.name()
      << ", type=" << info.type()
      << ", approach=" << info.approach()
      << ", parameters=" << info.parameters().toString()
         << ", my dependencies=(";
  for (TXmlTokenSpecList::const_iterator it = info.dependencySpecs().begin();
      it != info.dependencySpecs().end();
      ++it) {
    out << (*it)->name() << ", ";
  }
  out << "))";
  return out.str();
}

class ConfiguratorPrivate {
 public:
  ConfiguratorPrivate();
  ~ConfiguratorPrivate();

  TStrExecMap handlers_;
  TExecutor* general_xml_token;

  auto_ptr<TiXmlDocument> document_;
  TiXmlElement* root_;
  string root_path_;

  Configurator::TDataInfos data_infos_;
  Configurator::TXmlTokenSpecs xml_token_specs;
  Configurator::TTrainInfos train_infos;
  Configurator::TCrossvalidationInfos crossvalidation_infos;
};

// =============================== Config parsing ============================

class TOnConfigExecutor: public TExecutor {
 public:
  explicit TOnConfigExecutor(ConfiguratorPrivate* impl): TExecutor(impl) { }
  virtual void operator()(TiXmlElement* element) {
    cout << "TOnConfigExecutor" << endl;
  }  // We already read it
};

// =============================== Data parsing ==============================

class TOnDataExecutor: public TExecutor {
 public:
  explicit TOnDataExecutor(ConfiguratorPrivate* impl): TExecutor(impl) { }
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

    if (d->data_infos_.find(name) != d->data_infos_.end()) {
        throw logic_error("dublicate data name " + string(name));
    }

    d->data_infos_[name] = TDataInfo(name, approach, format, file_name);
  }
};

// =========================== General XML token parsing ======================

class TOnParameterExecutor: public TExecutor {
 public:
  explicit TOnParameterExecutor(ConfiguratorPrivate *impl)
    : TExecutor(impl)
    , container(NULL) {}

  void setContainer(ltr::ParametersContainer *cont) {
    container = cont;
  }

  virtual void operator()(TiXmlElement *element) {
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
    const string *type = element->Attribute(string(TYPE_ATTR));
    cout << "TOnParametersExecutor: name:" << name <<
            " val: " << val << " type: " << type << endl;
    addParameter(name, type ? *type : guessType(val), val);
  }

 private:
  static bool toBool(const string &value) {
    if (boost::iequals(value, string("true"))) {
      return true;
    } else if (boost::iequals(value, string("false"))) {
      return false;
    } else {
      throw logic_error("Can not convert " + value + " to bool!");
    }
  }
  static int toInt(const string &value) {
    try {
      return boost::lexical_cast<int>(value);
    } catch(boost::bad_lexical_cast &) {
      throw logic_error("Can not convert " + value + " to int!");
    }
  }
  static double toDouble(const string &value) {
    try {
      return boost::lexical_cast<double>(value);
    } catch(boost::bad_lexical_cast &) {
      throw logic_error("Can not convert " + value + " to double!");
    }
  }
  static const string XML_TOKEN_DEPENDENCY_TYPE;

  void addParameter(const string &name,
                    const string &type,
                    const string &value) {
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
  static string guessType(const string &value) {
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
  static string guessTypeOfOneElement(const string &value) {
    if (boost::iequals(value, string("true")) ||
       boost::iequals(value, string("false"))) {
      return "bool";
    }

    const string::size_type pos_of_decimal_point = value.find('.');
    if (pos_of_decimal_point != string::npos) {
      try {
        boost::lexical_cast<double>(value);
      } catch(boost::bad_lexical_cast &) {
        return XML_TOKEN_DEPENDENCY_TYPE;
      }
      return "double";
    }

    try {
      boost::lexical_cast<int>(value);
    } catch(boost::bad_lexical_cast &) {
      return XML_TOKEN_DEPENDENCY_TYPE;
    }
    return "int";
  }

  ltr::ParametersContainer *container;
};
const char* TOnParameterExecutor::XML_TOKEN_DEPENDENCY_TYPE =
    string("TXmlTokenDependency");


class TOnGeneralXmlToken: public TExecutor {
 public:
  explicit TOnGeneralXmlToken(ConfiguratorPrivate *impl): TExecutor(impl) {
    parameters_executor = new TOnParameterExecutor(impl);
  }
  ~TOnGeneralXmlToken() {
    delete parameters_executor;
  }

  virtual void operator()(TiXmlElement *element) {
    cout << "TOnGeneralXmlToken" << endl;

    const char* name = element->Attribute(NAME_ATTR);
    TXmlTokenSpec& spec = SafeInsert(d->xml_token_specs,
                                     name);
    const char* type = element->Attribute(TYPE_ATTR);
    if (!type) {
      throw logic_error("no 'type' attribute");
    }

    const char *approach = element->Attribute(APPROACH_ATTR);
    if (!approach) {
      WARN("No approach defined '%s'. Try to define automatically.", name);
      approach = "";
    }
    const char *tag_name = element->Value();
    if (!tag_name) {
      throw logic_error("no tag name");
    }

    spec.d->tag_name = tag_name;
    spec.d->name = name;
    spec.d->type = type;
    spec.d->approach = approach;

    parameters_executor->setContainer(&spec.d->parameters);
    genericParse(TStrExecMap(),
                 element->FirstChildElement(),
                 parameters_executor);
  }

 private:
  TOnParameterExecutor *parameters_executor;
};

//============================= Launch parsing =================================

class TOnCVLearnerExecutor: public TExecutor {
  public:
  explicit TOnCVLearnerExecutor(ConfiguratorPrivate *impl): TExecutor(impl) {
    info = NULL;
  }
  ~TOnCVLearnerExecutor() { }
  void setInfo(TCrossvalidationInfo *ti) {
    info = ti;
  }
  virtual void operator()(TiXmlElement *element) {
    cout << "TOnCVLearnerExecutor" << endl;
    assert(info);
    SafeInsert(info->learners, element->GetText());
  }

 private:
  TCrossvalidationInfo *info;
};

class TOnCVMeasureExecutor: public TExecutor {
  public:
  explicit TOnCVMeasureExecutor(ConfiguratorPrivate *impl): TExecutor(impl) {
    info = NULL;
  }
  ~TOnCVMeasureExecutor() { }
  void setInfo(TCrossvalidationInfo *ti) {
    info = ti;
  }
  virtual void operator()(TiXmlElement *element) {
    cout << "TOnCVMeasureExecutor" << endl;
    assert(info);
    SafeInsert(info->measures, element->GetText());
  }

 private:
  TCrossvalidationInfo *info;
};

class TOnCVDataExecutor: public TExecutor {
  public:
  explicit TOnCVDataExecutor(ConfiguratorPrivate* impl)
    : TExecutor(impl)
    , info(NULL) {}
  ~TOnCVDataExecutor() {}
  void setInfo(TCrossvalidationInfo* trainInfo) {
    info = trainInfo;
  }
  virtual void operator() (TiXmlElement* element) {
    cout << "TOnCVDataExecutor" << endl;
    assert(info);
    SafeInsert(info->datas, element->GetText());
  }

 private:
  TCrossvalidationInfo *info;
};



class TOnCrossvalidationExecutor: public TExecutor {
 public:
  explicit TOnCrossvalidationExecutor(ConfiguratorPrivate* impl):
    TExecutor(impl) {
    handlers_[LEARNER] = learner_executor = new TOnCVLearnerExecutor(impl);
    handlers_[MEASURE] = measure_executor = new TOnCVMeasureExecutor(impl);
    handlers_[DATA] = data_executor = new TOnCVDataExecutor(impl);
  }
  ~TOnCrossvalidationExecutor() {
    deleteAllFromUnorderedMap(&handlers_);
  }
  virtual void operator()(TiXmlElement* element) {
    cout << "TOnCrossvalidationExecutor" << endl;

    const char* fold = element->Attribute(FOLD_ATTR);
    if (!fold) {
        ERR("Failed: <crossvalidation> with no fold");
        return;
    }

    TCrossvalidationInfo new_fold_info(fold);
    d->crossvalidation_infos[fold] = new_fold_info;
    learner_executor->setInfo(&d->crossvalidation_infos[fold]);
    measure_executor->setInfo(&d->crossvalidation_infos[fold]);
    data_executor->setInfo(&d->crossvalidation_infos[fold]);
    genericParse(handlers_, element->FirstChildElement());
  }

 private:
  TOnCVLearnerExecutor* learner_executor;
  TOnCVMeasureExecutor* measure_executor;
  TOnCVDataExecutor* data_executor;
  TStrExecMap handlers_;
};

class TOnPredictExecutor: public TExecutor {
  public:
  explicit TOnPredictExecutor(ConfiguratorPrivate* impl)
    : TExecutor(impl)
    , info(NULL) {}

  ~TOnPredictExecutor() {}
  void setTrainInfo(TTrainInfo* trainInfo) {
    info = trainInfo;
  }

  virtual void operator() (TiXmlElement* element) {
    cout << "TOnPredictExecutor" << endl;
    assert(info);
    SafeInsert(info->predicts, element->GetText());
  }

 private:
  TTrainInfo *info;
};

class TOnCppGenExecutor: public TExecutor {
  public:
  explicit TOnCppGenExecutor(ConfiguratorPrivate* impl)
    : TExecutor(impl)
    , info(NULL) {}

  ~TOnCppGenExecutor() {}
  void setTrainInfo(TTrainInfo* trainInfo) {
    info = trainInfo;
  }

  virtual void operator() (TiXmlElement* element) {
    assert(info);
    info->gen_cpp = true;
  }

 private:
  TTrainInfo *info;
};

class TOnTrainExecutor: public TExecutor {
 public:
  explicit TOnTrainExecutor(ConfiguratorPrivate* impl): TExecutor(impl) {
    cpp_gen_executor =  new TOnCppGenExecutor(impl);
    predict_executor = new TOnPredictExecutor(impl);
    handlers_[CPP_GEN] = cpp_gen_executor;
    handlers_[PREDICT] = predict_executor;
  }
  ~TOnTrainExecutor() { deleteAllFromUnorderedMap(&handlers_); }
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
    if (d->train_infos.find(name) != d->train_infos.end()) {
        ERR("Failed: dublicate train name ");
        return;
    }

    TTrainInfo new_train_info(name, data, learner);
    d->train_infos[name] = new_train_info;

    cpp_gen_executor->setTrainInfo(&d->train_infos[name]);
    predict_executor->setTrainInfo(&d->train_infos[name]);
    genericParse(handlers_, element->FirstChildElement());
  }

 private:
  TStrExecMap handlers_;
  TOnCppGenExecutor* cpp_gen_executor;
  TOnPredictExecutor* predict_executor;
};

class TOnLaunchExecutor: public TExecutor {
 public:
  explicit TOnLaunchExecutor(ConfiguratorPrivate* impl): TExecutor(impl) {
    handlers_[TRAIN] = new TOnTrainExecutor(impl);
    handlers_[CROSSVALIDATION] = new TOnCrossvalidationExecutor(impl);
  }
  ~TOnLaunchExecutor() { deleteAllFromUnorderedMap(&handlers_); }
  virtual void operator() (TiXmlElement* element) {
    cout << "TOnLaunchExecutor" << endl;
    genericParse(handlers_, element->FirstChildElement());
  }

 private:
  TStrExecMap handlers_;
};

// ====================== ConfiguratorPrivate impl =============================

ConfiguratorPrivate::ConfiguratorPrivate() {
  root_ = NULL;
  general_xml_token = new TOnGeneralXmlToken(this);
  handlers_[CONFIG] = new TOnConfigExecutor(this);
  handlers_[DATA] = new TOnDataExecutor(this);
  handlers_[LAUNCH] = new TOnLaunchExecutor(this);
}
ConfiguratorPrivate::~ConfiguratorPrivate() {
  deleteAllFromUnorderedMap(&handlers_);
  delete general_xml_token;
}

Configurator::Configurator()
  : d(new ConfiguratorPrivate) {}

Configurator::~Configurator() {
  delete d;
}

void Configurator::loadConfig(const string& file_name) {
  d->document_ = auto_ptr<TiXmlDocument>(new TiXmlDocument(file_name));
  if (!d->document_->LoadFile()) {
    throw logic_error("not valid config in " + file_name);
  }

  d->root_ = d->document_->FirstChildElement(ROOT);
  if (!d->root_) {
    throw logic_error("can't find <LTR_experiment>");
  }

  TiXmlElement* config = d->root_->FirstChildElement(CONFIG);
  if (!config) {
    throw logic_error("can't find <config>");
  }

  TiXmlElement* root_dir = config->FirstChildElement(ROOT_DIR);
  if (!root_dir || !root_dir->GetText()) {
      throw logic_error("no root directory specified");
  }

  d->root_path_ = root_dir->GetText();

  INFO(" LTR Client. Copyright 2011 Yandex");
  INFO(" Experiment started ");

  genericParse(d->handlers_,
               d->root_->FirstChildElement(),
               d->general_xml_token);

  cout << "\n\nEnd of loadConfig. Collected data:\n";
  cout << "data_infos_\n" << toString(d->data_infos_) << endl;
  cout << "xml_token_specs\n" << toString(d->xml_token_specs) << endl;
  cout << "train_infos\n" << toString(d->train_infos) << endl;
  cout << "crossvalidation_infos\n" << toString(d->crossvalidation_infos)
       << endl;

  for (TXmlTokenSpecs::iterator it = d->xml_token_specs.begin();
      it != d->xml_token_specs.end();
      ++it) {
    TXmlTokenSpec &spec = it->second;
    spec.d->checkAvailability(d->xml_token_specs);
  }
}

const Configurator::TDataInfos& Configurator::dataInfos() const {
  return d->data_infos_;
}
const Configurator::TXmlTokenSpecs& Configurator::xmlTokenSpecs() const {
  return d->xml_token_specs;
}
const Configurator::TTrainInfos& Configurator::trainInfos() const {
  return d->train_infos;
}
const Configurator::TCrossvalidationInfos&
                                  Configurator::crossvalidationInfos() const {
  return d->crossvalidation_infos;
}

const TXmlTokenSpec& Configurator::findLearner(const string& name) const {
  for (TXmlTokenSpecs::const_iterator it = d->xml_token_specs.begin();
       it != d->xml_token_specs.end();
       ++it) {
    const TXmlTokenSpec& spec = it->second;
    if (spec.tagName() == "learner" && spec.name() == name) {
      return spec;
    }
  }
  throw logic_error("Can not find learner!");
}
const TDataInfo& Configurator::findData(const string& name) const {
  for (TDataInfos::const_iterator it = d->data_infos_.begin();
       it != d->data_infos_.end();
       ++it) {
    const TDataInfo& data_info = it->second;
    if (data_info.name == name) {
      return data_info;
    }
  }
  throw logic_error("Can not find data!");
}

const string& Configurator::rootPath() const {
  return d->root_path_;
}



