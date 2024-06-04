

set(command "/usr/bin/cmake;-DCMAKE_INSTALL_PREFIX=/home/ubuntu/ex_sharding/deps;-DCMAKE_POSITION_INDEPENDENT_CODE=on;-DWITH_LZ4=off;-DWITH_SNAPPY=on;-DWITH_GFLAGS=off;-DWITH_TESTS=off;-DWITH_TOOLS=off;-DBUILD_SHARED_LIBS=Off;-DUSE_RTTI=on;-DCMAKE_C_FLAGS=-pthread;-march=x86-64 -mtune=generic -fvisibility=hidden -fvisibility-inlines-hidden;-DCMAKE_CXX_FLAGS= -no-pie -std=c++14  -O0 -Wall -g2 -ggdb -Wtype-limits -Wsign-compare -Wno-dev -pthread;-march=x86-64 -mtune=generic -fvisibility=hidden -fvisibility-inlines-hidden;-DCMAKE_C_COMPILER=/usr/bin/cc;-DCMAKE_CXX_COMPILER=/usr/bin/c++;-GUnix Makefiles;/home/ubuntu/ex_sharding/deps/src/rocksdb")
execute_process(
  COMMAND ${command}
  RESULT_VARIABLE result
  OUTPUT_FILE "/home/ubuntu/ex_sharding/deps/src/rocksdb-stamp/rocksdb-configure-out.log"
  ERROR_FILE "/home/ubuntu/ex_sharding/deps/src/rocksdb-stamp/rocksdb-configure-err.log"
  )
if(result)
  set(msg "Command failed: ${result}\n")
  foreach(arg IN LISTS command)
    set(msg "${msg} '${arg}'")
  endforeach()
  set(msg "${msg}\nSee also\n  /home/ubuntu/ex_sharding/deps/src/rocksdb-stamp/rocksdb-configure-*.log")
  message(FATAL_ERROR "${msg}")
else()
  set(msg "rocksdb configure command succeeded.  See also /home/ubuntu/ex_sharding/deps/src/rocksdb-stamp/rocksdb-configure-*.log")
  message(STATUS "${msg}")
endif()
