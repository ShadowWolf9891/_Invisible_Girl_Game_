cmake_minimum_required(VERSION 3.10.2) # version on Ubuntu Bionic
project(behaviortree_cpp)

#---- Add the subdirectory cmake ----
set(CMAKE_CONFIG_PATH ${CMAKE_MODULE_PATH}  "${CMAKE_CURRENT_LIST_DIR}/cmake")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CONFIG_PATH}")

#---- Enable C++17 ----
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS -DWIN32_LEAN_AND_MEAN)
else()
    add_definitions(-Wpedantic)
endif()

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

#---- project configuration ----
option(BUILD_EXAMPLES   "Build tutorials and examples" ON)
option(BUILD_SAMPLES    "Build sample nodes" ON)
option(BUILD_UNIT_TESTS "Build the unit tests" ON)
option(BUILD_TOOLS "Build commandline tools" ON)
option(BUILD_SHARED_LIBS "Build shared libraries" ON)
option(BUILD_MANUAL_SELECTOR "Build manual selector node" ON)
option(ENABLE_COROUTINES "Enable boost coroutines" ON)
option(USE_V3_COMPATIBLE_NAMES  "Use some alias to compile more easily old 3.x code" OFF)

#---- Include boost to add coroutines ----
if(ENABLE_COROUTINES)
    find_package(Boost COMPONENTS coroutine QUIET)

    if(Boost_FOUND)
        string(REPLACE "." "0" Boost_VERSION_NODOT ${Boost_VERSION})
        if(NOT Boost_VERSION_NODOT VERSION_LESS 105900)
            message(STATUS "Found boost::coroutine2.")
            add_definitions(-DBT_BOOST_COROUTINE2)
            set(BT_COROUTINES true)
        elseif(NOT Boost_VERSION_NODOT VERSION_LESS 105300)
            message(STATUS "Found boost::coroutine.")
            add_definitions(-DBT_BOOST_COROUTINE)
            set(BT_COROUTINES true)
        endif()
        include_directories(${Boost_INCLUDE_DIRS})
    endif()

    if(NOT DEFINED BT_COROUTINES)
        message(STATUS "Boost coroutines disabled. Install Boost (version 1.59+ recommended).")
    endif()
else()
    message(STATUS "Boost coroutines disabled by CMake option.")
endif()

if(NOT DEFINED BT_COROUTINES)
    add_definitions(-DBT_NO_COROUTINES)
endif()

if(USE_V3_COMPATIBLE_NAMES)
    add_definitions(-DUSE_BTCPP3_OLD_NAMES)
endif()

#---- Find other packages ----
find_package(Threads)
find_package(ZMQ)

list(APPEND BEHAVIOR_TREE_PUBLIC_LIBRARIES
    ${CMAKE_THREAD_LIBS_INIT}
    ${CMAKE_DL_LIBS}
)

if( ZMQ_FOUND )
    message(STATUS "ZeroMQ found.")
    add_definitions( -DZMQ_FOUND )
    list(APPEND BT_SOURCE src/loggers/bt_zmq_publisher.cpp)
else()
    message(WARNING "ZeroMQ NOT found. Skipping the build of [PublisherZMQ] and [bt_recorder].")
endif()

set(BEHAVIOR_TREE_LIBRARY ${PROJECT_NAME})

# Update the policy setting to avoid an error when loading the ament_cmake package
# at the current cmake version level
if(POLICY CMP0057)
    cmake_policy(SET CMP0057 NEW)
endif()

find_package(ament_cmake QUIET)

if ( ament_cmake_FOUND )

    add_definitions( -DUSING_ROS2 )
    message(STATUS "------------------------------------------")
    message(STATUS "BehaviourTree is being built using AMENT.")
    message(STATUS "------------------------------------------")

    set(BUILD_TOOL_INCLUDE_DIRS ${ament_INCLUDE_DIRS})

elseif( CATKIN_DEVEL_PREFIX OR CATKIN_BUILD_BINARY_PACKAGE)

    set(catkin_FOUND 1)
    add_definitions( -DUSING_ROS )
    find_package(catkin REQUIRED COMPONENTS roslib)
    find_package(GTest)

    message(STATUS "------------------------------------------")
    message(STATUS "BehaviourTree is being built using CATKIN.")
    message(STATUS "------------------------------------------")

    catkin_package(
        INCLUDE_DIRS include # do not include "3rdparty" here
        LIBRARIES ${BEHAVIOR_TREE_LIBRARY}
        CATKIN_DEPENDS roslib
        )

    list(APPEND BEHAVIOR_TREE_PUBLIC_LIBRARIES ${catkin_LIBRARIES})
    set(BUILD_TOOL_INCLUDE_DIRS ${catkin_INCLUDE_DIRS})

