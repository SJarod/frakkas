add_library(EngineFrakkas STATIC
            src/foo.cpp
)


target_include_directories(EngineFrakkas
                           PRIVATE include
                           PRIVATE ${PROJECT_SOURCE_DIR}/lib
)

include(${PROJECT_SOURCE_DIR}/jolt.cmake)

target_link_libraries(EngineFrakkas
                      PRIVATE SDL2-static
)


set_target_properties(GameFrakkas
                      PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)