set( SUTDIR ${CMAKE_CURRENT_SOURCE_DIR}/.. )

add_library(oktetakastenio STATIC)
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
target_link_libraries(oktetakastenio
    PUBLIC
        KastenCore
        OktetaCore
    PRIVATE
        KF5::I18n
        KF5::CoreAddons
)
target_include_directories(oktetakastenio PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}/../..
    ${SUTDIR}
    ${SUTDIR}/..
    ${SUTDIR}/../document
    ${SUTDIR}/filesystem
)
# Configure export macros to be setup as with a static lib
# as the source files are reused for static oktetakastenio
# Saves doing a separate export header.
target_compile_definitions(oktetakastenio
    PUBLIC
        OKTETAKASTENCORE_STATICLIB
    PRIVATE
        TRANSLATION_DOMAIN=\"liboktetakasten\"
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
        KastenCoreTestUtil
        KastenCore
        OktetaCoreTestUtil
        KF5::I18n
        KF5::Bookmarks
        Qt5::Test
)
target_compile_definitions(bytearrayrawfilesynchronizertest PRIVATE -DTRANSLATION_DOMAIN=\"liboktetakasten\")

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
        KastenCoreTestUtil
        KastenCore
        OktetaCoreTestUtil
        KF5::I18n
        KF5::Bookmarks
        Qt5::Test
)
target_compile_definitions(bytearrayrawfilesynchronizerfactorytest PRIVATE -DTRANSLATION_DOMAIN=\"liboktetakasten\")