elseif(BUILD_UNIT_TESTS)
    if(${CMAKE_VERSION} VERSION_LESS "3.11.0")
        find_package(GTest REQUIRED)
    else()
        include(FetchContent)
        FetchContent_Declare(
            googletest
            URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
            )
        # For Windows: Prevent overriding the parent project's compiler/linker settings
        set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
        FetchContent_MakeAvailable(googletest)
    endif()
endif()


#############################################################
# LIBRARY

add_subdirectory(3rdparty/lexy)

list(APPEND BT_SOURCE
    src/action_node.cpp
    src/basic_types.cpp
    src/behavior_tree.cpp
    src/blackboard.cpp
    src/bt_factory.cpp
    src/decorator_node.cpp
    src/condition_node.cpp
    src/control_node.cpp
    src/shared_library.cpp
    src/tree_node.cpp
    src/script_parser.cpp
    src/xml_parsing.cpp

    src/decorators/inverter_node.cpp
    src/decorators/repeat_node.cpp
    src/decorators/retry_node.cpp
    src/decorators/subtree_node.cpp
    src/decorators/delay_node.cpp

    src/controls/if_then_else_node.cpp
    src/controls/fallback_node.cpp
    src/controls/parallel_node.cpp
    src/controls/reactive_sequence.cpp
    src/controls/reactive_fallback.cpp
    src/controls/sequence_node.cpp
    src/controls/sequence_star_node.cpp
    src/controls/switch_node.cpp
    src/controls/while_do_else_node.cpp

    src/loggers/bt_cout_logger.cpp
    src/loggers/bt_file_logger.cpp
    src/loggers/bt_minitrace_logger.cpp

    3rdparty/tinyxml2/tinyxml2.cpp
    3rdparty/minitrace/minitrace.cpp
    )

if(BUILD_MANUAL_SELECTOR)
    find_package(Curses QUIET)
    if(CURSES_FOUND)
        list(APPEND BT_SOURCE
            src/controls/manual_node.cpp
            )
        list(APPEND BEHAVIOR_TREE_PUBLIC_LIBRARIES ${CURSES_LIBRARIES})
        add_definitions(-DNCURSES_FOUND)
    else()
        message(WARNING "NCurses NOT found. Skipping the build of manual selector node.")
    endif()
endif()


######################################################

if (UNIX)
    list(APPEND BT_SOURCE src/shared_library_UNIX.cpp )
endif()

if (WIN32)
    set(CMAKE_DEBUG_POSTFIX "d")
    list(APPEND BT_SOURCE src/shared_library_WIN.cpp )
endif()

if (BUILD_SHARED_LIBS)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    add_library(${BEHAVIOR_TREE_LIBRARY} SHARED ${BT_SOURCE})
else()
    add_library(${BEHAVIOR_TREE_LIBRARY} STATIC ${BT_SOURCE})
endif()

if( ZMQ_FOUND )
    list(APPEND BUILD_TOOL_INCLUDE_DIRS ${ZMQ_INCLUDE_DIRS})
endif()

target_link_libraries(${BEHAVIOR_TREE_LIBRARY} PUBLIC
    ${BEHAVIOR_TREE_PUBLIC_LIBRARIES})

target_link_libraries(${BEHAVIOR_TREE_LIBRARY} PRIVATE
    ${Boost_LIBRARIES}
    ${ZMQ_LIBRARIES}
    foonathan::lexy
)

#get_target_property(my_libs ${BEHAVIOR_TREE_LIBRARY} INTERFACE_LINK_LIBRARIES)
#list(REMOVE_ITEM _libs X)
#message("my_libs: ${my_libs}")

#set_target_properties(${BEHAVIOR_TREE_LIBRARY} PROPERTIES INTERFACE_LINK_LIBRARIES "")

target_compile_definitions(${BEHAVIOR_TREE_LIBRARY} PRIVATE $<$<CONFIG:Debug>:TINYXML2_DEBUG>)

