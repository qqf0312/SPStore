cmake_minimum_required(VERSION 3.4)
set(SCHAIN_CMAKE_DIR "${CMAKE_CURRENT_LIST_DIR}/cmake" CACHE PATH "The path to the cmake directory")
list(APPEND CMAKE_MODULE_PATH ${SCHAIN_CMAKE_DIR})

project(FISCO-BCOS VERSION "2.3.0")
# Suffix like "-rc1" e.t.c. to append to versions wherever needed.

set(VERSION_SUFFIX "")
SET(CMAKE_BUILD_TYPE "Debug")
#SET(jsoncpp_DIR "/home/ubuntu/tmp/vcpkg/installed/x64-linux/share/jsoncpp")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -no-pie -std=c++14  -O0 -Wall -g2 -ggdb -Wtype-limits -Wsign-compare -Wno-dev -pthread")
#find_package(Protobuf CONFIG REQUIRED)
find_package(Threads REQUIRED)
#find_package(gRPC CONFIG REQUIRED)
#set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
#find_package(jsoncpp CONFIG REQUIRED)
enable_testing()   
set(CMAKE_C_FLAGS "-pthread")
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
find_package(Git QUIET)
# if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
#     option(GIT_SUBMODULE "Check submodules during build" ON)
#     if(GIT_SUBMODULE)
#         execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive --remote
#                         WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#                         RESULT_VARIABLE GIT_SUBMOD_RESULT)
#         execute_process(COMMAND ${GIT_EXECUTABLE} checkout 2f757f79f1f304bfc56a24ed2f8b342c891c0594
#                         WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/evmc
#                         RESULT_VARIABLE CHECKOUT_OUTPUT_RESULT)
#         if(NOT GIT_SUBMOD_RESULT EQUAL "0")
#             message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
#         endif()
#     endif()
# endif()

if(NOT EXISTS "${CMAKE_SOURCE_DIR}/evmc/.git")
    message(FATAL_ERROR "The submodules were not downloaded! GIT_SUBMODULE was turned off or failed. Please execute:\n  git submodule update --init")
endif()

# set(CMAKE_SKIP_INSTALL_ALL_DEPENDENCY On)

# basic setting
include(EthOptions)
include(EthBuildInfo)
#include(EthCompilerSettings)
include(EthDependencies)
#add_subdirectory(erasure-tests)
#add_subdirectory(gf-arith-tests)
# install dependencies
include(ProjectTBB)
include(ProjectSnappy)
include(ProjectLevelDB)
include(ProjectRocksDB)
include(ProjectMHD)
include(ProjectCryptopp)
include(ProjectSecp256k1)
include(ProjectJsonCpp)
include(ProjectJsonRpcCpp)
include(ProjectTASSL)
include(ProjectBoost)
include(ProjectLibzdb)
include(ProjectTCMalloc)
#include(ProjectProtobuf)
if (CRYPTO_EXTENSION)
    include(ProjectPaillier)
    include(ProjectGroupSig)
endif()

include_directories("${CMAKE_SOURCE_DIR}" "evmc/include")

#configure_project(TESTS)
add_subdirectory(gf-arithmetic)
add_subdirectory(erasure-codes)
add_subdirectory(lohi-generator)
add_subdirectory(log-table-generator)
add_subdirectory(libbloomfilter)
add_subdirectory(libchannelserver)
add_subdirectory(libdevcore)
add_subdirectory(libdevcrypto)
#add_subdirectory(liberasure-code)
add_subdirectory(libethcore)
add_subdirectory(evmc)
add_subdirectory(libinterpreter)
add_subdirectory(libevm)
add_subdirectory(libtxpool)
add_subdirectory(libstorage)
add_subdirectory(libprecompiled)
add_subdirectory(libnetwork)
add_subdirectory(libp2p)
add_subdirectory(libexecutive)
add_subdirectory(libmptstate)
add_subdirectory(libblockverifier)
add_subdirectory(libstoragestate)
add_subdirectory(libblockchain)
add_subdirectory(libsync)
add_subdirectory(libconsensus)
add_subdirectory(libledger)
add_subdirectory(librpc)
add_subdirectory(libinitializer)
add_subdirectory(libsecurity)
add_subdirectory(libeventfilter)




file(GLOB SRC_LIST "*.cpp" "*.hpp")
file(GLOB HEADERS "*.h")



































FIND_LIBRARY(pointproofs_lib pointproofs /home/qqf/test_spchain/ex_sharding NO_DEFAULT_PATH)
link_directories("/home/qqf/test_spchain/ex_sharding")

link_libraries("/home/qqf/test_spchain/ex_sharding/libpointproofs.a")
# include_directories("/home/zcy/ex_sharding")
add_compile_options(-Wno-unused-variable)
add_compile_options(-Wno-unused-parameter)
add_compile_options(-Wno-unused-function)
add_compile_options(-Wno-missing-field-initializers)
add_compile_options(-Wno-unknown-pragmas)
add_compile_options(-fno-omit-frame-pointer)
add_compile_options(-Wno-dev)
add_compile_options(-Wno-return-type)
add_executable(mini-p2p ${SRC_LIST} ${HEADERS})
target_link_libraries(mini-p2p PUBLIC initializer erasure  mptstate RocksDB )
