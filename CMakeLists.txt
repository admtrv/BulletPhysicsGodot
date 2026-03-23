# CMakeLists.txt

cmake_minimum_required(VERSION 3.22)
project(BulletPhysicsGodot)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# dependencies
add_subdirectory(godot-cpp)

set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
add_subdirectory(BulletPhysics)

# extension sources
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_library(bulletphysics_godot SHARED ${SOURCES})
target_include_directories(bulletphysics_godot PRIVATE src/)
target_link_libraries(bulletphysics_godot PRIVATE godot::cpp BulletPhysics)

# build type suffix for GDExtension naming convention
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(GODOT_BUILD_TYPE "template_release")
else()
    set(GODOT_BUILD_TYPE "template_debug")
endif()

# output to project/bin/
set_target_properties(bulletphysics_godot PROPERTIES
    OUTPUT_NAME "libbulletphysics.linux.${GODOT_BUILD_TYPE}.x86_64"
    PREFIX ""
    SUFFIX ".so"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/project/bin"
)
