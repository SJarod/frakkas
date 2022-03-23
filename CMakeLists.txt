cmake_minimum_required(VERSION 3.9...3.21)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

set(CMAKE_CXX_STANDARD 20 CACHE STRING "Language standard" FORCE)
set(CMAKE_CXX_STANDARD_REQUIRED ON CACHE BOOL "Use the C++ standard" FORCE)
set(CMAKE_CXX_EXTENSIONS OFF CACHE BOOL "" FORCE)

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

set(COMMON_SOURCE_DIR ${PROJECT_SOURCE_DIR}/common)

# MATHS
set(MATHS_BUILD_EXECUTABLE OFF) 
add_subdirectory(maths)

# GAME
add_subdirectory(game)

# EDITOR
add_subdirectory(editor)

# ENGINE
add_subdirectory(engine)

# LAUNCHER
add_subdirectory(launcher)

add_dependencies(EngineFrakkas
                 GameFrakkas
)

add_dependencies(EditorFrakkas
                 EngineFrakkas
)

add_dependencies(Launcher
                 EngineFrakkas)


#add_library(GameFrakkas SHARED game/src/game.cpp)
