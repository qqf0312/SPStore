file(GLOB SRC_LIST "*.cpp")
file(GLOB HEADERS "*.h")

add_library(mptstate ${SRC_LIST} ${HEADERS})
link_libraries("/root/ex_sharding/libmpt_state/libpointproofs.a")
target_link_libraries(mptstate PRIVATE ethcore security)
