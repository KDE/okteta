set( SUTDIR ${CMAKE_CURRENT_SOURCE_DIR}/.. )

# make sure to not use KDE_EXPORT or KDE_IMPORT because we link statically
# against a small part of the libs
add_definitions(-DOKTETACORE_EXPORT=)
add_definitions(-DOKTETAKASTENCORE_EXPORT=)
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
        Qt::Core
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
        Qt::Test
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
        Qt::Test
)
