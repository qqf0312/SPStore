

set(command "/usr/bin/cmake;-DCMAKE_INSTALL_PREFIX=/home/ubuntu/ex_sharding/deps;-DCMAKE_BUILD_TYPE=Release;-DCMAKE_POSITION_INDEPENDENT_CODE=;-DBUILD_SHARED=Off;-DBUILD_TESTING=Off;-DCMAKE_C_FLAGS=-march=x86-64 -mtune=generic -fvisibility=hidden -fvisibility-inlines-hidden;-DCMAKE_CXX_FLAGS=-march=x86-64 -mtune=generic -fvisibility=hidden -fvisibility-inlines-hidden;-DCMAKE_C_COMPILER=/usr/bin/cc;-DCMAKE_CXX_COMPILER=/usr/bin/c++;-GUnix Makefiles;/home/ubuntu/ex_sharding/deps/src/cryptopp")
execute_process(
  COMMAND ${command}
  RESULT_VARIABLE result
  OUTPUT_FILE "/home/ubuntu/ex_sharding/deps/src/cryptopp-stamp/cryptopp-configure-out.log"
  ERROR_FILE "/home/ubuntu/ex_sharding/deps/src/cryptopp-stamp/cryptopp-configure-err.log"
  )
if(result)
  set(msg "Command failed: ${result}\n")
  foreach(arg IN LISTS command)
    set(msg "${msg} '${arg}'")
  endforeach()
  set(msg "${msg}\nSee also\n  /home/ubuntu/ex_sharding/deps/src/cryptopp-stamp/cryptopp-configure-*.log")
  message(FATAL_ERROR "${msg}")
else()
  set(msg "cryptopp configure command succeeded.  See also /home/ubuntu/ex_sharding/deps/src/cryptopp-stamp/cryptopp-configure-*.log")
  message(STATUS "${msg}")
endif()
