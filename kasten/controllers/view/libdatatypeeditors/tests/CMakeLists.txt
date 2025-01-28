set(SUTDIR ${CMAKE_CURRENT_SOURCE_DIR}/..)
set(CONTROLLERSDIR ${SUTDIR}/../..)

ecm_add_test(
    ${SUTDIR}/char8stringvalidator.cpp
    ${SUTDIR}/char8stringparser.cpp
    char8stringvalidatortest.cpp

    TEST_NAME char8stringvalidatortest
    NAME_PREFIX oktetakasten-controllers-libdatatypeeditors-
    LINK_LIBRARIES
        OktetaCore
        Qt5::Gui
        Qt5::Test
)
target_include_directories(char8stringvalidatortest
    PUBLIC
        ${SUTDIR}
)
