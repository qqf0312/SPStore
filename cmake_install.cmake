# Install script for directory: /home/ubuntu/ex_sharding

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/ubuntu/ex_sharding/gf-arithmetic/cmake_install.cmake")
  include("/home/ubuntu/ex_sharding/erasure-codes/cmake_install.cmake")
  include("/home/ubuntu/ex_sharding/lohi-generator/cmake_install.cmake")
  include("/home/ubuntu/ex_sharding/log-table-generator/cmake_install.cmake")
  include("/home/ubuntu/ex_sharding/libbloomfilter/cmake_install.cmake")
  include("/home/ubuntu/ex_sharding/libchannelserver/cmake_install.cmake")
  include("/home/ubuntu/ex_sharding/libdevcore/cmake_install.cmake")
  include("/home/ubuntu/ex_sharding/libdevcrypto/cmake_install.cmake")
  include("/home/ubuntu/ex_sharding/libethcore/cmake_install.cmake")
  include("/home/ubuntu/ex_sharding/evmc/cmake_install.cmake")
  include("/home/ubuntu/ex_sharding/libinterpreter/cmake_install.cmake")
  include("/home/ubuntu/ex_sharding/libevm/cmake_install.cmake")
  include("/home/ubuntu/ex_sharding/libtxpool/cmake_install.cmake")
  include("/home/ubuntu/ex_sharding/libstorage/cmake_install.cmake")
  include("/home/ubuntu/ex_sharding/libprecompiled/cmake_install.cmake")
  include("/home/ubuntu/ex_sharding/libnetwork/cmake_install.cmake")
  include("/home/ubuntu/ex_sharding/libp2p/cmake_install.cmake")
  include("/home/ubuntu/ex_sharding/libexecutive/cmake_install.cmake")
  include("/home/ubuntu/ex_sharding/libmptstate/cmake_install.cmake")
  include("/home/ubuntu/ex_sharding/libblockverifier/cmake_install.cmake")
  include("/home/ubuntu/ex_sharding/libstoragestate/cmake_install.cmake")
  include("/home/ubuntu/ex_sharding/libblockchain/cmake_install.cmake")
  include("/home/ubuntu/ex_sharding/libsync/cmake_install.cmake")
  include("/home/ubuntu/ex_sharding/libconsensus/cmake_install.cmake")
  include("/home/ubuntu/ex_sharding/libledger/cmake_install.cmake")
  include("/home/ubuntu/ex_sharding/librpc/cmake_install.cmake")
  include("/home/ubuntu/ex_sharding/libinitializer/cmake_install.cmake")
  include("/home/ubuntu/ex_sharding/libsecurity/cmake_install.cmake")
  include("/home/ubuntu/ex_sharding/libeventfilter/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/ubuntu/ex_sharding/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
