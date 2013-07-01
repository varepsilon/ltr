// Copyright 2013 Yandex
#ifndef LTR_CLIENT_CONFIG_CONSTANTS_H_
#define LTR_CLIENT_CONFIG_CONSTANTS_H_

/**
 * Name of the tag that containes name of the root directory.
 */
static const char* const ROOT_DIR = "root_directory";
/**
 * Name of listwise approach.
 */
static const char* const LISTWISE = "listwise";
/**
 * Name of the tag that containes configuration information.
 */
static const char* const CONFIG = "config";
/**
 * Name of the root tag.
 */
static const char* const ROOT = "LTR_experiment";
/**
 * Name of the tag that containes information about dataset.
 */
static const char* const DATA = "data";
/**
 * Name of the tag that containes launch information.
 */
static const char* const LAUNCH = "launch";
/**
 * Attribute "name"
 */
static const char* const NAME_ATTR = "name";
/**
 * Attribute "format"
 */
static const char* const FORMAT_ATTR = "format";
/**"
 * Attribute "type"
 */
static const char* const TYPE_ATTR = "type";
/**
 * Attribute "approach"
 */
static const char* const APPROACH_ATTR = "approach";
/**
 * Name of the tag that containes train information.
 */
static const char* const TRAIN = "train";
/**
 * Name of the tag that containes crossvalidation information.
 */
static const char* const CROSSVALIDATION = "crossvalidation";
/**
 * Name of the C++ code generation flag.
 */
static const char* const CPP_GEN = "cpp";
/**
 * Name of prediction tag.
 */
static const char* const PREDICT = "predict";
/**
 * Attribute "fold".
 */
static const char* const FOLD_ATTR = "fold";
/**
 * Constant that denotes learners in config.
 */
static const char* const LEARNER = "learner";
/**
 * Constant that denotes measures in config.
 */
static const char* const MEASURE = "measure";
/**
 * Constant that denotes splitters in config.
 */
static const char* const SPLITTER = "splitter";
/**
 * Constant that denotes metrics in config.
 */
static const char* const METRIC = "metric";
/**
 * Constant that denotes predictions aggregators in config.
 */
static const char* const PREDICTIONS_AGGREGATOR = "predictions_aggregator";
/**
 * Constant that denotes neighbour weighters in config.
 */
static const char* const NEIGHBOUR_WEIGHTER = "neighbour_weighter";

#endif  // LTR_CLIENT_CONFIG_CONSTANTS_H_
