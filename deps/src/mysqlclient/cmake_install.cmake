# Install script for directory: /home/ubuntu/ex_sharding/deps/src/mysqlclient

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/ubuntu/ex_sharding/deps")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "RelWithDebInfo")
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

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xReadmex" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/." TYPE FILE OPTIONAL FILES
    "/home/ubuntu/ex_sharding/deps/src/mysqlclient/COPYING"
    "/home/ubuntu/ex_sharding/deps/src/mysqlclient/LICENSE.mysql"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xReadmex" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/." TYPE FILE FILES "/home/ubuntu/ex_sharding/deps/src/mysqlclient/README")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xDocumentationx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/docs" TYPE FILE FILES
    "/home/ubuntu/ex_sharding/deps/src/mysqlclient/Docs/INFO_SRC"
    "/home/ubuntu/ex_sharding/deps/src/mysqlclient/Docs/INFO_BIN"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xDebugReadmex" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/." TYPE FILE OPTIONAL RENAME "LICENSE.mysql-debug" FILES "/home/ubuntu/ex_sharding/deps/src/mysqlclient/LICENSE.mysql")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xDebugReadmex" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/." TYPE FILE OPTIONAL RENAME "COPYING-debug" FILES "/home/ubuntu/ex_sharding/deps/src/mysqlclient/COPYING")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xDebugReadmex" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/." TYPE FILE RENAME "README-debug" FILES "/home/ubuntu/ex_sharding/deps/src/mysqlclient/README")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/ubuntu/ex_sharding/deps/src/mysqlclient/zlib/cmake_install.cmake")
  include("/home/ubuntu/ex_sharding/deps/src/mysqlclient/extra/yassl/cmake_install.cmake")
  include("/home/ubuntu/ex_sharding/deps/src/mysqlclient/extra/yassl/taocrypt/cmake_install.cmake")
  include("/home/ubuntu/ex_sharding/deps/src/mysqlclient/include/cmake_install.cmake")
  include("/home/ubuntu/ex_sharding/deps/src/mysqlclient/dbug/cmake_install.cmake")
  include("/home/ubuntu/ex_sharding/deps/src/mysqlclient/strings/cmake_install.cmake")
  include("/home/ubuntu/ex_sharding/deps/src/mysqlclient/vio/cmake_install.cmake")
  include("/home/ubuntu/ex_sharding/deps/src/mysqlclient/regex/cmake_install.cmake")
  include("/home/ubuntu/ex_sharding/deps/src/mysqlclient/mysys/cmake_install.cmake")
  include("/home/ubuntu/ex_sharding/deps/src/mysqlclient/mysys_ssl/cmake_install.cmake")
  include("/home/ubuntu/ex_sharding/deps/src/mysqlclient/libmysql/cmake_install.cmake")
  include("/home/ubuntu/ex_sharding/deps/src/mysqlclient/extra/cmake_install.cmake")
  include("/home/ubuntu/ex_sharding/deps/src/mysqlclient/scripts/cmake_install.cmake")
  include("/home/ubuntu/ex_sharding/deps/src/mysqlclient/testclients/cmake_install.cmake")
  include("/home/ubuntu/ex_sharding/deps/src/mysqlclient/support-files/cmake_install.cmake")
  include("/home/ubuntu/ex_sharding/deps/src/mysqlclient/packaging/WiX/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/ubuntu/ex_sharding/deps/src/mysqlclient/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
