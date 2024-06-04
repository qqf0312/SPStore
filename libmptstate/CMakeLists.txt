file(GLOB SRC_LIST "*.cpp" "*.hpp")
file(GLOB HEADERS "*.h")

add_library(mptstate ${SRC_LIST} ${HEADERS})
link_libraries("/root/ex_sharding/libmptstate/libpointproofs.a")
target_link_libraries(mptstate PRIVATE ethcore security erasure RocksDB Boost::Serialization Boost::Thread initializer pointproofs)
