set(SUTDIR ${CMAKE_CURRENT_SOURCE_DIR}/..)

ecm_add_test(
    ${SUTDIR}/typeeditors/char8stringvalidator.cpp
    ${SUTDIR}/typeeditors/char8stringparser.cpp
    char8stringvalidatortest.cpp

    TEST_NAME char8stringvalidatortest
    LINK_LIBRARIES
        OktetaCore
        Qt5::Gui
        Qt5::Test
)
target_include_directories(char8stringvalidatortest
    PUBLIC
        ${SUTDIR}/typeeditors
)
