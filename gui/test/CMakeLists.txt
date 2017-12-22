set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )
set( SUTDIR ${CMAKE_CURRENT_SOURCE_DIR}/.. )

# make sure to not use KDE_EXPORT or KDE_IMPORT because we link statically
# against a small part of the libs
add_definitions( -DOKTETACORE_STATIC_DEFINE -DOKTETAGUI_STATIC_DEFINE )

include_directories(
  ${SUTDIR}
  ${CMAKE_CURRENT_BINARY_DIR}/..
)

ecm_add_test(
  selectiontest.cpp
  TEST_NAME selectiontest
  NAME_PREFIX libokteta-gui-
  LINK_LIBRARIES OktetaCore Qt5::Test )

ecm_add_test(
  coordtest.cpp
  TEST_NAME coordtest
  NAME_PREFIX libokteta-gui-
  LINK_LIBRARIES OktetaCore Qt5::Test )


ecm_add_test(
  coordrangetest.cpp
  TEST_NAME coordrangetest
  NAME_PREFIX libokteta-gui-
  LINK_LIBRARIES OktetaCore Qt5::Test )


ecm_add_test(
  ${SUTDIR}/coordrangelist.cpp
  coordrangelisttest.cpp
  TEST_NAME coordrangelisttest
  NAME_PREFIX libokteta-gui-
  LINK_LIBRARIES OktetaCore Qt5::Test )


ecm_add_test(
  ${SUTDIR}/bytearraytablelayout.cpp
  bytearraytablelayouttest.cpp
  TEST_NAME bytearraytablelayouttest
  NAME_PREFIX libokteta-gui-
  LINK_LIBRARIES OktetaCore Qt5::Test )
# FIXME:
# this is a rather stupid workaround - you should link to liboktetagui instead
set_tests_properties( libokteta-gui-bytearraytablelayouttest PROPERTIES COMPILE_FLAGS "-DMAKE_OKTETAGUI_LIB" )


ecm_add_test(
  ${SUTDIR}/bytearraytablecursor.cpp
  ${SUTDIR}/bytearraytablelayout.cpp
  bytearraytablecursortest.cpp
  TEST_NAME bytearraytablecursortest
  NAME_PREFIX libokteta-gui-
  LINK_LIBRARIES OktetaCore Qt5::Test )
# FIXME:
# this is a rather stupid workaround - you should link to liboktetagui instead
set_tests_properties(libokteta-gui-bytearraytablecursortest PROPERTIES COMPILE_FLAGS "-DMAKE_OKTETAGUI_LIB" )


########### mvctest ###############

#set( mvctest_SRCS mvctest.cpp )

##kde4_add_unit_test(mvctest TESTNAME okteta-mvctest  ${mvctest_SRCS} )

# target_link_libraries( mvctest  oktetacore oktetagui )

########### install files ###############
