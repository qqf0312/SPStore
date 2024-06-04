message(STATUS "downloading...
     src='http://dl.bintray.com/boostorg/release/1.68.0/source/boost_1_68_0.tar.bz2;/home/qqf/shard/boost_1_68_0.tar.bz2'
     dst='/home/qqf/test_spchain/ex_sharding/deps/src/boost_1_68_0.tar.bz2'
     timeout='none'")




file(DOWNLOAD
  "http://dl.bintray.com/boostorg/release/1.68.0/source/boost_1_68_0.tar.bz2;/home/qqf/shard/boost_1_68_0.tar.bz2"
  "/home/qqf/test_spchain/ex_sharding/deps/src/boost_1_68_0.tar.bz2"
  
  # no TIMEOUT
  STATUS status
  LOG log)

list(GET status 0 status_code)
list(GET status 1 status_string)

if(NOT status_code EQUAL 0)
  message(FATAL_ERROR "error: downloading 'http://dl.bintray.com/boostorg/release/1.68.0/source/boost_1_68_0.tar.bz2;/home/qqf/shard/boost_1_68_0.tar.bz2' failed
  status_code: ${status_code}
  status_string: ${status_string}
  log: ${log}
")
endif()

message(STATUS "downloading... done")
