if(${CMAKE_HOST_SYSTEM_NAME} MATCHES "Windows")
    find_library(ASSIMP_LIB assimp-vc143-mt ${PROJECT_SOURCE_DIR}/lib/assimp)
    find_library(ZLIB_LIB zlibstatic ${PROJECT_SOURCE_DIR}/lib/assimp)

    target_include_directories(EngineFrakkas PUBLIC ${PROJECT_SOURCE_DIR}/lib/assimp/include)

    target_link_libraries(EngineFrakkas
                          PRIVATE ${ASSIMP_LIB}
                          PRIVATE ${ZLIB_LIB}
    )

elseif(${CMAKE_HOST_SYSTEM_NAME} MATCHES "Linux")
find_package(assimp REQUIRED)
target_include_directories(EngineFrakkas PRIVATE ${assimp_INCLUDE_DIRS})
target_link_libraries(EngineFrakkas PRIVATE assimp::assimp)
endif()
