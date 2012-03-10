# SET paths to puppy's's sources and binaries
SET (Puppy_SOURCE_DIR ${Source_Path}/contrib/puppy)
SET (Puppy_INCLUDE_DIR ${Source_Path}/contrib)
SET (Puppy_BINARY_DIR ${PROJECT_BINARY_DIR})

# SET puppy's source files
SET (Puppy_HEADERS ${Puppy_SOURCE_DIR}/Context.hpp
				   ${Puppy_SOURCE_DIR}/Primitive.hpp
				   ${Puppy_SOURCE_DIR}/PrimitiveHandle.hpp
				   ${Puppy_SOURCE_DIR}/PrimitiveInline.hpp
				   ${Puppy_SOURCE_DIR}/Puppy.hpp
				   ${Puppy_SOURCE_DIR}/Randomizer.hpp
				   ${Puppy_SOURCE_DIR}/TokenT.hpp
				   ${Puppy_SOURCE_DIR}/Tree.hpp)

SET (Puppy_SOURCES ${Puppy_SOURCE_DIR}/src/Primitive.cpp
				   ${Puppy_SOURCE_DIR}/src/Puppy.cpp
				   ${Puppy_SOURCE_DIR}/src/Tree.cpp)
				   
# Where Puppy's .h files can be found.
INCLUDE_DIRECTORIES(${Puppy_INCLUDE_DIR})

# Where Puppy's libraries can be found.
LINK_DIRECTORIES(${Puppy_BINARY_DIR})
