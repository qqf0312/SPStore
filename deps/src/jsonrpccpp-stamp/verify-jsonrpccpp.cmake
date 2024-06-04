# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

if("/home/qqf/zcy/jsonrpccpp-0.7.0.tar.gz" STREQUAL "")
  message(FATAL_ERROR "LOCAL can't be empty")
endif()

if(NOT EXISTS "/home/qqf/zcy/jsonrpccpp-0.7.0.tar.gz")
  message(FATAL_ERROR "File not found: /home/qqf/zcy/jsonrpccpp-0.7.0.tar.gz")
endif()

if("SHA256" STREQUAL "")
  message(WARNING "File will not be verified since no URL_HASH specified")
  return()
endif()

if("669c2259909f11a8c196923a910f9a16a8225ecc14e6c30e2bcb712bab9097eb" STREQUAL "")
  message(FATAL_ERROR "EXPECT_VALUE can't be empty")
endif()

message(STATUS "verifying file...
     file='/home/qqf/zcy/jsonrpccpp-0.7.0.tar.gz'")

file("SHA256" "/home/qqf/zcy/jsonrpccpp-0.7.0.tar.gz" actual_value)

if(NOT "${actual_value}" STREQUAL "669c2259909f11a8c196923a910f9a16a8225ecc14e6c30e2bcb712bab9097eb")
  message(FATAL_ERROR "error: SHA256 hash of
  /home/qqf/zcy/jsonrpccpp-0.7.0.tar.gz
does not match expected value
  expected: '669c2259909f11a8c196923a910f9a16a8225ecc14e6c30e2bcb712bab9097eb'
    actual: '${actual_value}'
")
endif()

message(STATUS "verifying file... done")
