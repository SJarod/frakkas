add_library(GameFrakkas SHARED
            src/bar.cpp
            ${COMMON_SOURCE_DIR}/game/component.cpp
            ${COMMON_SOURCE_DIR}/game/entity.cpp
            ${COMMON_SOURCE_DIR}/game/transform.cpp
            ${COMMON_SOURCE_DIR}/resources/serializer.cpp
            ${IMGUI_SOURCE_FILES}
)

target_include_directories(GameFrakkas
                           PRIVATE include
                           PRIVATE ${COMMON_INCLUDE_DIR}
                           PRIVATE ${PROJECT_SOURCE_DIR}/maths/include
                           PRIVATE ${PROJECT_SOURCE_DIR}/maths/inline
                           PRIVATE ${imgui_SOURCE_DIR}
)

add_custom_command(TARGET GameFrakkas POST_BUILD
                   COMMAND ${CMAKE_COMMAND} ARGS -E copy_directory
                   ${PROJECT_SOURCE_DIR}/game/assets
                   ${PROJECT_SOURCE_DIR}/build/debug/game/assets

                   COMMAND ${CMAKE_COMMAND} ARGS -E copy_directory
                   ${PROJECT_SOURCE_DIR}/game/assets
                   ${PROJECT_SOURCE_DIR}/build/release/game/assets
)
