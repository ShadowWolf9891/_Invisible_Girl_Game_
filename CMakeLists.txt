cmake_minimum_required(VERSION 3.22)

project(Invisible_Girl_Game)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(External_Libraries)

file(GLOB SOURCES
	"*.h"
	"*.hpp"
	"*.cpp"
	"Assets/*/*.png"
	"Assets/*/*/*.png"
	"level_data/*.json"
	"data/*/*.xml"
)

set(SFML_STATIC_LIBRARIES TRUE)

set(SFML_DIR ${PROJECT_SOURCE_DIR}/External_Libraries/SFML-2.5.1/lib/cmake/SFML)
set(behaviortree_cpp_DIR ${PROJECT_SOURCE_DIR}/External_Libraries/BehaviorTree/lib/cmake/behaviortree_cpp)

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
find_package(behaviortree_cpp REQUIRED)

add_executable(Invisible_Girl ${SOURCES})

target_link_libraries(Invisible_Girl sfml-graphics sfml-system sfml-window sfml-audio sfml-network)
target_link_libraries(Invisible_Girl BT::behaviortree_cpp)
target_link_libraries(Invisible_Girl tinyxml2)


# Install target
install(TARGETS Invisible_Girl DESTINATION bin)

target_include_directories(Invisible_Girl PUBLIC 
						"${PROJECT_SOURCE_DIR}"
						"${PROJECT_BINARY_DIR}"
						"${PROJECT_SOURCE_DIR}/External_Libraries/tileson-master/include"
						"${PROJECT_SOURCE_DIR}/External_Libraries/tinyxml2-master"
						"${PROJECT_SOURCE_DIR}/External_Libraries/BehaviorTree/include"
						"${PROJECT_SOURCE_DIR}/External_Libraries/SFML-2.5.1/include"
						"${PROJECT_SOURCE_DIR}/header-folder"
						"${PROJECT_SOURCE_DIR}/source-folder"
						)

if (WIN32)
# copy the .dll file to the same folder as the executable
add_custom_command(
    TARGET Invisible_Girl POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    $<TARGET_FILE_DIR:BT::behaviortree_cpp>
    $<TARGET_FILE_DIR:Invisible_Girl>)
endif()