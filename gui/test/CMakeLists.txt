set( SUTDIR ${CMAKE_CURRENT_SOURCE_DIR}/.. )

include_directories(
  ${SUTDIR}
  ${CMAKE_CURRENT_BINARY_DIR}/..
)

# Configure export macros to be setup as with a static lib
# as the source files are reused for the tests.
# Saves doing a separate export header.
add_definitions(
  -DOKTETAGUI_STATIC_DEFINE
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


ecm_add_test(
  ${SUTDIR}/bytearraytablecursor.cpp
  ${SUTDIR}/bytearraytablelayout.cpp
  bytearraytablecursortest.cpp
  TEST_NAME bytearraytablecursortest
  NAME_PREFIX libokteta-gui-
  LINK_LIBRARIES OktetaCore Qt5::Test )
