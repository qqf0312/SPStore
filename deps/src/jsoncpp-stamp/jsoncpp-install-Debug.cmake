

set(command "${make};install")
execute_process(
  COMMAND ${command}
  RESULT_VARIABLE result
  OUTPUT_FILE "/home/ubuntu/ex_sharding/deps/src/jsoncpp-stamp/jsoncpp-install-out.log"
  ERROR_FILE "/home/ubuntu/ex_sharding/deps/src/jsoncpp-stamp/jsoncpp-install-err.log"
  )
if(result)
  set(msg "Command failed: ${result}\n")
  foreach(arg IN LISTS command)
    set(msg "${msg} '${arg}'")
  endforeach()
  set(msg "${msg}\nSee also\n  /home/ubuntu/ex_sharding/deps/src/jsoncpp-stamp/jsoncpp-install-*.log")
  message(FATAL_ERROR "${msg}")
else()
  set(msg "jsoncpp install command succeeded.  See also /home/ubuntu/ex_sharding/deps/src/jsoncpp-stamp/jsoncpp-install-*.log")
  message(STATUS "${msg}")
endif()
