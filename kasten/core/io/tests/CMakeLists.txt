set( SUTDIR ${CMAKE_CURRENT_SOURCE_DIR}/.. )

set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}/../..
    ${SUTDIR}
    ${SUTDIR}/..
    ${SUTDIR}/../document
    ${SUTDIR}/filesystem
)

########### io ###############

add_library(oktetakastenio STATIC)
target_link_libraries(oktetakastenio
    PRIVATE
        KastenCore
        OktetaCore
        KF5::I18n
        KF5::CoreAddons
        Qt5::Core
)
target_sources(oktetakastenio
    PRIVATE
        ${SUTDIR}/filesystem/bytearrayrawfileloadthread.cpp
        ${SUTDIR}/filesystem/bytearrayrawfilereloadthread.cpp
        ${SUTDIR}/filesystem/bytearrayrawfilewritethread.cpp
        ${SUTDIR}/filesystem/bytearrayrawfileloadjob.cpp
        ${SUTDIR}/filesystem/bytearrayrawfileconnectjob.cpp
        ${SUTDIR}/filesystem/bytearrayrawfilewritejob.cpp
        ${SUTDIR}/filesystem/bytearrayrawfilereloadjob.cpp
        ${SUTDIR}/filesystem/bytearrayrawfilewritetojob.cpp
)
# Configure export macros to be setup as with a static lib
# as the source files are reused for static oktetakastenio
# Saves doing a separate export header.
target_compile_definitions(oktetakastenio PUBLIC DOKTETAKASTENCORE_STATICLIB)

########### bytearrayrawfilesynchronizertest ###############

ecm_add_test(
    bytearrayrawfilesynchronizertest.cpp
    ${SUTDIR}/../document/bytearraydocument.cpp
    ${SUTDIR}/filesystem/bytearrayrawfilesynchronizer.cpp
    ${SUTDIR}/filesystem/externalbookmarkstorage.cpp
    TEST_NAME bytearrayrawfilesynchronizertest
    NAME_PREFIX oktetakasten-io-
    LINK_LIBRARIES
        oktetakastenio
        KastenCore
        kastencoretestutil
        oktetacoretestutil
        testoktetacore
        KF5::I18n
        KF5::Bookmarks
        Qt5::Test
)

########### bytearrayrawfilesynchronizerfactorytest ###############

ecm_add_test(
    bytearrayrawfilesynchronizerfactorytest.cpp
    ${SUTDIR}/../document/bytearraydocument.cpp
    ${SUTDIR}/filesystem/externalbookmarkstorage.cpp
    ${SUTDIR}/filesystem/bytearrayrawfilesynchronizer.cpp
    ${SUTDIR}/filesystem/bytearrayrawfilesynchronizerfactory.cpp
    TEST_NAME bytearrayrawfilesynchronizerfactorytest
    NAME_PREFIX oktetakasten-io-
    LINK_LIBRARIES
        oktetakastenio
        KastenCore
        kastencoretestutil
        oktetacoretestutil
        testoktetacore
        KF5::I18n
        KF5::Bookmarks
        Qt5::Test
)
