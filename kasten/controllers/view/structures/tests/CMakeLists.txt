ecm_add_tests(
    primitivearraytest.cpp
    arraydatainformationtest.cpp
    basicdatainformationtest.cpp
    primitivedatainformationtest.cpp
    scriptclassestest.cpp
    uniondatainformationtest.cpp
    allprimitivetypestest.cpp
    scriptvalueconvertertest.cpp
    osdparsertest.cpp
    commonparsertest.cpp
    jsparsertest.cpp
    customtostringtest.cpp
    locktooffsettest.cpp

    NAME_PREFIX structures-
    LINK_LIBRARIES
        structureview-tested-srcs
        poddecoder-typeeditors
        Qt5::Test
)

# remove useless default definition of QT_TESTCASE_BUILDDIR in interface from imported target Qt5::Test
get_target_property(qttestdefs Qt5::Test INTERFACE_COMPILE_DEFINITIONS)
list(FILTER qttestdefs EXCLUDE REGEX [[^QT_TESTCASE_BUILDDIR=]])
set_property(TARGET Qt5::Test PROPERTY INTERFACE_COMPILE_DEFINITIONS ${qttestdefs})
# set the definition as needed for our tests
target_compile_definitions(customtostringtest PRIVATE QT_TESTCASE_BUILDDIR="${CMAKE_CURRENT_BINARY_DIR}")
target_compile_definitions(jsparsertest       PRIVATE QT_TESTCASE_BUILDDIR="${CMAKE_CURRENT_BINARY_DIR}")
# TODO: still fragile once __FILE__ is neither absolute or full relative path to this build dir
# look into using custom code intead of simple QFINDTESTDATA usage
