set(SOURCES_UNIX_ONLY
    configfile/pkgconfig.cc
    system/process.cc
    system/socket.cc
    system/system.cc)

set(SOURCES
    configfile/commandline.cc
    configfile/configfile.cc
    configfile/configset.cc
    configfile/shell_expand.cc
    configsearch/configuration_finder.cc
    demangle/demangle.cc
    memory/dynamic_pool.cc
    singleton/dummy.cc
    singleton/server.cc)

if(NOT WIN32)
    set(SOURCES ${SOURCES} ${SOURCES_UNIX_ONLY})
endif(NOT WIN32)

add_library(utilmm SHARED ${SOURCES})

set_target_properties(utilmm PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${API_VERSION}
    INSTALL_NAME_DIR ${CMAKE_INSTALL_PREFIX}/lib)

target_link_libraries(utilmm ${Boost_REGEX_LIBRARY})
target_link_libraries(utilmm ${Boost_THREAD_LIBRARY})
target_link_libraries(utilmm ${Boost_FILESYSTEM_LIBRARY})
if(APPLE OR WIN32)
    target_link_libraries(utilmm ${Boost_SYSTEM_LIBRARY})
endif(APPLE OR WIN32)

install(TARGETS utilmm
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)
