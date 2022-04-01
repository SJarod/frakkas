if(${CMAKE_HOST_SYSTEM_NAME} MATCHES "Windows")
    find_library(ASSIMP_LIB_DEBUG assimp-vc142-mtd ${PROJECT_SOURCE_DIR}/lib/assimp/windows)
    find_library(ASSIMP_LIB_RELEASE assimp-vc142-mt ${PROJECT_SOURCE_DIR}/lib/assimp/windows)

    find_library(ZLIB_LIB_DEBUG zlibstaticd ${PROJECT_SOURCE_DIR}/lib/assimp/windows)
    find_library(ZLIB_LIB_RELEASE zlibstatic ${PROJECT_SOURCE_DIR}/lib/assimp/windows)

    target_include_directories(EngineFrakkas PUBLIC ${PROJECT_SOURCE_DIR}/lib/assimp/include)

    target_link_libraries(EngineFrakkas
                          PRIVATE debug ${ASSIMP_LIB_DEBUG}
                          PRIVATE optimized ${ASSIMP_LIB_RELEASE}
                          PRIVATE debug ${ZLIB_LIB_DEBUG}
                          PRIVATE optimized ${ZLIB_LIB_RELEASE}
    )

elseif(${CMAKE_HOST_SYSTEM_NAME} MATCHES "Linux")
find_package(assimp REQUIRED)
target_include_directories(EngineFrakkas PRIVATE ${assimp_INCLUDE_DIRS})
target_link_libraries(EngineFrakkas PRIVATE assimp::assimp)
endif()
