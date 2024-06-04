

set(command "/usr/bin/cmake;-DMYSQL_TCP_PORT=3305;-DCMAKE_INSTALL_PREFIX=/home/ubuntu/ex_sharding/deps/;-GUnix Makefiles;/home/ubuntu/ex_sharding/deps/src/mysqlclient")
execute_process(
  COMMAND ${command}
  RESULT_VARIABLE result
  OUTPUT_FILE "/home/ubuntu/ex_sharding/deps/src/mysqlclient-stamp/mysqlclient-configure-out.log"
  ERROR_FILE "/home/ubuntu/ex_sharding/deps/src/mysqlclient-stamp/mysqlclient-configure-err.log"
  )
if(result)
  set(msg "Command failed: ${result}\n")
  foreach(arg IN LISTS command)
    set(msg "${msg} '${arg}'")
  endforeach()
  set(msg "${msg}\nSee also\n  /home/ubuntu/ex_sharding/deps/src/mysqlclient-stamp/mysqlclient-configure-*.log")
  message(FATAL_ERROR "${msg}")
else()
  set(msg "mysqlclient configure command succeeded.  See also /home/ubuntu/ex_sharding/deps/src/mysqlclient-stamp/mysqlclient-configure-*.log")
  message(STATUS "${msg}")
endif()
