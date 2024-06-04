# Install script for directory: /home/ubuntu/ex_sharding/deps/src/cryptopp

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/ubuntu/ex_sharding/deps")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/ubuntu/ex_sharding/deps/src/cryptopp-build/libcryptopp.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/cryptopp" TYPE FILE FILES
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/3way.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/adler32.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/aes.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/algebra.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/algparam.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/arc4.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/argnames.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/asn.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/authenc.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/base32.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/base64.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/basecode.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/bench.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/blake2.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/blowfish.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/blumshub.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/camellia.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/cast.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/cbcmac.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/ccm.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/chacha.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/channels.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/cmac.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/config.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/cpu.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/crc.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/cryptlib.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/default.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/des.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/dh.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/dh2.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/dll.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/dmac.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/drbg.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/dsa.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/eax.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/ec2n.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/eccrypto.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/ecp.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/ecpoint.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/elgamal.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/emsa2.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/eprecomp.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/esign.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/factory.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/fhmqv.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/files.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/filters.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/fips140.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/fltrimpl.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/gcm.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/gf256.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/gf2_32.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/gf2n.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/gfpcrypt.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/gost.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/gzip.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/hex.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/hkdf.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/hmac.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/hmqv.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/hrtimer.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/ida.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/idea.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/integer.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/iterhash.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/keccak.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/lubyrack.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/luc.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/mars.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/md2.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/md4.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/md5.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/mdc.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/mersenne.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/misc.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/modarith.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/modes.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/modexppc.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/mqueue.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/mqv.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/nbtheory.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/network.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/nr.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/oaep.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/oids.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/osrng.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/ossig.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/panama.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/pch.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/pkcspad.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/poly1305.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/polynomi.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/pssr.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/pubkey.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/pwdbased.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/queue.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/rabin.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/randpool.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/rc2.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/rc5.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/rc6.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/rdrand.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/resource.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/rijndael.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/ripemd.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/rng.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/rsa.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/rw.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/safer.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/salsa.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/seal.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/secblock.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/seckey.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/seed.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/serpent.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/serpentp.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/sha.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/sha3.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/shacal2.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/shark.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/simple.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/siphash.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/skipjack.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/smartptr.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/socketft.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/sosemanuk.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/square.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/stdcpp.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/strciphr.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/tea.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/tiger.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/trap.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/trdlocal.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/trunhash.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/ttmac.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/twofish.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/validate.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/vmac.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/wait.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/wake.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/whrlpool.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/winpipes.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/words.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/xtr.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/xtrcrypt.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/zdeflate.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/zinflate.h"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/zlib.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/cryptopp" TYPE FILE FILES
    "/home/ubuntu/ex_sharding/deps/src/cryptopp/cryptopp-config.cmake"
    "/home/ubuntu/ex_sharding/deps/src/cryptopp-build/cryptopp-config-version.cmake"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/cryptopp/cryptopp-targets.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/cryptopp/cryptopp-targets.cmake"
         "/home/ubuntu/ex_sharding/deps/src/cryptopp-build/CMakeFiles/Export/lib/cmake/cryptopp/cryptopp-targets.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/cryptopp/cryptopp-targets-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/cryptopp/cryptopp-targets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/cryptopp" TYPE FILE FILES "/home/ubuntu/ex_sharding/deps/src/cryptopp-build/CMakeFiles/Export/lib/cmake/cryptopp/cryptopp-targets.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/cryptopp" TYPE FILE FILES "/home/ubuntu/ex_sharding/deps/src/cryptopp-build/CMakeFiles/Export/lib/cmake/cryptopp/cryptopp-targets-release.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/ubuntu/ex_sharding/deps/src/cryptopp-build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
