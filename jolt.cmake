target_include_directories(EngineFrakkas PRIVATE ${CMAKE_SOURCE_DIR}/lib/jolt/include)

if(WIN32)
    find_library(JOLT_LIB_DEBUG Jolt_d ${CMAKE_SOURCE_DIR}/lib/jolt/windows)
    find_library(JOLT_LIB_RELEASE Jolt ${CMAKE_SOURCE_DIR}/lib/jolt/windows)
    target_link_libraries(EngineFrakkas
                          PRIVATE debug ${JOLT_LIB_DEBUG}
                          PRIVATE optimized ${JOLT_LIB_RELEASE})

elseif(UNIX)
    find_library(JOLT_LIB Jolt ${CMAKE_SOURCE_DIR}/lib/jolt/linux)
    target_link_libraries(EngineFrakkas PRIVATE ${JOLT_LIB})
endif()