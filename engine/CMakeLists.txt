add_library(EngineFrakkas STATIC
        	src/engine/entity_manager.cpp
        	src/engine/serializer.cpp
			src/engine/model.cpp
			src/engine/engine_entity.cpp
			src/renderer/lowlevel/camera.cpp
			src/renderer/lowlevel/lowrenderer.cpp
			src/resources/program_shader.cpp
		    src/resources/resources_manager.cpp
			${COMMON_SOURCE_DIR}/component.cpp
			${COMMON_SOURCE_DIR}/entity.cpp
		    ${PROJECT_SOURCE_DIR}/lib/stb_image.c
)

target_include_directories(EngineFrakkas
                           PRIVATE include
                           PRIVATE ${PROJECT_SOURCE_DIR}/lib
                           PRIVATE ${PROJECT_SOURCE_DIR}/maths/include
                           PRIVATE ${PROJECT_SOURCE_DIR}/maths/inline
						   PRIVATE ${PROJECT_SOURCE_DIR}/common/include
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