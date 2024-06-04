## erasure code plugins
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -std=c++17 -O0 -g2 -ggdb")
SET(CMAKE_BUILD_TYPE "Debug")
file(GLOB SRC_LIST "*.cc")
file(GLOB HEADERS "*.h")
add_subdirectory(arch)
add_subdirectory(common)
add_subdirectory(clay)
add_library(erasure_code ${SRC_LIST} ${HEADERS})
# target_include_directories(erasure_code PUBLIC  ${CMAKE_SOURCE_DIR})
target_link_libraries(erasure_code PRIVATE ec_clay  arch eccommon)

