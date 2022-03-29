add_executable(Launcher launcher.cpp)

target_include_directories(Launcher
                           PRIVATE ${ENGINE_INCLUDE_DIR}
                           PRIVATE ${GAME_INCLUDE_DIR}
                           PRIVATE ${MATHS_INCLUDE_DIR}
)

target_link_libraries(Launcher PRIVATE GameFrakkas)

set_target_properties(Launcher PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)