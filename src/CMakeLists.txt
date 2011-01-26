add_library(utilmm SHARED
    configfile/commandline.cc
    configfile/configfile.cc
    configfile/configset.cc
    configfile/pkgconfig.cc
    configfile/shell_expand.cc
    configsearch/configuration_finder.cc
    demangle/demangle.cc
    memory/dynamic_pool.cc
    singleton/dummy.cc
    singleton/server.cc
    system/process.cc
    system/socket.cc
    system/system.cc)

set_target_properties(utilmm PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${API_VERSION}
    INSTALL_NAME_DIR ${CMAKE_INSTALL_PREFIX}/lib)

target_link_libraries(utilmm ${Boost_REGEX_LIBRARY})
target_link_libraries(utilmm ${Boost_THREAD_LIBRARY})
target_link_libraries(utilmm ${Boost_FILESYSTEM_LIBRARY})

install(TARGETS utilmm
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)
