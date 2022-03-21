target_include_directories(EngineFrakkas PRIVATE ${CMAKE_SOURCE_DIR}/lib/assimp/include)

if(WIN32)
    find_library(ASSIMP_LIB_DEBUG assimp-vc142-mtd ${CMAKE_SOURCE_DIR}/lib/assimp/windows)
    find_library(ASSIMP_LIB_RELEASE assimp-vc142-mt ${CMAKE_SOURCE_DIR}/lib/assimp/windows)
    target_link_libraries(EngineFrakkas
                          PRIVATE debug ${ASSIMP_LIB_DEBUG}
                          PRIVATE optimized ${ASSIMP_LIB_RELEASE})

elseif(UNIX)
    find_library(ASSIMP_LIB_DEBUG libassimp ${CMAKE_SOURCE_DIR}/lib/assimp/linux)
    find_library(ASSIMP_LIB_RELEASE libassimpd ${CMAKE_SOURCE_DIR}/lib/assimp/linux)
    target_link_libraries(EngineFrakkas
                          PRIVATE debug ${ASSIMP_LIB_DEBUG}
                          PRIVATE optimized ${ASSIMP_LIB_RELEASE})
endif()