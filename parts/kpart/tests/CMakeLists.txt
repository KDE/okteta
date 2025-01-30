include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/..
  ${CMAKE_CURRENT_BINARY_DIR}/..
)

configure_file(oktetaparttest-config.hpp.in ${CMAKE_CURRENT_BINARY_DIR}/oktetaparttest-config.hpp @ONLY)

# TODO: deduplicate object file generation of
ecm_add_test(
  ../part.cpp
  ../browserextension.cpp
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
  KF5::I18n
  KF5::Parts
  Qt5::Test
)

kcoreaddons_desktop_to_json(oktetaparttest ${CMAKE_CURRENT_BINARY_DIR}/../oktetapart.desktop)

