project(Launcher
        LANGUAGES CXX C
        VERSION 1.0
        DESCRIPTION "Launcher"
)

add_executable(${PROJECT_NAME} launcher.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE Game)

set_target_properties(${PROJECT_NAME} PROPERTIES
                      RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/build/debug
                      RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/build/release
)