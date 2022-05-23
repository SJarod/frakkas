project(Engine
		LANGUAGES CXX C
		VERSION 1.0
		DESCRIPTION "Engine"
)

add_library(${PROJECT_NAME} STATIC
			src/game/entity_manager.cpp
        	src/game/transform.cpp
            src/game/lowcomponent/static_draw.cpp
			src/game/lowcomponent/animated_draw.cpp
			src/game/lowcomponent/camera.cpp
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

target_include_directories(${PROJECT_NAME} PUBLIC include)

target_link_libraries(${PROJECT_NAME}
					  PUBLIC glad
					  PUBLIC SDL2-static
					  PUBLIC TracyClient
					  PUBLIC assimp::assimp
					  PUBLIC jolt
					  PUBLIC miniaudio
					  PRIVATE stb
					  PUBLIC imgui
					  PUBLIC Mathlib
)

add_custom_target(copy_shaders
				  COMMAND ${CMAKE_COMMAND} -E copy_directory
				  ${CMAKE_SOURCE_DIR}/engine/shaders
				  ${CMAKE_SOURCE_DIR}/build/debug/engine/shaders

				  COMMAND ${CMAKE_COMMAND} -E copy_directory
				  ${CMAKE_SOURCE_DIR}/engine/shaders
				  ${CMAKE_SOURCE_DIR}/build/release/engine/shaders
)

add_dependencies(${PROJECT_NAME} copy_shaders)