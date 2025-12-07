set(SUTDIR ${CMAKE_CURRENT_SOURCE_DIR}/..)

ecm_add_test(
    ${SUTDIR}/char8stringvalidator.cpp
    ${SUTDIR}/char8stringparser.cpp
    char8stringvalidatortest.cpp

    TEST_NAME char8stringvalidatortest
    LINK_LIBRARIES
        OktetaCore
        Qt6::Gui
        Qt6::Test
)
target_include_directories(char8stringvalidatortest
    PUBLIC
        ${SUTDIR}
)
