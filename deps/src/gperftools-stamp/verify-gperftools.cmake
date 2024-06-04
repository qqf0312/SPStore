# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

if("/home/qqf/zcy/gperftools-2.7.tar.gz" STREQUAL "")
  message(FATAL_ERROR "LOCAL can't be empty")
endif()

if(NOT EXISTS "/home/qqf/zcy/gperftools-2.7.tar.gz")
  message(FATAL_ERROR "File not found: /home/qqf/zcy/gperftools-2.7.tar.gz")
endif()

if("SHA256" STREQUAL "")
  message(WARNING "File will not be verified since no URL_HASH specified")
  return()
endif()

if("1ee8c8699a0eff6b6a203e59b43330536b22bbcbe6448f54c7091e5efb0763c9" STREQUAL "")
  message(FATAL_ERROR "EXPECT_VALUE can't be empty")
endif()

message(STATUS "verifying file...
     file='/home/qqf/zcy/gperftools-2.7.tar.gz'")

file("SHA256" "/home/qqf/zcy/gperftools-2.7.tar.gz" actual_value)

if(NOT "${actual_value}" STREQUAL "1ee8c8699a0eff6b6a203e59b43330536b22bbcbe6448f54c7091e5efb0763c9")
  message(FATAL_ERROR "error: SHA256 hash of
  /home/qqf/zcy/gperftools-2.7.tar.gz
does not match expected value
  expected: '1ee8c8699a0eff6b6a203e59b43330536b22bbcbe6448f54c7091e5efb0763c9'
    actual: '${actual_value}'
")
endif()

message(STATUS "verifying file... done")
