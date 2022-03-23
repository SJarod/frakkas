cmake_minimum_required(VERSION 3.9...3.21)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

set(CMAKE_CXX_STANDARD 20 CACHE STRING "Language standard" FORCE)
set(CMAKE_CXX_STANDARD_REQUIRED ON CACHE BOOL "Use the C++ standard" FORCE)
set(CMAKE_CXX_EXTENSIONS OFF CACHE BOOL "" FORCE)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

project(EngineFrakkas
        VERSION 1.0
        DESCRIPTION "Engine"
)

include(FetchContent)
FetchContent_Declare(sdl
                     GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
                     GIT_TAG b424665 # release-2.0.20
)
FetchContent_MakeAvailable(sdl)

set(COMMON_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/common/include)
set(COMMON_SOURCE_DIR ${PROJECT_SOURCE_DIR}/common/src)

# MATHS
set(MATHS_BUILD_EXECUTABLE OFF) 
add_subdirectory(maths)

add_subdirectory(game)
add_subdirectory(engine)
add_subdirectory(editor)
add_subdirectory(launcher)

add_dependencies(EngineFrakkas GameFrakkas)
add_dependencies(EditorFrakkas EngineFrakkas)
add_dependencies(Launcher EngineFrakkas)


#add_library(GameFrakkas SHARED game/src/game.cpp)