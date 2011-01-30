cmake_minimum_required( VERSION 2.6 )

set( App_name smart_ptr )
set( App_bin smart_ptr )

project( ${App_name} )

set( Boost_USE_STATIC_LIBS ON )
set( Boost_USE_MULTITHREADED ON )

find_package( Threads )
find_package( Boost COMPONENTS
    system
    thread
    REQUIRED
)

# --------------------------------------------------------------------
set( VERSION "0.0.0" )

set( USED_LIBS 
    gtest 
)

# --------------------------------------------------------------------
if( "${CMAKE_BUILD_TYPE}" STREQUAL "debug" )
    message( STATUS "debug build" )
    set( CMAKE_VERBOSE_MAKEFILE ON )
elseif( "${CMAKE_BUILD_TYPE}" STREQUAL "release" )
    message( STATUS "release build" )
    set( CMAKE_VERBOSE_MAKEFILE OFF )
else()
    message( FATAL_ERROR "bad build type -- ( debug, release )" )
endif()

add_definitions( -DVERSION="${VERSION}" )

# --------------------------------------------------------------------
include_directories(
    ${Boost_INCLUDE_DIR}
)

# --------------------------------------------------------------------
file( GLOB_RECURSE INC_FILES ${CMAKE_SOURCE_DIR}/src/*.h )
file( GLOB_RECURSE SRC_FILES ${CMAKE_SOURCE_DIR}/src/*.cpp )

# --------------------------------------------------------------------
# build
add_executable( ${App_bin}
    ${INC_FILES}
    ${SRC_FILES}
)

target_link_libraries( ${App_bin}
    ${USED_LIBS}
    ${CMAKE_THREAD_LIBS_INIT}
    ${Boost_LIBRARIES}
)

# --------------------------------------------------------------------
# install
install( FILES
    ${CMAKE_BINARY_DIR}/${App_bin}
    ${CMAKE_SOURCE_DIR}/data/common.conf
    ${CMAKE_SOURCE_DIR}/data/user.conf
    DESTINATION ${CMAKE_SOURCE_DIR}/bin
)

