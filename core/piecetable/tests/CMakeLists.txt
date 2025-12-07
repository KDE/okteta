set(SUTDIR ${CMAKE_CURRENT_SOURCE_DIR}/..)

set(ECM_TEST_NAME_PREFIX_STRATEGY "VARIABLE")
set(ECM_TEST_NAME_PREFIX "libpiecetable-")

# to see Okteta Core headers for Address, Size, etc. types
# also for tests to enable more namespaced <piecetable/*> includes
include_directories(
    ${SUTDIR}/..
)

add_library(OktetaCoreTestPieceTableChange STATIC
    abstractpiecetablechangeiftest.cpp
    testpiecetablechange.cpp
)
target_link_libraries(OktetaCoreTestPieceTableChange
    PUBLIC
        OktetaCorePieceTable
        Qt6::Test
)

ecm_add_test( piecetest.cpp
    LINK_LIBRARIES Qt6::Test
)

ecm_add_test(
    piecelisttest.cpp
    ${SUTDIR}/piecelist.cpp
    TEST_NAME piecelisttest
    LINK_LIBRARIES
        Qt6::Test
)

ecm_add_test( piecetabletest.cpp
    LINK_LIBRARIES
        OktetaCoreTestPieceTableChange
        OktetaCorePieceTable
        Qt6::Test
)

ecm_add_test( testpiecetablechangetest.cpp
    LINK_LIBRARIES
        OktetaCoreTestPieceTableChange
        Qt6::Test
)

ecm_add_test( testpiecetablechangeabstractpiecetablechangeiftest.cpp
    LINK_LIBRARIES
        OktetaCoreTestPieceTableChange
        OktetaCorePieceTable
        Qt6::Test
)

ecm_add_test( insertpiecetablechangeabstractpiecetablechangeiftest.cpp
    LINK_LIBRARIES
        OktetaCoreTestPieceTableChange
        OktetaCorePieceTable
        Qt6::Test
)

ecm_add_test( removepiecetablechangeabstractpiecetablechangeiftest.cpp
    LINK_LIBRARIES
        OktetaCoreTestPieceTableChange
        OktetaCorePieceTable
        Qt6::Test
)

ecm_add_test( replacepiecetablechangeabstractpiecetablechangeiftest.cpp
    LINK_LIBRARIES
        OktetaCoreTestPieceTableChange
        OktetaCorePieceTable
        Qt6::Test
)

ecm_add_test( swaprangespiecetablechangeabstractpiecetablechangeiftest.cpp
    LINK_LIBRARIES
        OktetaCoreTestPieceTableChange
        OktetaCorePieceTable
        Qt6::Test
)

# grouped changes break some assumptions and got workarounds, so disable for now
if(IWILLPICKUPWORKONOKTETACORE)
ecm_add_test( grouppiecetablechangeabstractpiecetablechangeiftest.cpp
    LINK_LIBRARIES
        OktetaCoreTestPieceTableChange
        OktetaCorePieceTable
        Qt6::Test
)
endif()

ecm_add_test( grouppiecetablechangetest.cpp
    LINK_LIBRARIES
        OktetaCoreTestPieceTableChange
        OktetaCorePieceTable
        Qt6::Test
)

ecm_add_test( piecetablechangehistorytest.cpp
    LINK_LIBRARIES
        OktetaCoreTestPieceTableChange
        OktetaCorePieceTable
        Qt6::Test
)

ecm_add_test( revertablepiecetabletest.cpp
    LINK_LIBRARIES
        OktetaCorePieceTable
        Qt6::Test
)
