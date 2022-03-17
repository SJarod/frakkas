cmake_minimum_required(VERSION 3.21)
project(EngineFrakkas)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

add_library(GameFrakkas SHARED game/src/behavior_base.cpp)

target_include_directories(GameFrakkas PRIVATE game/include)
set_target_properties(GameFrakkas
        PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

add_library(EditorFrakkas STATIC editor/src/editor_render.cpp)

target_include_directories(EditorFrakkas PRIVATE editor/include)
set_target_properties(EditorFrakkas
        PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

add_executable(EngineFrakkas engine/src/main.cpp editor/include/Editor/editor_render.hpp engine/src/foo.cpp)

add_dependencies(EngineFrakkas EditorFrakkas GameFrakkas)
target_include_directories(EngineFrakkas PRIVATE engine/include editor/include game/include)
target_link_directories(EngineFrakkas PRIVATE lib)

target_link_libraries(EngineFrakkas EditorFrakkas)



#add_library(GameFrakkas SHARED game/src/game.cpp)
