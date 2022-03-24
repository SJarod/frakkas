add_executable(Launcher launcher.cpp)

target_include_directories(Launcher
                           PRIVATE ${PROJECT_SOURCE_DIR}/engine/include
                           PRIVATE ${COMMON_INCLUDE_DIR}
)

target_link_libraries(Launcher PRIVATE EngineFrakkas)

set_target_properties(Launcher PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)