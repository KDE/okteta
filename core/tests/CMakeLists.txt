set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}/.. # for oktetacore_export.hpp
  ${CMAKE_CURRENT_SOURCE_DIR}/.. # okteta core includes
)

# make sure to not use KDE_EXPORT or KDE_IMPORT because we link statically
# against a small part of the libs
add_definitions(-DOKTETACORE_STATIC_DEFINE)

# helper
set( oktetacoretestutil_LIB_SRCS
  util/fill.cpp
)

add_library( oktetacoretestutil  STATIC ${oktetacoretestutil_LIB_SRCS} )
target_link_libraries( oktetacoretestutil Qt5::Core )

ecm_add_test(
  arraychangemetricstest.cpp
#  ${CMAKE_CURRENT_SOURCE_DIR}/../arraychangemetrics.cpp
  TEST_NAME arraychangemetricstest
  NAME_PREFIX libokteta-core-
  LINK_LIBRARIES Qt5::Test
)


ecm_add_test(
  addressrangetest.cpp
  TEST_NAME addressrangetest
  NAME_PREFIX libokteta-core-
  LINK_LIBRARIES testoktetacore Qt5::Test
)


ecm_add_test(
  addressrangelisttest.cpp
  TEST_NAME addressrangelisttest
  NAME_PREFIX libokteta-core-
  LINK_LIBRARIES testoktetacore Qt5::Test
)


ecm_add_test(
  fixedsizebytearraymodeltest.cpp
  TEST_NAME fixedsizebytearraymodeltest
  NAME_PREFIX libokteta-core-
  LINK_LIBRARIES testoktetacore Qt5::Test KF5::I18n
)


ecm_add_test(
  abstractbytearraymodeliftest.cpp
  fixedsizebytearraymodelabstractbytearraymodeliftest.cpp
  TEST_NAME fixedsizebytearraymodelabstractbytearraymodeliftest
  NAME_PREFIX libokteta-core-
  LINK_LIBRARIES oktetacoretestutil testoktetacore Qt5::Test KF5::I18n
)


ecm_add_test(
  bytearraymodeltest.cpp
  TEST_NAME bytearraymodeltest
  NAME_PREFIX libokteta-core-
  LINK_LIBRARIES OktetaCore Qt5::Test
)


ecm_add_test(
  abstractbytearraymodeliftest.cpp
  bytearraymodelabstractbytearraymodeliftest.cpp
  TEST_NAME bytearraymodelabstractbytearraymodeliftest
  NAME_PREFIX libokteta-core-
  LINK_LIBRARIES oktetacoretestutil testoktetacore Qt5::Test KF5::I18n
)


ecm_add_test(
  abstractbytearraymodeliftest.cpp
  piecetablebytearraymodelabstractbytearraymodeliftest.cpp
  TEST_NAME piecetablebytearraymodelabstractbytearraymodeliftest
  NAME_PREFIX libokteta-core-
  LINK_LIBRARIES oktetacoretestutil testoktetacore Qt5::Test KF5::I18n
)


ecm_add_test(
  versionableiftest.cpp
  piecetablebytearraymodelversionableiftest.cpp
  TEST_NAME piecetablebytearraymodelversionableiftest
  NAME_PREFIX libokteta-core-
  LINK_LIBRARIES oktetacoretestutil OktetaCore Qt5::Test
)


ecm_add_test(
  valuecodectest.cpp
  TEST_NAME valuecodectest
  NAME_PREFIX libokteta-core-
  LINK_LIBRARIES OktetaCore Qt5::Test
)


ecm_add_test(
  charcodectest.cpp
  TEST_NAME charcodectest
  NAME_PREFIX libokteta-core-
  LINK_LIBRARIES OktetaCore Qt5::Test KF5::I18n
)


ecm_add_test(
  textcharcodectest.cpp
  TEST_NAME textcharcodectest
  NAME_PREFIX libokteta-core-
  LINK_LIBRARIES testoktetacore Qt5::Test KF5::I18n KF5::Codecs
)


ecm_add_test(
  charcodeciftest.cpp
  textcharcodeccharcodeciftest.cpp
  TEST_NAME textcharcodeccharcodeciftest
  NAME_PREFIX libokteta-core-
  LINK_LIBRARIES testoktetacore Qt5::Test KF5::I18n KF5::Codecs
)


ecm_add_test(
  charcodeciftest.cpp ebcdic1047charcodeccharcodeciftest.cpp
  TEST_NAME ebcdic1047charcodeccharcodeciftest
  NAME_PREFIX libokteta-core-
  LINK_LIBRARIES testoktetacore Qt5::Test
)


ecm_add_test(
  bookmarktest.cpp
  TEST_NAME bookmarktest
  NAME_PREFIX libokteta-core-
  LINK_LIBRARIES Qt5::Test
)


ecm_add_test(
  bookmarklisttest.cpp
  TEST_NAME bookmarklisttest
  NAME_PREFIX libokteta-core-
  LINK_LIBRARIES testoktetacore Qt5::Test KF5::I18n
)


