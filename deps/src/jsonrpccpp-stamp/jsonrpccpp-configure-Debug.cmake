

set(command "/usr/bin/cmake;-DCMAKE_INSTALL_PREFIX=/home/ubuntu/ex_sharding/deps;-DCMAKE_BUILD_TYPE=Release;-DCMAKE_POSITION_INDEPENDENT_CODE=;-DBUILD_STATIC_LIBS=On;-DBUILD_SHARED_LIBS=Off;-DUNIX_DOMAIN_SOCKET_SERVER=Off;-DUNIX_DOMAIN_SOCKET_CLIENT=Off;-DHTTP_SERVER=On;-DHTTP_CLIENT=off;-DCOMPILE_TESTS=Off;-DCOMPILE_STUBGEN=Off;-DCOMPILE_EXAMPLES=Off;-DJSONCPP_INCLUDE_DIR=/home/ubuntu/ex_sharding/deps/include;-DJSONCPP_INCLUDE_PREFIX=json;-DJSONCPP_LIBRARY=/home/ubuntu/ex_sharding/deps/lib/libjsoncpp.a;-DMHD_INCLUDE_DIR=/home/ubuntu/ex_sharding/deps/src/mhd/src/include/;-DMHD_LIBRARY=/home/ubuntu/ex_sharding/deps/src/mhd/src/microhttpd/.libs/libmicrohttpd.a;-DCMAKE_C_FLAGS=-fvisibility=hidden;-fvisibility-inlines-hidden;-DCMAKE_CXX_FLAGS=-fvisibility=hidden;-fvisibility-inlines-hidden;-DCMAKE_C_COMPILER=/usr/bin/cc;-DCMAKE_CXX_COMPILER=/usr/bin/c++;-GUnix Makefiles;/home/ubuntu/ex_sharding/deps/src/jsonrpccpp")
execute_process(
  COMMAND ${command}
  RESULT_VARIABLE result
  OUTPUT_FILE "/home/ubuntu/ex_sharding/deps/src/jsonrpccpp-stamp/jsonrpccpp-configure-out.log"
  ERROR_FILE "/home/ubuntu/ex_sharding/deps/src/jsonrpccpp-stamp/jsonrpccpp-configure-err.log"
  )
if(result)
  set(msg "Command failed: ${result}\n")
  foreach(arg IN LISTS command)
    set(msg "${msg} '${arg}'")
  endforeach()
  set(msg "${msg}\nSee also\n  /home/ubuntu/ex_sharding/deps/src/jsonrpccpp-stamp/jsonrpccpp-configure-*.log")
  message(FATAL_ERROR "${msg}")
else()
  set(msg "jsonrpccpp configure command succeeded.  See also /home/ubuntu/ex_sharding/deps/src/jsonrpccpp-stamp/jsonrpccpp-configure-*.log")
  message(STATUS "${msg}")
endif()
