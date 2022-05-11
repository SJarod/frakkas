add_library(EngineFrakkas STATIC
			${PROJECT_SOURCE_DIR}/lib/stb_image/stb_image.c
			${PROJECT_SOURCE_DIR}/lib/miniaudio/miniaudio.c
			${IMGUI_SOURCE_FILES}
			src/game/entity_manager.cpp
        	src/game/transform.cpp
            src/game/lowcomponent/static_draw.cpp
			src/game/lowcomponent/animated_draw.cpp
            src/game/lowcomponent/camera_component.cpp
            src/game/lowcomponent/component.cpp
            src/game/lowcomponent/sound_component.cpp
			src/game/lowcomponent/drawable.cpp
		    src/game/lowcomponent/collider/box_collider.cpp 
		    src/game/lowcomponent/collider/collider.cpp 
		    src/game/lowcomponent/collider/sphere_collider.cpp
			src/game/entity.cpp
            src/game/time_manager.cpp
            src/game/inputs_manager.cpp
			src/game/component_generator.cpp
			src/renderer/render_object.cpp
			src/renderer/model.cpp
			src/renderer/skeletal_model.cpp
			src/renderer/point.cpp
			src/renderer/screen.cpp
        	src/renderer/lowlevel/camera.cpp
			src/renderer/lowlevel/lowrenderer.cpp
        	src/renderer/graph.cpp
			src/resources/program_shader.cpp
			src/resources/resources_manager.cpp
			src/resources/serializer.cpp
			src/resources/mesh.cpp
			src/resources/skeletal_mesh.cpp
			src/resources/skeletal_animation.cpp
			src/resources/texture.cpp
			src/resources/sound.cpp
			src/animation/animator.cpp
        	src/debug/log.cpp
            src/engine.cpp
        	src/multithread/threadpool.cpp
		    src/physic/physic_scene.cpp
			src/physic/contact_listener.cpp
			src/physic/layers.cpp
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

set_target_properties(EngineFrakkas PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

add_custom_target(copy_shaders
				  COMMAND ${CMAKE_COMMAND} -E copy_directory
				  ${PROJECT_SOURCE_DIR}/engine/shaders
				  ${PROJECT_SOURCE_DIR}/build/debug/engine/shaders

				  COMMAND ${CMAKE_COMMAND} -E copy_directory
				  ${PROJECT_SOURCE_DIR}/engine/shaders
				  ${PROJECT_SOURCE_DIR}/build/release/engine/shaders
)

add_dependencies(EngineFrakkas copy_shaders)
