

set(command "bash;-c;cp ./build/*_release/*.a* /home/ubuntu/ex_sharding/deps/lib")
execute_process(
  COMMAND ${command}
  RESULT_VARIABLE result
  OUTPUT_FILE "/home/ubuntu/ex_sharding/deps/src/tbb-stamp/tbb-install-out.log"
  ERROR_FILE "/home/ubuntu/ex_sharding/deps/src/tbb-stamp/tbb-install-err.log"
  )
if(result)
  set(msg "Command failed: ${result}\n")
  foreach(arg IN LISTS command)
    set(msg "${msg} '${arg}'")
  endforeach()
  set(msg "${msg}\nSee also\n  /home/ubuntu/ex_sharding/deps/src/tbb-stamp/tbb-install-*.log")
  message(FATAL_ERROR "${msg}")
else()
  set(msg "tbb install command succeeded.  See also /home/ubuntu/ex_sharding/deps/src/tbb-stamp/tbb-install-*.log")
  message(STATUS "${msg}")
endif()
