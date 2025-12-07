set(SUTDIR ${CMAKE_CURRENT_SOURCE_DIR}/..)

ecm_add_tests(
    primitivearraytest.cpp
    arraydatainformationtest.cpp
    basicdatainformationtest.cpp
    scriptclassestest.cpp
    uniondatainformationtest.cpp
    allprimitivetypestest.cpp
    scriptvalueconvertertest.cpp
    commonparsertest.cpp
    jsparsertest.cpp
    customtostringtest.cpp
    locktooffsettest.cpp

    LINK_LIBRARIES
        OktetaKastenControllersStructuresDataTypes
        Qt6::Test
)

ecm_add_test(
    primitivedatainformationtest.cpp
    LINK_LIBRARIES
        OktetaKastenControllersStructuresDataTypes
        KF6::I18n
        Qt6::Test
)
target_compile_definitions(primitivedatainformationtest PRIVATE -DTRANSLATION_DOMAIN=\"liboktetakasten\")

ecm_add_test(
    ${SUTDIR}/parsers/abstractstructureparser.cpp
    ${SUTDIR}/parsers/osdparser.cpp
    osdparsertest.cpp

    TEST_NAME osdparsertest
    LINK_LIBRARIES
        OktetaKastenControllersStructuresDataTypes
        KF6::I18n
        Qt6::Xml
        Qt6::Test
)
target_compile_definitions(osdparsertest PRIVATE -DTRANSLATION_DOMAIN=\"liboktetakasten\")

ecm_add_test(
    ${SUTDIR}/parsers/abstractstructureparser.cpp
    ${SUTDIR}/parsers/osdparser.cpp
    ${SUTDIR}/parsers/scriptfileparser.cpp
    ${SUTDIR}/structureinstalljob.cpp
    ${SUTDIR}/structureuninstalljob.cpp
    ${SUTDIR}/structuremetadata.cpp
    ${SUTDIR}/structuredefinitionfile.cpp
    ${SUTDIR}/structuresmanager.cpp
    structuresmanagertest.cpp

    TEST_NAME structuresmanagertest
    LINK_LIBRARIES
        OktetaKastenControllersStructuresDataTypes
        KF6::Archive
        KF6::KIOCore
        KF6::I18n
        Qt6::Xml
        Qt6::Test
)
target_compile_definitions(structuresmanagertest PRIVATE -DTRANSLATION_DOMAIN=\"liboktetakasten\")

# remove useless default definition of QT_TESTCASE_BUILDDIR in interface from imported target Qt6::Test
get_target_property(qttestdefs Qt6::Test INTERFACE_COMPILE_DEFINITIONS)
list(FILTER qttestdefs EXCLUDE REGEX [[^QT_TESTCASE_BUILDDIR=]])
set_property(TARGET Qt6::Test PROPERTY INTERFACE_COMPILE_DEFINITIONS ${qttestdefs})
# set the definition as needed for our tests
target_compile_definitions(customtostringtest PRIVATE QT_TESTCASE_BUILDDIR="${CMAKE_CURRENT_BINARY_DIR}")
target_compile_definitions(jsparsertest       PRIVATE QT_TESTCASE_BUILDDIR="${CMAKE_CURRENT_BINARY_DIR}")
# TODO: still fragile once __FILE__ is neither absolute or full relative path to this build dir
# look into using custom code instead of simple QFINDTESTDATA usage
