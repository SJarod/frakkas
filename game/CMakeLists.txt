add_library(GameFrakkas SHARED
            src/bar.cpp
            ../common/src/component.cpp
            ../common/src/entity.cpp
)

target_include_directories(GameFrakkas
                           PRIVATE include
                           PRIVATE ${PROJECT_SOURCE_DIR}/common/include
                           PRIVATE ${PROJECT_SOURCE_DIR}/maths/include
                           PRIVATE ${PROJECT_SOURCE_DIR}/maths/inline
)

set_target_properties(GameFrakkas
                      PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib
)