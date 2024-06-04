include(ExternalProject)

set(PROTOBUF_CONFIG ./configure )
set(PROTOBUF_BUILD make)

ExternalProject_Add(protobuf
    PREFIX ${CMAKE_SOURCE_DIR}/deps
    DOWNLOAD_NO_PROGRESS 1
    DOWNLOAD_NAME protobuf-cpp-3.11.3.tar.gz
    #URL https://github.com/protocolbuffers/protobuf/releases/download/v3.11.3/protobuf-cpp-3.11.3.tar.gz
    #URL_HASH SHA256=37c36f1be177f0e37ef181a645cd3baac1000bd322a01c2eff70f3cc8c91749c
    URL /home/qqf/zcy/protobuf-cpp-3.11.3.tar.gz
    BUILD_IN_SOURCE 1
    CONFIGURE_COMMAND ${PROTOBUF_CONFIG}
    LOG_CONFIGURE 1
    LOG_BUILD 1
    LOG_INSTALL 1
    BUILD_COMMAND make
    INSTALL_COMMAND ""
    BUILD_BYPRODUCTS <SOURCE_DIR>/src/.libs/libprotobuf.a
)

ExternalProject_Get_Property(protobuf SOURCE_DIR)
add_library(PROTOBUF STATIC IMPORTED)
set(PROTOBUF_INCLUDE_DIR ${SOURCE_DIR}/src ${SOURCE_DIR}/src/include)
set(PROTOBUF_LIBRARY ${SOURCE_DIR}/src/.libs/libprotobuf.a)
file(MAKE_DIRECTORY ${PROTOBUF_INCLUDE_DIR})  # Must exist.

set_property(TARGET PROTOBUF PROPERTY IMPORTED_LOCATION ${PROTOBUF_LIBRARY})
set_property(TARGET PROTOBUF PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${PROTOBUF_INCLUDE_DIR})
add_dependencies(PROTOBUF protobuf)
unset(SOURCE_DIR)
