set(SUTDIR ${CMAKE_CURRENT_SOURCE_DIR}/..)

add_library(OktetaGuiSourcesTestShim INTERFACE)
target_include_directories(OktetaGuiSourcesTestShim INTERFACE
    ${SUTDIR}
    ${CMAKE_CURRENT_BINARY_DIR}/..
)
# Configure export macros to be setup as with a static lib
# as the source files are reused for the tests.
# Saves doing a separate export header.
target_compile_definitions(OktetaGuiSourcesTestShim INTERFACE
    -DOKTETAGUI_STATICLIB
)

ecm_add_test(
    selectiontest.cpp
    TEST_NAME selectiontest
    NAME_PREFIX libokteta-gui-
    LINK_LIBRARIES OktetaGuiSourcesTestShim OktetaCore Qt5::Test
)

ecm_add_test(
    coordtest.cpp
    TEST_NAME coordtest
    NAME_PREFIX libokteta-gui-
    LINK_LIBRARIES OktetaGuiSourcesTestShim OktetaCore Qt5::Test
)

ecm_add_test(
    coordrangetest.cpp
    TEST_NAME coordrangetest
    NAME_PREFIX libokteta-gui-
    LINK_LIBRARIES OktetaGuiSourcesTestShim OktetaCore Qt5::Test
)

ecm_add_test(
    ${SUTDIR}/coordrangelist.cpp
    coordrangelisttest.cpp
    TEST_NAME coordrangelisttest
    NAME_PREFIX libokteta-gui-
    LINK_LIBRARIES OktetaGuiSourcesTestShim OktetaCore Qt5::Test
)

ecm_add_test(
    ${SUTDIR}/bytearraytablelayout.cpp
    bytearraytablelayouttest.cpp
    TEST_NAME bytearraytablelayouttest
    NAME_PREFIX libokteta-gui-
    LINK_LIBRARIES OktetaGuiSourcesTestShim OktetaCore Qt5::Test
)

ecm_add_test(
    ${SUTDIR}/bytearraytablecursor.cpp
    ${SUTDIR}/bytearraytablelayout.cpp
    bytearraytablecursortest.cpp
    TEST_NAME bytearraytablecursortest
    NAME_PREFIX libokteta-gui-
    LINK_LIBRARIES OktetaGuiSourcesTestShim OktetaCore Qt5::Test
)

ecm_add_test(
    ${SUTDIR}/fontscalingzoomstate.cpp
    fontscalingzoomstatetest.cpp
    TEST_NAME fontscalingzoomstatetest
    NAME_PREFIX libokteta-gui-
    LINK_LIBRARIES OktetaGuiSourcesTestShim Qt5::Gui Qt5::Test
)

ecm_add_test(
    abstractbytearrayviewiftest.cpp
    bytearrayrowviewabstractbytearrayviewiftest.cpp
    TEST_NAME bytearrayrowviewabstractbytearrayviewiftest
    NAME_PREFIX libokteta-gui-
    LINK_LIBRARIES OktetaGui Qt5::Test
)

ecm_add_test(
    abstractbytearrayviewiftest.cpp
    bytearraycolumnviewabstractbytearrayviewiftest.cpp
    TEST_NAME bytearraycolumnviewabstractbytearrayviewiftest
    NAME_PREFIX libokteta-gui-
    LINK_LIBRARIES OktetaGui Qt5::Test
)
