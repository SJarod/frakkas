add_library(GameFrakkas src/bar.cpp)

target_include_directories(GameFrakkas
                           PRIVATE include
                           PRIVATE ${ENGINE_INCLUDE_DIR}
                           PRIVATE ${MATHS_INCLUDE_DIR}
                           PRIVATE ${imgui_SOURCE_DIR}
)

target_link_directories(GameFrakkas PRIVATE EngineFrakkas)

set_target_properties(GameFrakkas PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

add_custom_command(TARGET GameFrakkas POST_BUILD
                   COMMAND ${CMAKE_COMMAND} ARGS -E copy_directory
                   ${PROJECT_SOURCE_DIR}/game/assets
                   ${PROJECT_SOURCE_DIR}/build/debug/game/assets

                   COMMAND ${CMAKE_COMMAND} ARGS -E copy_directory
                   ${PROJECT_SOURCE_DIR}/game/assets
                   ${PROJECT_SOURCE_DIR}/build/release/game/assets
)
