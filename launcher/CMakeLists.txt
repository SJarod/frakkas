add_executable(Launcher launcher.cpp)

target_include_directories(Launcher
                           PRIVATE ${PROJECT_SOURCE_DIR}/engine/include
                           PRIVATE ${COMMON_INCLUDE_DIR}
)

target_link_libraries(Launcher PRIVATE EngineFrakkas)