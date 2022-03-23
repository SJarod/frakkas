add_library(GameFrakkas SHARED
            src/bar.cpp
            ${COMMON_SOURCE_DIR}/component.cpp
            ${COMMON_SOURCE_DIR}/entity.cpp
)

target_include_directories(GameFrakkas
                           PRIVATE include
                           PRIVATE ${COMMON_INCLUDE_DIR}
                           PRIVATE ${PROJECT_SOURCE_DIR}/maths/include
                           PRIVATE ${PROJECT_SOURCE_DIR}/maths/inline
)