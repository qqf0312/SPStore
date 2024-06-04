file(GLOB SRC_LIST "*.*")

add_library(eccommon STATIC ${SRC_LIST})
target_include_directories(eccommon PRIVATE ${OPENSSL_INCLUDE_DIR})
target_link_libraries(eccommon arch)