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

# Tracy
FetchContent_Declare(tracy
                     GIT_REPOSITORY https://github.com/wolfpld/tracy.git
                     GIT_TAG 9ba7171 # v0.8
                     GIT_SHALLOW TRUE
                     GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(tracy)

if(NOT EXISTS ${PROJECT_SOURCE_DIR}/lib/tracy)
file(ARCHIVE_EXTRACT INPUT ${PROJECT_SOURCE_DIR}/lib/tracy_executable.7z
                     DESTINATION ${PROJECT_SOURCE_DIR}/lib
)
endif()

# SDL2
FetchContent_Declare(sdl
                     GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
                     GIT_TAG b424665 # release-2.0.20
)
FetchContent_MakeAvailable(sdl)

# ImGui
FetchContent_Declare(imgui
                     GIT_REPOSITORY https://github.com/ocornut/imgui.git
                     GIT_TAG docking # branch name
)
FetchContent_MakeAvailable(imgui)

set(IMGUI_SOURCE_FILES ${imgui_SOURCE_DIR}/imgui.cpp
                       ${imgui_SOURCE_DIR}/imgui_demo.cpp
                       ${imgui_SOURCE_DIR}/imgui_draw.cpp
                       ${imgui_SOURCE_DIR}/imgui_tables.cpp
                       ${imgui_SOURCE_DIR}/imgui_widgets.cpp
)

set(IMGUI_BACKENDS_FILES ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl.cpp
                         ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

set(ENGINE_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/engine/include)
set(GAME_INCLUDE_DIRE ${PROJECT_SOURCE_DIR}/game/include)
set(MATHS_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/maths/include
                      ${PROJECT_SOURCE_DIR}/maths/inline
)
set(LIB_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/lib
                    ${PROJECT_SOURCE_DIR}/lib/stb_image
                    ${PROJECT_SOURCE_DIR}/lib/miniaudio
)

# MATHS
set(MATHS_BUILD_EXECUTABLE OFF) 
add_subdirectory(maths)

add_subdirectory(engine)
add_subdirectory(game)
add_subdirectory(editor)
add_subdirectory(launcher)

add_dependencies(GameFrakkas EngineFrakkas)
add_dependencies(EditorFrakkas EngineFrakkas)
add_dependencies(Launcher GameFrakkas)
