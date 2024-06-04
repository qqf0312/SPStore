

set(command "make;install")
execute_process(
  COMMAND ${command}
  RESULT_VARIABLE result
  OUTPUT_FILE "/home/ubuntu/ex_sharding/deps//src/gperftools-stamp/gperftools-build-out.log"
  ERROR_FILE "/home/ubuntu/ex_sharding/deps//src/gperftools-stamp/gperftools-build-err.log"
  )
if(result)
  set(msg "Command failed: ${result}\n")
  foreach(arg IN LISTS command)
    set(msg "${msg} '${arg}'")
  endforeach()
  set(msg "${msg}\nSee also\n  /home/ubuntu/ex_sharding/deps//src/gperftools-stamp/gperftools-build-*.log")
  message(FATAL_ERROR "${msg}")
else()
  set(msg "gperftools build command succeeded.  See also /home/ubuntu/ex_sharding/deps//src/gperftools-stamp/gperftools-build-*.log")
  message(STATUS "${msg}")
endif()