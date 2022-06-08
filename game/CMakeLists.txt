project(Game
		LANGUAGES CXX C
		VERSION 1.0
		DESCRIPTION "Game"
)

add_library(${PROJECT_NAME} STATIC
			assets/scripts/portal.cpp
			assets/Scripts/physic_goal_trigger.cpp
			assets/Scripts/physic_ball_object.cpp
			assets/Scripts/physic_ball_trigger.cpp
			assets/Scripts/physic_player.cpp
			assets/Scripts/level_selector.cpp
			assets/Scripts/portal_manager.cpp
			assets/scripts/life_item.cpp
			assets/scripts/pause_trigger.cpp
			assets/scripts/life.cpp
        	assets/scripts/game_over_trigger.cpp
			assets/scripts/player_weapon.cpp
			assets/scripts/enemy_weapon.cpp
        	assets/scripts/enemy_stats.cpp
			assets/scripts/enemy_mesh_rotation.cpp
			assets/Scripts/player.cpp
			assets/Scripts/spawner.cpp
			assets/Scripts/skeleton.cpp
			assets/Scripts/golem.cpp
		    assets/Scripts/fps_movement.cpp
			assets/Scripts/follow_camera.cpp
			assets/Scripts/player_mesh_rotation.cpp
 		    assets/Scripts/menu_script.cpp
        )

target_include_directories(${PROJECT_NAME}
		                   PUBLIC include
		                   PUBLIC assets/Scripts
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
