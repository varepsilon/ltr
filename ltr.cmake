# lets list all LTR's binaries


# lets list all LTR's source binaries
SET(LTR_DATA ${Source_Path}/ltr/data/data_set.h
             ${Source_Path}/ltr/data/object_pair.h ${Source_Path}/ltr/data/object_pair.cc
             ${Source_Path}/ltr/data/object_list.h ${Source_Path}/ltr/data/object_list.cc
             ${Source_Path}/ltr/data/object.h ${Source_Path}/ltr/data/object.cc
             ${Source_Path}/ltr/data/features_info.h ${Source_Path}/ltr/data/features_info.cc
	     ${Source_Path}/ltr/data/utility/object_utility.h
             )

SET(LTR_ALL ${LTR_DATA})
