########### util ###############

add_library(KastenCoreTestUtil STATIC)
target_sources(KastenCoreTestUtil
    PRIVATE
        util/filesystem.cpp
)
target_link_libraries(KastenCoreTestUtil
    PUBLIC
        Qt5::Core
    PRIVATE
        Qt5::Test
)

########### io ###############

add_library(KastenCoreTestIo STATIC)
target_link_libraries(KastenCoreTestIo
    PUBLIC
        KastenCore
    PRIVATE
        KF5::CoreAddons
)
target_sources(KastenCoreTestIo
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
        KastenCoreTestIo
        KastenCoreTestUtil
        KastenCore
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
        KastenCoreTestIo
        KastenCoreTestUtil
        KastenCore
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
