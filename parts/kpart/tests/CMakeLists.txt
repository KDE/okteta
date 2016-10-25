if (NOT Qt5Test_FOUND)
  return()
endif()

include(ECMAddTests)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/..
)

add_definitions(
    -DTESTPATH1=\"${CMAKE_CURRENT_SOURCE_DIR}/oktetaparttest.h\"
    -DTESTPATH2=\"${CMAKE_CURRENT_SOURCE_DIR}/oktetaparttest.cpp\"
)

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

if(${KF5_VERSION} VERSION_LESS "5.16.0")
    kcoreaddons_desktop_to_json(oktetaparttest ../oktetapart.desktop)
else()
    kcoreaddons_desktop_to_json(oktetaparttest ../oktetapart.desktop  SERVICE_TYPES kpart.desktop)
endif()

