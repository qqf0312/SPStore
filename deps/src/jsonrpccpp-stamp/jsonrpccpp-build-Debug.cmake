

set(command "/usr/bin/cmake;--build;/home/ubuntu/ex_sharding/deps/src/jsonrpccpp-build;--config;Release")
execute_process(
  COMMAND ${command}
  RESULT_VARIABLE result
  OUTPUT_FILE "/home/ubuntu/ex_sharding/deps/src/jsonrpccpp-stamp/jsonrpccpp-build-out.log"
  ERROR_FILE "/home/ubuntu/ex_sharding/deps/src/jsonrpccpp-stamp/jsonrpccpp-build-err.log"
  )
if(result)
  set(msg "Command failed: ${result}\n")
  foreach(arg IN LISTS command)
    set(msg "${msg} '${arg}'")
  endforeach()
  set(msg "${msg}\nSee also\n  /home/ubuntu/ex_sharding/deps/src/jsonrpccpp-stamp/jsonrpccpp-build-*.log")
  message(FATAL_ERROR "${msg}")
else()
  set(msg "jsonrpccpp build command succeeded.  See also /home/ubuntu/ex_sharding/deps/src/jsonrpccpp-stamp/jsonrpccpp-build-*.log")
  message(STATUS "${msg}")
endif()
