set(SUTDIR ${CMAKE_CURRENT_SOURCE_DIR}/..)

add_library(OktetaCoreSourcesTestShim INTERFACE)
target_include_directories(OktetaCoreSourcesTestShim INTERFACE
    ${SUTDIR}
    ${CMAKE_CURRENT_BINARY_DIR}/.. # for oktetacore_export.hpp
)
# Configure export macros to be setup as with a static lib
# as the source files are reused for the tests.
# Saves doing a separate export header.
target_compile_definitions(OktetaCoreSourcesTestShim INTERFACE
    -DOKTETACORE_STATICLIB
)

# helper
add_library(OktetaCoreTestUtil STATIC)
target_sources(OktetaCoreTestUtil PRIVATE
    util/fill.cpp
)
target_link_libraries(OktetaCoreTestUtil
    PUBLIC
        OktetaCore
        Qt6::Core
)

# variant for static version of OktetaCore
add_library(OktetaCoreSourcesShimTestUtil STATIC)
target_sources(OktetaCoreSourcesShimTestUtil PRIVATE
    util/fill.cpp
)
target_link_libraries(OktetaCoreSourcesShimTestUtil
    PUBLIC
      OktetaCoreSourcesTestShim
      Qt6::Core
)

ecm_add_test(
    arraychangemetricstest.cpp
    #  ${CMAKE_CURRENT_SOURCE_DIR}/../arraychangemetrics.cpp
    TEST_NAME arraychangemetricstest
    LINK_LIBRARIES
        OktetaCoreSourcesTestShim
        Qt6::Test
)


ecm_add_test(
    addressrangetest.cpp
    TEST_NAME addressrangetest
    LINK_LIBRARIES
        OktetaCoreTypes
        Qt6::Test
)


ecm_add_test(
    addressrangelisttest.cpp
    TEST_NAME addressrangelisttest
    LINK_LIBRARIES
        OktetaCoreTypes
        Qt6::Core5Compat
        Qt6::Test
)


ecm_add_test(
    fixedsizebytearraymodeltest.cpp
    TEST_NAME fixedsizebytearraymodeltest
    LINK_LIBRARIES
        OktetaCoreByteArrayModels
        Qt6::Test
)


ecm_add_test(
    abstractbytearraymodeliftest.cpp
    fixedsizebytearraymodelabstractbytearraymodeliftest.cpp
    TEST_NAME fixedsizebytearraymodelabstractbytearraymodeliftest
    LINK_LIBRARIES
        OktetaCoreSourcesShimTestUtil
        OktetaCoreByteArrayModels
        Qt6::Test
)


ecm_add_test(
    bytearraymodeltest.cpp
    TEST_NAME bytearraymodeltest
    LINK_LIBRARIES
        OktetaCore
        Qt6::Test
)


ecm_add_test(
    abstractbytearraymodeliftest.cpp
    bytearraymodelabstractbytearraymodeliftest.cpp
    TEST_NAME bytearraymodelabstractbytearraymodeliftest
    LINK_LIBRARIES
        OktetaCoreSourcesShimTestUtil
        OktetaCoreByteArrayModels
        Qt6::Test
)


ecm_add_test(
    abstractbytearraymodeliftest.cpp
    piecetablebytearraymodelabstractbytearraymodeliftest.cpp
    TEST_NAME piecetablebytearraymodelabstractbytearraymodeliftest
    LINK_LIBRARIES
        OktetaCoreSourcesShimTestUtil
        OktetaCoreByteArrayModels
        Qt6::Test
)


ecm_add_test(
    versionableiftest.cpp
    piecetablebytearraymodelversionableiftest.cpp
    TEST_NAME piecetablebytearraymodelversionableiftest
    LINK_LIBRARIES
        OktetaCoreTestUtil
        OktetaCore
        Qt6::Test
)


ecm_add_test(
    valuecodectest.cpp
    TEST_NAME valuecodectest
    LINK_LIBRARIES
        OktetaCoreValueCodecs
        Qt6::Test
)


ecm_add_test(
    charcodectest.cpp
    TEST_NAME charcodectest
    LINK_LIBRARIES
        OktetaCoreCharCodecs
        Qt6::Test
)


ecm_add_test(
    textcharcodectest.cpp
    TEST_NAME textcharcodectest
    LINK_LIBRARIES
        OktetaCoreCharCodecs
        Qt6::Test
)


ecm_add_test(
    charcodeciftest.cpp
    textcharcodeccharcodeciftest.cpp
    TEST_NAME textcharcodeccharcodeciftest
    LINK_LIBRARIES
        OktetaCoreCharCodecs
        Qt6::Test
)


ecm_add_test(
    charcodeciftest.cpp
    ebcdic1047charcodeccharcodeciftest.cpp
    TEST_NAME ebcdic1047charcodeccharcodeciftest
    LINK_LIBRARIES
        OktetaCoreCharCodecs
        Qt6::Test
)


ecm_add_test(
    bookmarktest.cpp
    TEST_NAME bookmarktest
    LINK_LIBRARIES
        OktetaCoreSourcesTestShim
        Qt6::Test
)


ecm_add_test(
    bookmarklisttest.cpp
    TEST_NAME bookmarklisttest
    LINK_LIBRARIES
        OktetaCoreBookmark
        Qt6::Test
)
