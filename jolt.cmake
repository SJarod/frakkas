if(${CMAKE_HOST_SYSTEM_NAME} MATCHES "Windows")
    find_library(JOLT_LIB Jolt ${PROJECT_SOURCE_DIR}/lib/Jolt/libs)
    target_link_libraries(EngineFrakkas PRIVATE ${JOLT_LIB})

elseif(${CMAKE_HOST_SYSTEM_NAME} MATCHES "Linux")
    find_library(JOLT_LIB libJolt.a ${PROJECT_SOURCE_DIR}/lib/Jolt/libs)
    target_link_libraries(EngineFrakkas PRIVATE ${JOLT_LIB})
endif()