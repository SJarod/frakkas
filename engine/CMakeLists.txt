add_library(EngineFrakkas STATIC
			${PROJECT_SOURCE_DIR}/lib/stb_image/stb_image.c
			${PROJECT_SOURCE_DIR}/lib/miniaudio/miniaudio.c
			${IMGUI_SOURCE_FILES}
			src/game/entity_manager.cpp
        	src/game/transform.cpp
        	src/game/drawable.cpp
			src/game/camera_component.cpp
			src/game/component.cpp
			src/game/entity.cpp
            src/game/time_manager.cpp
            src/game/inputs_manager.cpp
			src/game/light_component.cpp
			src/renderer/model.cpp
			src/game/sound_component.cpp
        	src/renderer/lowlevel/camera.cpp
			src/renderer/lowlevel/lowrenderer.cpp
			src/resources/program_shader.cpp
			src/resources/resources_manager.cpp
			src/resources/serializer.cpp
			src/resources/texture.cpp
			src/resources/sound.cpp
        	src/log.cpp
            src/engine.cpp
	    	src/threadpool.cpp
)

target_include_directories(EngineFrakkas
                           PRIVATE include
                           PRIVATE ${LIB_INCLUDE_DIR}
                           PRIVATE ${MATHS_INCLUDE_DIR}
						   PRIVATE ${imgui_SOURCE_DIR}
		                   PRIVATE ${sdl_INCLUDE_DIR}
						   PRIVATE ${tracy_SOURCE_DIR}
)

# Include jolt and assimp libraries
include(${PROJECT_SOURCE_DIR}/jolt.cmake)
include(${PROJECT_SOURCE_DIR}/assimp.cmake)

target_link_libraries(EngineFrakkas
					  PRIVATE SDL2-static
					  PUBLIC TracyClient
)

set_target_properties(EngineFrakkas PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${LIB_INCLUDE_DIR})

add_custom_target(copy_shaders
				  COMMAND ${CMAKE_COMMAND} -E copy_directory
				  ${PROJECT_SOURCE_DIR}/engine/shaders
				  ${PROJECT_SOURCE_DIR}/build/debug/engine/shaders

				  COMMAND ${CMAKE_COMMAND} -E copy_directory
				  ${PROJECT_SOURCE_DIR}/engine/shaders
				  ${PROJECT_SOURCE_DIR}/build/release/engine/shaders
)

add_dependencies(EngineFrakkas copy_shaders)
