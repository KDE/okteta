include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/..
)

add_definitions(
    -DTESTPATH1=\"${CMAKE_CURRENT_SOURCE_DIR}/oktetaparttest.hpp\"
    -DTESTPATH2=\"${CMAKE_CURRENT_SOURCE_DIR}/oktetaparttest.cpp\"
)

set_source_files_properties(../partfactory.cpp PROPERTIES
    COMPILE_FLAGS -DOKTETA_VERSION=\\\"${Okteta_VERSION}\\\"
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../oktetapart.json.in oktetapart.json @ONLY)

# TODO: deduplicate object file generation of
ecm_add_test(
  ../part.cpp
  ../navigationextension.cpp
  ../partfactory.cpp
  oktetaparttest.cpp
  TEST_NAME oktetaparttest
  NAME_PREFIX kpart-
  LINK_LIBRARIES
  OktetaKastenControllers
  OktetaKastenGui
  OktetaKastenCore
  KastenControllers
  KastenGui
  KastenCore
  KF6::I18n
  KF6::Parts
  Qt6::Test
)
