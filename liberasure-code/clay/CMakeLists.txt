# clay plugin
message("${CMAKE_SOURCE_DIR}")
file(GLOB sources
	*.h
	*.cc
#${CMAKE_SOURCE_DIR}/erasure-code/common/str_map.cc
)
add_subdirectory(isa)
add_library(ec_clay ${sources})
add_dependencies(ec_clay ec_isa)
SET_TARGET_PROPERTIES(ec_clay PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(ec_clay PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(ec_clay PRIVATE  ec_isa)


