add_library(EngineFrakkas STATIC
            src/entity_manager.cpp
            src/serializer.cpp
            ../common/src/component.cpp
            ../common/src/entity.cpp
        )


target_include_directories(EngineFrakkas
                           PRIVATE include
                           PRIVATE ${PROJECT_SOURCE_DIR}/lib
                           PUBLIC ${PROJECT_SOURCE_DIR}/lib/assimp/include
                           PRIVATE ${PROJECT_SOURCE_DIR}/maths/include
                           PRIVATE ${PROJECT_SOURCE_DIR}/maths/inline
						   PRIVATE ${PROJECT_SOURCE_DIR}/common/include
)

target_link_directories(EngineFrakkas PUBLIC ${PROJECT_SOURCE_DIR}/lib/assimp/windows)
# Include jolt and assimp libraries
include(${PROJECT_SOURCE_DIR}/jolt.cmake)
include(${PROJECT_SOURCE_DIR}/assimp.cmake)

target_link_libraries(EngineFrakkas
                      PRIVATE SDL2-static
                      PRIVATE debug assimp-vc142-mtd
                      PRIVATE optimized assimp-vc142-mt
		              PRIVATE debug zlibstaticd
                      PRIVATE optimized zlibstatic
)

set_target_properties(GameFrakkas PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
