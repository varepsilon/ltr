# lets list all LTR's binaries


# lets list all LTR's source binaries
SET(LTR_DATA ${Source_Path}/ltr/data/data_set.h
             ${Source_Path}/ltr/data/object_pair.h ${Source_Path}/ltr/data/object_pair.cc
             ${Source_Path}/ltr/data/object_list.h ${Source_Path}/ltr/data/object_list.cc
             ${Source_Path}/ltr/data/object.h ${Source_Path}/ltr/data/object.cc
             ${Source_Path}/ltr/data/features_info.h ${Source_Path}/ltr/data/features_info.cc
             )

SET(LTR_DATA_UTILS ${Source_Path}/ltr/data/utility/object_utility.h)

SET(LTR_UTILS ${Source_Path}/ltr/utility/numerical.h ${Source_Path}/ltr/utility/numerical.cc)

SET(LTR_CORE ${Source_Path}/ltr/interfaces.h)

SET(LTR_FEATURE_CONVERTERS ${Source_Path}/ltr/feature_converters/feature_converter.h
                           ${Source_Path}/ltr/feature_converters/feature_subset_chooser.h
                           ${Source_Path}/ltr/feature_converters/feature_subset_chooser.cc)

# lets divide binaries in groups, for comfort navigation
SOURCE_GROUP(data FILES ${LTR_DATA})
SOURCE_GROUP(utils FILES ${LTR_UTILS})
SOURCE_GROUP(data\\utils FILES ${LTR_DATA_UTILS})
SOURCE_GROUP(core FILES ${LTR_CORE})
SOURCE_GROUP(feature_converters ${LTR_FEATURE_CONVERTERS})


SET(LTR_ALL ${LTR_DATA} ${LTR_DATA_UTILS} ${LTR_UTILS} ${LTR_CORE} ${LTR_FEATURE_CONVERTERS})
