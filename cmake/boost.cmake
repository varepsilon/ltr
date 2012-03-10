SET (Boost_SOURCE_DIR ${Source_Path}/contrib/boost_libs)
SET (Boost_INCLUDE_DIR ${Source_Path}/contrib)
SET (Boost_BINARY_DIR ${PROJECT_BINARY_DIR})

SET(Boost_date_time_SOURCE_DIR ${Boost_SOURCE_DIR}/date_time)
SET(Boost_date_time_SOURCES
  ${Boost_date_time_SOURCE_DIR}/posix_time/posix_time_types.cpp
  ${Boost_date_time_SOURCE_DIR}/gregorian/date_generators.cpp
  ${Boost_date_time_SOURCE_DIR}/gregorian/greg_month.cpp
  ${Boost_date_time_SOURCE_DIR}/gregorian/greg_names.hpp
  ${Boost_date_time_SOURCE_DIR}/gregorian/greg_weekday.cpp
  ${Boost_date_time_SOURCE_DIR}/gregorian/gregorian_types.cpp
)

SET(Boost_filesystem_SOURCE_DIR ${Boost_SOURCE_DIR}/filesystem)
SET(Boost_filesystem_SOURCES
  ${Boost_filesystem_SOURCE_DIR}/codecvt_error_category.cpp
  ${Boost_filesystem_SOURCE_DIR}/operations.cpp
  ${Boost_filesystem_SOURCE_DIR}/path.cpp
  ${Boost_filesystem_SOURCE_DIR}/path_traits.cpp
  ${Boost_filesystem_SOURCE_DIR}/portability.cpp
  ${Boost_filesystem_SOURCE_DIR}/unique_path.cpp
  ${Boost_filesystem_SOURCE_DIR}/utf8_codecvt_facet.cpp
  ${Boost_filesystem_SOURCE_DIR}/windows_file_codecvt.cpp
  ${Boost_filesystem_SOURCE_DIR}/windows_file_codecvt.hpp
)

SET(Boost_system_SOURCE_DIR ${Boost_SOURCE_DIR}/system)
SET(Boost_system_SOURCES
  ${Boost_system_SOURCE_DIR}/error_code.cpp
  ${Boost_system_SOURCE_DIR}/local_free_on_destruction.hpp
)

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
LINK_DIRECTORIES(${Boost_BINARY_DIR})
