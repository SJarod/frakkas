add_executable(EditorFrakkas
			   ${PROJECT_SOURCE_DIR}/appicon.rc
               ${PROJECT_SOURCE_DIR}/lib/glad.c
			   ${IMGUI_BACKENDS_FILES}
			   ${tracy_SOURCE_DIR}/TracyClient.cpp
               src/main.cpp
        	   src/editor/editor_render.cpp
        	   src/editor/hierarchy.cpp
        	   src/editor/menu_bar.cpp
        	   src/editor/file_browser.cpp
        	   src/editor/console.cpp
        	   src/editor/inspector.cpp
        	   src/editor/scene.cpp
        	   src/editor/game_scene.cpp
        	   src/editor/debugger.cpp
			   ${imguizmo_SOURCE_DIR}/ImGuizmo.cpp
               src/helpers/game_edit.cpp
)

target_link_libraries(EditorFrakkas
                      PRIVATE EngineFrakkas
					  PRIVATE GameFrakkas
                      PRIVATE SDL2-static
					  PUBLIC TracyClient
)

target_include_directories(EditorFrakkas
                           PRIVATE include
                           PRIVATE ${LIB_INCLUDE_DIR}
                           PRIVATE ${ENGINE_INCLUDE_DIR}
                           PRIVATE ${GAME_INCLUDE_DIR}
                           PRIVATE ${MATHS_INCLUDE_DIR}
						   PRIVATE ${imgui_SOURCE_DIR}
                           PRIVATE ${imgui_SOURCE_DIR}/backends
						   PRIVATE ${tracy_SOURCE_DIR}
						   PRIVATE ${imguizmo_SOURCE_DIR}
)

set_target_properties(EditorFrakkas PROPERTIES
					  RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/build/debug
					  RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/build/release
					  VS_DEBUGGER_WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

add_custom_target(copy_editor_assets
				   COMMAND ${CMAKE_COMMAND} -E copy_directory
				   ${PROJECT_SOURCE_DIR}/editor/assets
		 		   ${PROJECT_SOURCE_DIR}/build/debug/editor/assets
				   COMMAND ${CMAKE_COMMAND} -E copy_if_different
				   ${PROJECT_SOURCE_DIR}/imgui.ini
				   ${PROJECT_SOURCE_DIR}/build/debug

				   COMMAND ${CMAKE_COMMAND} -E copy_directory
				   ${PROJECT_SOURCE_DIR}/editor/assets
				   ${PROJECT_SOURCE_DIR}/build/release/editor/assets
				   COMMAND ${CMAKE_COMMAND} -E copy_if_different
				   ${PROJECT_SOURCE_DIR}/imgui.ini
				   ${PROJECT_SOURCE_DIR}/build/release
)

add_dependencies(EditorFrakkas copy_editor_assets)
