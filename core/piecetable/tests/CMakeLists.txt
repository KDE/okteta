set( SUTDIR ${CMAKE_CURRENT_SOURCE_DIR}/../.. )

include_directories(
    ${SUTDIR}
)

add_library(testpiecetablechangelib STATIC
    abstractpiecetablechangeiftest.cpp
    testpiecetablechange.cpp
)
target_link_libraries( testpiecetablechangelib
    PUBLIC
        testoktetacore
        Qt6::Test
)


########### piecetest ###############

ecm_add_test( piecetest.cpp
    NAME_PREFIX "libpiecetable-"
    LINK_LIBRARIES Qt6::Test
)
target_include_directories(piecetest
    PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/../..
)

########### piecetabletest ###############

ecm_add_test( piecetabletest.cpp
    NAME_PREFIX "libpiecetable-"
    LINK_LIBRARIES testoktetacore testpiecetablechangelib Qt6::Test
)

########### testpiecetablechangetest ###############

ecm_add_test( testpiecetablechangetest.cpp
    NAME_PREFIX "libpiecetable-"
    LINK_LIBRARIES oktetacoretestutil testpiecetablechangelib Qt6::Test
)

########### testpiecetablechangeabstractpiecetablechangeiftest ###############

ecm_add_test( testpiecetablechangeabstractpiecetablechangeiftest.cpp
    NAME_PREFIX "libpiecetable-"
    LINK_LIBRARIES oktetacoretestutil testoktetacore testpiecetablechangelib Qt6::Test
)

########### insertpiecetablechangeabstractpiecetablechangeiftest ###############

ecm_add_test( insertpiecetablechangeabstractpiecetablechangeiftest.cpp
    NAME_PREFIX "libpiecetable-"
    LINK_LIBRARIES oktetacoretestutil testoktetacore testpiecetablechangelib Qt6::Test
)

########### removepiecetablechangeabstractpiecetablechangeiftest ###############

ecm_add_test( removepiecetablechangeabstractpiecetablechangeiftest.cpp
    NAME_PREFIX "libpiecetable-"
    LINK_LIBRARIES oktetacoretestutil testoktetacore testpiecetablechangelib Qt6::Test
)

########### replacepiecetablechangeabstractpiecetablechangeiftest ###############

ecm_add_test( replacepiecetablechangeabstractpiecetablechangeiftest.cpp
    NAME_PREFIX "libpiecetable-"
    LINK_LIBRARIES oktetacoretestutil testoktetacore testpiecetablechangelib Qt6::Test
)

########### swaprangespiecetablechangeabstractpiecetablechangeiftest ###############

ecm_add_test( swaprangespiecetablechangeabstractpiecetablechangeiftest.cpp
    NAME_PREFIX "libpiecetable-"
    LINK_LIBRARIES oktetacoretestutil testoktetacore testpiecetablechangelib Qt6::Test
)

########### grouppiecetablechangeabstractpiecetablechangeiftest ###############

# grouped changes break some assumptions and got workarounds, so disable for now
if(IWILLPICKUPWORKONOKTETACORE)
ecm_add_test( grouppiecetablechangeabstractpiecetablechangeiftest.cpp
    NAME_PREFIX "libpiecetable-"
    LINK_LIBRARIES oktetacoretestutil testoktetacore testpiecetablechangelib Qt6::Test
)
endif()

########### grouppiecetablechangetest ###############

ecm_add_test( grouppiecetablechangetest.cpp
    NAME_PREFIX "libpiecetable-"
    LINK_LIBRARIES testoktetacore testpiecetablechangelib Qt6::Test
)

########### piecetablechangehistorytest ###############

ecm_add_test( piecetablechangehistorytest.cpp
    NAME_PREFIX "libpiecetable-"
    LINK_LIBRARIES testoktetacore testpiecetablechangelib Qt6::Test
)

########### revertablepiecetabletest ###############

ecm_add_test( revertablepiecetabletest.cpp
    NAME_PREFIX "libpiecetable-"
    LINK_LIBRARIES testoktetacore Qt6::Test
)
