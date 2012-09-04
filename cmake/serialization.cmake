SET(Serialization_generator_SOURCES ${Source_Path}/ltr/serialization_test/generator/generator.cc
                      ${Source_Path}/ltr/serialization_test/generator/config.h
					  ${Source_Path}/ltr/serialization_test/generator/generator_utility.cc
					  ${Source_Path}/ltr/serialization_test/generator/generator_utility.h)

SET(Serialization_tester_SOURCES ${Source_Path}/ltr/serialization_test/tester/tester.cc
				   ${Source_Path}/ltr/serialization_test/tester/tester_utility.cc
				   ${Source_Path}/ltr/serialization_test/tester/tester_utility.h)

SOURCE_GROUP(serialization FILES ${Serialization_tester_SOURCES})

MACRO(CreateEmptyTesterFile)
ADD_CUSTOM_COMMAND(OUTPUT ${Source_Path}/ltr/serialization_test/tester/tester.cc
	COMMAND touch ${Source_Path}/ltr/serialization_test/tester/tester.cc
	DEPENDS serialization_generator)
ENDMACRO(CreateEmptyTesterFile)