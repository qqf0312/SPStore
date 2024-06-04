# Install script for directory: /home/ubuntu/ex_sharding/deps/src/mysqlclient/include

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

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xDevelopmentx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "/home/ubuntu/ex_sharding/deps/src/mysqlclient/include/mysql.h"
    "/home/ubuntu/ex_sharding/deps/src/mysqlclient/include/mysql_com.h"
    "/home/ubuntu/ex_sharding/deps/src/mysqlclient/include/my_command.h"
    "/home/ubuntu/ex_sharding/deps/src/mysqlclient/include/mysql_time.h"
    "/home/ubuntu/ex_sharding/deps/src/mysqlclient/include/my_list.h"
    "/home/ubuntu/ex_sharding/deps/src/mysqlclient/include/my_alloc.h"
    "/home/ubuntu/ex_sharding/deps/src/mysqlclient/include/typelib.h"
    "/home/ubuntu/ex_sharding/deps/src/mysqlclient/include/binary_log_types.h"
    "/home/ubuntu/ex_sharding/deps/src/mysqlclient/include/my_dbug.h"
    "/home/ubuntu/ex_sharding/deps/src/mysqlclient/include/m_string.h"
    "/home/ubuntu/ex_sharding/deps/src/mysqlclient/include/my_sys.h"
    "/home/ubuntu/ex_sharding/deps/src/mysqlclient/include/my_xml.h"
    "/home/ubuntu/ex_sharding/deps/src/mysqlclient/include/mysql_embed.h"
    "/home/ubuntu/ex_sharding/deps/src/mysqlclient/include/my_thread.h"
    "/home/ubuntu/ex_sharding/deps/src/mysqlclient/include/my_thread_local.h"
    "/home/ubuntu/ex_sharding/deps/src/mysqlclient/include/decimal.h"
    "/home/ubuntu/ex_sharding/deps/src/mysqlclient/include/errmsg.h"
    "/home/ubuntu/ex_sharding/deps/src/mysqlclient/include/my_global.h"
    "/home/ubuntu/ex_sharding/deps/src/mysqlclient/include/my_getopt.h"
    "/home/ubuntu/ex_sharding/deps/src/mysqlclient/include/sslopt-longopts.h"
    "/home/ubuntu/ex_sharding/deps/src/mysqlclient/include/my_dir.h"
    "/home/ubuntu/ex_sharding/deps/src/mysqlclient/include/sslopt-vars.h"
    "/home/ubuntu/ex_sharding/deps/src/mysqlclient/include/sslopt-case.h"
    "/home/ubuntu/ex_sharding/deps/src/mysqlclient/include/sql_common.h"
    "/home/ubuntu/ex_sharding/deps/src/mysqlclient/include/keycache.h"
    "/home/ubuntu/ex_sharding/deps/src/mysqlclient/include/m_ctype.h"
    "/home/ubuntu/ex_sharding/deps/src/mysqlclient/include/my_compiler.h"
    "/home/ubuntu/ex_sharding/deps/src/mysqlclient/include/mysql_com_server.h"
    "/home/ubuntu/ex_sharding/deps/src/mysqlclient/include/my_byteorder.h"
    "/home/ubuntu/ex_sharding/deps/src/mysqlclient/include/byte_order_generic.h"
    "/home/ubuntu/ex_sharding/deps/src/mysqlclient/include/byte_order_generic_x86.h"
    "/home/ubuntu/ex_sharding/deps/src/mysqlclient/include/little_endian.h"
    "/home/ubuntu/ex_sharding/deps/src/mysqlclient/include/big_endian.h"
    "/home/ubuntu/ex_sharding/deps/src/mysqlclient/include/thr_cond.h"
    "/home/ubuntu/ex_sharding/deps/src/mysqlclient/include/thr_mutex.h"
    "/home/ubuntu/ex_sharding/deps/src/mysqlclient/include/thr_rwlock.h"
    "/home/ubuntu/ex_sharding/deps/src/mysqlclient/include/mysql_version.h"
    "/home/ubuntu/ex_sharding/deps/src/mysqlclient/include/my_config.h"
    "/home/ubuntu/ex_sharding/deps/src/mysqlclient/include/mysqld_ername.h"
    "/home/ubuntu/ex_sharding/deps/src/mysqlclient/include/mysqld_error.h"
    "/home/ubuntu/ex_sharding/deps/src/mysqlclient/include/sql_state.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xDevelopmentx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/mysql" TYPE DIRECTORY FILES "/home/ubuntu/ex_sharding/deps/src/mysqlclient/include/mysql/" REGEX "/[^/]*\\.h$" REGEX "/psi\\_abi[^/]*$" EXCLUDE)
endif()

