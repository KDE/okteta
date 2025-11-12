if( BUILD_KPARTSPLUGIN )
    ecm_test_set_dir_properties(PREFIX_NAME_IGNORE)

    add_subdirectory( kpart )
endif()
