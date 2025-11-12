if( BUILD_OKTETAKASTENLIBS )
    ecm_test_set_dir_properties(PREFIX_NAME_IGNORE)

    add_subdirectory( kasten )
    add_subdirectory( framesprint )
endif()
