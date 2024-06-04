

set(command "./configure;--with-mysql=/home/ubuntu/ex_sharding/deps/src/mysqlclient/scripts/mysql_config;--without-sqlite;--without-postgresql;--enable-shared=false;--enable-protected")
execute_process(
  COMMAND ${command}
  RESULT_VARIABLE result
  OUTPUT_FILE "/home/ubuntu/ex_sharding/deps/src/libzdb-stamp/libzdb-configure-out.log"
  ERROR_FILE "/home/ubuntu/ex_sharding/deps/src/libzdb-stamp/libzdb-configure-err.log"
  )
if(result)
  set(msg "Command failed: ${result}\n")
  foreach(arg IN LISTS command)
    set(msg "${msg} '${arg}'")
  endforeach()
  set(msg "${msg}\nSee also\n  /home/ubuntu/ex_sharding/deps/src/libzdb-stamp/libzdb-configure-*.log")
  message(FATAL_ERROR "${msg}")
else()
  set(msg "libzdb configure command succeeded.  See also /home/ubuntu/ex_sharding/deps/src/libzdb-stamp/libzdb-configure-*.log")
  message(STATUS "${msg}")
endif()
