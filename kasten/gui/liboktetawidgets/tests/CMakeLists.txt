set(SUTDIR ${CMAKE_CURRENT_SOURCE_DIR}/..)

list(APPEND ECM_TEST_NAME_PREFIX_IGNORE_DIRS "gui")

add_library(OktetaWidgetsSourcesTestShim INTERFACE)
target_include_directories(OktetaWidgetsSourcesTestShim INTERFACE
    ${SUTDIR}
    ${CMAKE_CURRENT_BINARY_DIR}/../..
)
# Configure export macros to be setup as with a static lib
# as the source files are reused for the tests.
# Saves doing a separate export header.
target_compile_definitions(OktetaWidgetsSourcesTestShim INTERFACE
    -DOKTETAKASTENGUI_STATICLIB
)

ecm_add_test(
    bytearraychar8stringencodertest.cpp
    ${SUTDIR}/bytearraychar8stringencoder.cpp
    TEST_NAME bytearraychar8stringencodertest
    LINK_LIBRARIES
        OktetaWidgetsSourcesTestShim
        OktetaCoreCharCodecs
        Qt6::Core5Compat
        Qt6::Test
)

ecm_add_test(
    bytearraychar8stringdecodertest.cpp
    ${SUTDIR}/bytearraychar8stringdecoder.cpp
    TEST_NAME bytearraychar8stringdecodertest
    LINK_LIBRARIES
        OktetaWidgetsSourcesTestShim
        OktetaCoreCharCodecs
        Qt6::Core5Compat
        Qt6::Test
)

ecm_add_test(
    bytearraychar8stringcodecroundtriptest.cpp
    ${SUTDIR}/bytearraychar8stringdecoder.cpp
    ${SUTDIR}/bytearraychar8stringencoder.cpp
    TEST_NAME bytearraychar8stringcodecroundtriptest
    LINK_LIBRARIES
        OktetaWidgetsSourcesTestShim
        OktetaCoreCharCodecs
        Qt6::Core5Compat
        Qt6::Test
)

ecm_add_test(
    bytearrayutf8stringencodertest.cpp
    ${SUTDIR}/bytearrayutf8stringencoder.cpp
    TEST_NAME bytearrayutf8stringencodertest
    LINK_LIBRARIES
        OktetaWidgetsSourcesTestShim
        OktetaCoreCharCodecs
        Qt6::Core5Compat
        Qt6::Test
)

ecm_add_test(
    bytearrayutf8stringdecodertest.cpp
    ${SUTDIR}/bytearrayutf8stringdecoder.cpp
    TEST_NAME bytearrayutf8stringdecodertest
    LINK_LIBRARIES
        OktetaWidgetsSourcesTestShim
        OktetaCoreCharCodecs
        Qt6::Core5Compat
        Qt6::Test
)

ecm_add_test(
    bytearrayvaluestringdecodertest.cpp
    ${SUTDIR}/bytearrayvaluestringdecoder.cpp
    TEST_NAME bytearrayvaluestringdecodertest
    LINK_LIBRARIES
        OktetaWidgetsSourcesTestShim
        OktetaCoreValueCodecs
        Qt6::Test
)

ecm_add_test(
    bytearrayvalidatortest.cpp
    ${SUTDIR}/bytearraychar8stringdecoder.cpp
    ${SUTDIR}/bytearraychar8stringencoder.cpp
    ${SUTDIR}/bytearraychar8stringvalidator.cpp
    ${SUTDIR}/bytearrayutf8stringdecoder.cpp
    ${SUTDIR}/bytearrayutf8stringencoder.cpp
    ${SUTDIR}/bytearrayutf8stringvalidator.cpp
    ${SUTDIR}/bytearrayvaluestringvalidator.cpp
    ${SUTDIR}/bytearrayvalidator.cpp
    TEST_NAME bytearrayvalidatortest
    LINK_LIBRARIES
        OktetaWidgetsSourcesTestShim
        OktetaCoreCharCodecs
        OktetaCoreValueCodecs
        Qt6::Gui
        Qt6::Core5Compat
        Qt6::Test
)
