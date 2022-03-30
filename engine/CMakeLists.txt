add_library(EngineFrakkas STATIC
		src/game/entity_manager.cpp
		src/renderer/model.cpp
        src/renderer/lowlevel/camera.cpp
		src/renderer/lowlevel/lowrenderer.cpp
		src/resources/program_shader.cpp
		src/resources/resources_manager.cpp
		src/game/component.cpp
		src/game/entity.cpp
		src/resources/serializer.cpp
        src/game/transform.cpp
		${PROJECT_SOURCE_DIR}/lib/stb_image.c
		${IMGUI_SOURCE_FILES}
		src/log.cpp
        src/game/drawable.cpp)

target_include_directories(EngineFrakkas
                           PRIVATE include
                           PRIVATE ${PROJECT_SOURCE_DIR}/lib
                           PRIVATE ${MATHS_INCLUDE_DIR}
						   PRIVATE ${GAME_INCLUDE_DIR}
						   PRIVATE ${imgui_SOURCE_DIR}
)

# Include jolt and assimp libraries
include(${PROJECT_SOURCE_DIR}/jolt.cmake)
include(${PROJECT_SOURCE_DIR}/assimp.cmake)

target_link_libraries(EngineFrakkas PRIVATE SDL2-static)

set_target_properties(EngineFrakkas PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

add_custom_command(TARGET EngineFrakkas POST_BUILD
				   COMMAND ${CMAKE_COMMAND} ARGS -E copy_directory
				   ${PROJECT_SOURCE_DIR}/engine/shaders
				   ${PROJECT_SOURCE_DIR}/build/debug/engine/shaders

				   COMMAND ${CMAKE_COMMAND} ARGS -E copy_directory
				   ${PROJECT_SOURCE_DIR}/engine/shaders
				   ${PROJECT_SOURCE_DIR}/build/release/engine/shaders
)