target_include_directories(${BEHAVIOR_TREE_LIBRARY} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/3rdparty>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/lexy/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    ${BUILD_TOOL_INCLUDE_DIRS})

if( ZMQ_FOUND )
    target_compile_definitions(${BEHAVIOR_TREE_LIBRARY} PUBLIC ZMQ_FOUND)
endif()

if(MSVC)
else()
    target_compile_options(${BEHAVIOR_TREE_LIBRARY} PRIVATE
        -Wall -Wextra -Werror=return-type)
endif()

#############################################################
if(ament_cmake_FOUND)
    find_package(ament_index_cpp REQUIRED)
    ament_target_dependencies(${BEHAVIOR_TREE_LIBRARY} PUBLIC ament_index_cpp)
    ament_export_dependencies(ament_index_cpp)

    set( BEHAVIOR_TREE_LIB_DESTINATION   lib )
    set( BEHAVIOR_TREE_INC_DESTINATION   include )
    set( BEHAVIOR_TREE_BIN_DESTINATION   bin )

    ament_export_include_directories(include)
    ament_export_libraries(${BEHAVIOR_TREE_LIBRARY})
    ament_package()
elseif(catkin_FOUND)
    set( BEHAVIOR_TREE_LIB_DESTINATION   ${CATKIN_PACKAGE_LIB_DESTINATION} )
    set( BEHAVIOR_TREE_INC_DESTINATION   ${CATKIN_GLOBAL_INCLUDE_DESTINATION} )
    set( BEHAVIOR_TREE_BIN_DESTINATION   ${CATKIN_GLOBAL_BIN_DESTINATION} )
else()
    set( BEHAVIOR_TREE_LIB_DESTINATION   lib )
    set( BEHAVIOR_TREE_INC_DESTINATION   include )
    set( BEHAVIOR_TREE_BIN_DESTINATION   bin )

endif()

message( STATUS "BEHAVIOR_TREE_LIB_DESTINATION:   ${BEHAVIOR_TREE_LIB_DESTINATION} " )
message( STATUS "BEHAVIOR_TREE_BIN_DESTINATION:   ${BEHAVIOR_TREE_BIN_DESTINATION} " )
message( STATUS "BUILD_UNIT_TESTS:   ${BUILD_UNIT_TESTS} " )


######################################################
# Samples
if (BUILD_SAMPLES)
    add_subdirectory(sample_nodes)
endif()

######################################################
# Test
if (BUILD_UNIT_TESTS AND BUILD_SAMPLES)
    add_subdirectory(tests)
endif()

######################################################
# INSTALL

INSTALL(TARGETS ${BEHAVIOR_TREE_LIBRARY}
    EXPORT ${PROJECT_NAME}Targets
    ARCHIVE DESTINATION ${BEHAVIOR_TREE_LIB_DESTINATION}
    LIBRARY DESTINATION ${BEHAVIOR_TREE_LIB_DESTINATION}
    RUNTIME DESTINATION ${BEHAVIOR_TREE_BIN_DESTINATION}
    )

INSTALL( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION ${BEHAVIOR_TREE_INC_DESTINATION}
    FILES_MATCHING PATTERN "*.h*")

install(EXPORT ${PROJECT_NAME}Targets
    FILE "${PROJECT_NAME}Targets.cmake"
    DESTINATION "${BEHAVIOR_TREE_LIB_DESTINATION}/cmake/${PROJECT_NAME}"
    NAMESPACE BT::
    )

export(PACKAGE ${PROJECT_NAME})

include(CMakePackageConfigHelpers)

configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Config.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  INSTALL_DESTINATION "${BEHAVIOR_TREE_LIB_DESTINATION}/cmake/${PROJECT_NAME}"
)

# This requires to declare to project version in the project() macro

#write_basic_package_version_file(
#  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
#  VERSION ${PROJECT_VERSION}
#  COMPATIBILITY AnyNewerVersion
#)

install(
  FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    #    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    DESTINATION "${BEHAVIOR_TREE_LIB_DESTINATION}/cmake/${PROJECT_NAME}"
)

######################################################
# EXAMPLES and TOOLS
if(BUILD_TOOLS)
    add_subdirectory(tools)
endif()

if(BUILD_EXAMPLES AND BUILD_SAMPLES)
    add_subdirectory(examples)
endif()
