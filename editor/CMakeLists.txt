add_executable(EditorFrakkas
			   ${PROJECT_SOURCE_DIR}/appicon.rc
               ${PROJECT_SOURCE_DIR}/lib/glad.c
			   ${IMGUI_BACKENDS_FILES}
			   ${tracy_SOURCE_DIR}/TracyClient.cpp
               src/main.cpp
               src/editor_render.cpp
               src/hierarchy.cpp
               src/menu_bar.cpp
               src/file_browser.cpp
        	   src/console.cpp
			   src/inspector.cpp
			   src/scene.cpp
			   src/game_scene.cpp
        src/debugger.cpp)

target_link_libraries(EditorFrakkas
                      PRIVATE EngineFrakkas
                      PRIVATE SDL2-static
					  PUBLIC TracyClient
)

target_include_directories(EditorFrakkas
                           PRIVATE include
                           PRIVATE ${ENGINE_INCLUDE_DIR}
                           PRIVATE ${MATHS_INCLUDE_DIR}
                           PRIVATE ${PROJECT_SOURCE_DIR}/lib
                           PRIVATE ${imgui_SOURCE_DIR}
                           PRIVATE ${imgui_SOURCE_DIR}/backends
						   PRIVATE ${tracy_SOURCE_DIR}
)

set_target_properties(EditorFrakkas PROPERTIES
					  RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/build/debug
					  RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/build/release
					  VS_DEBUGGER_WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

add_custom_command(TARGET EditorFrakkas POST_BUILD
		 		   COMMAND ${CMAKE_COMMAND} ARGS -E copy_directory
				   ${PROJECT_SOURCE_DIR}/editor/assets
		 		   ${PROJECT_SOURCE_DIR}/build/debug/editor/assets
				   COMMAND ${CMAKE_COMMAND} -E copy
				   ${PROJECT_SOURCE_DIR}/imgui.ini
				   ${PROJECT_SOURCE_DIR}/build/debug

				   COMMAND ${CMAKE_COMMAND} ARGS -E copy_directory
				   ${PROJECT_SOURCE_DIR}/editor/assets
				   ${PROJECT_SOURCE_DIR}/build/release/editor/assets
				   COMMAND ${CMAKE_COMMAND} -E copy
				   ${PROJECT_SOURCE_DIR}/imgui.ini
				   ${PROJECT_SOURCE_DIR}/build/release
)
