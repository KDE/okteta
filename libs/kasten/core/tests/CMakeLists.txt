set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )

set( SUTDIR ${CMAKE_CURRENT_SOURCE_DIR}/.. )

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}/..
    ${SUTDIR}/..
    ${SUTDIR}/document
    ${SUTDIR}/io
    ${SUTDIR}/io/filesystem
    ${SUTDIR}
)

########### util ###############

add_library(kastencoretestutil STATIC)
target_link_libraries(kastencoretestutil PRIVATE Qt5::Test Qt5::Core)
target_sources(kastencoretestutil
    PRIVATE
        util/filesystem.cpp
)

########### io ###############

add_library(kastencoretestio STATIC)
target_link_libraries(kastencoretestio
    PUBLIC
        Qt5::Core
    PRIVATE
        KF5::I18n
        KF5::CoreAddons
)
target_sources(kastencoretestio
    PRIVATE
        testdocumentfileloadthread.cpp
        testdocumentfilereloadthread.cpp
        testdocumentfilewritethread.cpp
        testdocumentfileloadjob.cpp
        testdocumentfileconnectjob.cpp
        testdocumentfilewritejob.cpp
        testdocumentfilereloadjob.cpp
        testdocumentfilewritetojob.cpp
)

########### testdocumenttest ###############

ecm_add_test(
    testdocument.cpp
    testdocumenttest.cpp
    TEST_NAME testdocumenttest
    NAME_PREFIX libkasten-core-
    LINK_LIBRARIES
        KastenCore
        Qt5::Test
)

########### testdocumentfilesynchronizertest ###############

ecm_add_test(
    testdocument.cpp
    testdocumentfilesynchronizer.cpp
    testdocumentfilesynchronizertest.cpp
    TEST_NAME testdocumentfilesynchronizertest
    NAME_PREFIX libkasten-core-
    LINK_LIBRARIES
        kastencoretestio
        KastenCore
        kastencoretestutil
        Qt5::Test
)

############ testdocumentfilesynchronizerfactorytest ###############

ecm_add_test(
    testdocument.cpp
    testdocumentfilesynchronizer.cpp
    testdocumentfilesynchronizerfactory.cpp
    testdocumentfilesynchronizerfactorytest.cpp
    TEST_NAME testdocumentfilesynchronizerfactorytest
    NAME_PREFIX libkasten-core-
    LINK_LIBRARIES
        kastencoretestio
        KastenCore
        kastencoretestutil
        Qt5::Test
)

########### documentmanagertest ###############

ecm_add_test(
    testdocument.cpp
    documentmanagertest.cpp
    TEST_NAME documentmanagertest
    NAME_PREFIX libkasten-core-
    LINK_LIBRARIES
        KastenCore
        Qt5::Test
)
