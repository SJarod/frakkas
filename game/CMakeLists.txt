add_library(GameFrakkas STATIC
		src/game/bar.cpp
		src/game/world.cpp
		src/game/fps_movement.cpp
		src/game/player_movement.cpp
		src/game/follow_camera.cpp
		src/game/lowcomponent/component_register.cpp
        ${PROJECT_SOURCE_DIR}/lib/glad.c
)

target_include_directories(GameFrakkas
                           PRIVATE include
                           PRIVATE ${ENGINE_INCLUDE_DIR}
                           PRIVATE ${MATHS_INCLUDE_DIR}
                           PRIVATE ${imgui_SOURCE_DIR}
                           PRIVATE ${sdl_INCLUDE_DIR}
                           PRIVATE ${LIB_INCLUDE_DIR}
						   PRIVATE ${PROJECT_SOURCE_DIR}/lib/Jolt
)

target_link_directories(GameFrakkas PRIVATE EngineFrakkas)

set_target_properties(GameFrakkas PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

add_custom_target(copy_game_assets
                  COMMAND ${CMAKE_COMMAND} -E copy_directory
                  ${PROJECT_SOURCE_DIR}/game/assets
                  ${PROJECT_SOURCE_DIR}/build/debug/game/assets

                  COMMAND ${CMAKE_COMMAND} -E copy_directory
                  ${PROJECT_SOURCE_DIR}/game/assets
                  ${PROJECT_SOURCE_DIR}/build/release/game/assets
)

add_dependencies(GameFrakkas copy_game_assets)
