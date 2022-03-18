add_library(GameFrakkas SHARED src/behavior_base.cpp)

target_include_directories(GameFrakkas
                           PRIVATE include)

set_target_properties(GameFrakkas
                      PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)