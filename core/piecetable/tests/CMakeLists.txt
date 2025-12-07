set( SUTDIR ${CMAKE_CURRENT_SOURCE_DIR}/../.. )

set(ECM_TEST_NAME_PREFIX_STRATEGY "VARIABLE")
set(ECM_TEST_NAME_PREFIX "libpiecetable-")

include_directories(
    ${SUTDIR}
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


########### piecetest ###############

ecm_add_test( piecetest.cpp
    LINK_LIBRARIES Qt6::Test
)
target_include_directories(piecetest
    PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/../..
)

########### piecetabletest ###############

ecm_add_test( piecetabletest.cpp
    LINK_LIBRARIES
        OktetaCoreTestPieceTableChange
        OktetaCorePieceTable
        Qt6::Test
)

########### testpiecetablechangetest ###############

ecm_add_test( testpiecetablechangetest.cpp
    LINK_LIBRARIES
        OktetaCoreTestPieceTableChange
        Qt6::Test
)

########### testpiecetablechangeabstractpiecetablechangeiftest ###############

ecm_add_test( testpiecetablechangeabstractpiecetablechangeiftest.cpp
    LINK_LIBRARIES
        OktetaCoreTestPieceTableChange
        OktetaCorePieceTable
        Qt6::Test
)

########### insertpiecetablechangeabstractpiecetablechangeiftest ###############

ecm_add_test( insertpiecetablechangeabstractpiecetablechangeiftest.cpp
    LINK_LIBRARIES
        OktetaCoreTestPieceTableChange
        OktetaCorePieceTable
        Qt6::Test
)

########### removepiecetablechangeabstractpiecetablechangeiftest ###############

ecm_add_test( removepiecetablechangeabstractpiecetablechangeiftest.cpp
    LINK_LIBRARIES
        OktetaCoreTestPieceTableChange
        OktetaCorePieceTable
        Qt6::Test
)

########### replacepiecetablechangeabstractpiecetablechangeiftest ###############

ecm_add_test( replacepiecetablechangeabstractpiecetablechangeiftest.cpp
    LINK_LIBRARIES
        OktetaCoreTestPieceTableChange
        OktetaCorePieceTable
        Qt6::Test
)

########### swaprangespiecetablechangeabstractpiecetablechangeiftest ###############

ecm_add_test( swaprangespiecetablechangeabstractpiecetablechangeiftest.cpp
    LINK_LIBRARIES
        OktetaCoreTestPieceTableChange
        OktetaCorePieceTable
        Qt6::Test
)

########### grouppiecetablechangeabstractpiecetablechangeiftest ###############

# grouped changes break some assumptions and got workarounds, so disable for now
if(IWILLPICKUPWORKONOKTETACORE)
ecm_add_test( grouppiecetablechangeabstractpiecetablechangeiftest.cpp
    LINK_LIBRARIES
        OktetaCoreTestPieceTableChange
        OktetaCorePieceTable
        Qt6::Test
)
endif()

########### grouppiecetablechangetest ###############

ecm_add_test( grouppiecetablechangetest.cpp
    LINK_LIBRARIES
        OktetaCoreTestPieceTableChange
        OktetaCorePieceTable
        Qt6::Test
)

########### piecetablechangehistorytest ###############

ecm_add_test( piecetablechangehistorytest.cpp
    LINK_LIBRARIES
        OktetaCoreTestPieceTableChange
        OktetaCorePieceTable
        Qt6::Test
)

########### revertablepiecetabletest ###############

ecm_add_test( revertablepiecetabletest.cpp
    LINK_LIBRARIES
        OktetaCorePieceTable
        Qt6::Test
)
