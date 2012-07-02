// Copyright 2012 Yandex

#ifndef LTR_FEATURE_CONVERTERS_UTILITY_UTILITY_H_
#define LTR_FEATURE_CONVERTERS_UTILITY_UTILITY_H_

#include <vector>

#include "ltr/feature_converters/feature_converter.h"

namespace ltr {
namespace utility {
/** Function converts all objects in PointwiseDataSet
 * @param converter - Converter to apply
 * @param source_dataset - Dataset to convert
 * @param converted_dataset - Dataset with converted objects
 * 
 */
void ApplyFeatureConverter(FeatureConverter::ConstPtr converter,
                           const PointwiseDataSet& source_dataset,
                           PointwiseDataSet* converted_dataset);

/** Function converts all objects in PairwiseDataSet
 * @param converter - Converter to apply
 * @param source_dataset - Dataset to convert
 * @param converted_dataset - Dataset with converted object pairs
 * 
 */
void ApplyFeatureConverter(FeatureConverter::ConstPtr converter,
                           const PairwiseDataSet& source_dataset,
                           PairwiseDataSet* converted_dataset);

/** Function converts all objects in ListwiseDataSet
 * @param converter - Converter to apply
 * @param source_dataset - Dataset to convert
 * @param converted_dataset - Dataset with converted object lists
 * 
 */
void ApplyFeatureConverter(FeatureConverter::ConstPtr converter,
                           const ListwiseDataSet& source_dataset,
                           ListwiseDataSet* converted_dataset);
}
}
#endif  // LTR_FEATURE_CONVERTERS_UTILITY_UTILITY_H_
