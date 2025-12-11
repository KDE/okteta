if( BUILD_OKTETAKASTENLIBS )
    if (BUILD_TESTING)
        ecm_test_set_dir_properties(PREFIX_NAME_IGNORE)
    endif()

    add_subdirectory( kasten )
    add_subdirectory( framesprint )
endif()
