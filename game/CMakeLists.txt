project(Game
		LANGUAGES CXX C
		VERSION 1.0
		DESCRIPTION "Game"
)

add_library(${PROJECT_NAME} STATIC
        	src/game/world.cpp
			assets/scripts/rumble_trigger.cpp
	src/game/component_register.cpp
		    assets/scripts/fps_movement.cpp
		    assets/scripts/player_movement.cpp
		    assets/scripts/follow_camera.cpp
)

target_include_directories(${PROJECT_NAME}
		                   PUBLIC include
		                   PUBLIC assets/scripts
)

target_link_libraries(${PROJECT_NAME} PUBLIC Engine)

if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
	add_custom_target(copy_game_assets
					  COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_SOURCE_DIR}/build/debug/game/assets

	                  COMMAND ${CMAKE_COMMAND} -E copy_directory
	                  ${CMAKE_SOURCE_DIR}/game/assets
	                  ${CMAKE_SOURCE_DIR}/build/debug/game/assets
	)
elseif(${CMAKE_BUILD_TYPE} MATCHES "Release")
	add_custom_target(copy_game_assets
					  COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_SOURCE_DIR}/build/release/game/assets

	                  COMMAND ${CMAKE_COMMAND} -E copy_directory
	                  ${CMAKE_SOURCE_DIR}/game/assets
	                  ${CMAKE_SOURCE_DIR}/build/release/game/assets
	)
endif()

add_dependencies(${PROJECT_NAME} copy_game_assets)
