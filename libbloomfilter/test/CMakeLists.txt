add_subdirectory(bf)

enable_testing()
find_package(Boost REQUIRED COMPONENTS serialization system filesystem)
include_directories(${Boost_INCLUDE_DIRS})
add_executable(bf-test tests.cpp)
target_link_libraries(bf-test libbf_shared ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES} ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY})
# add_test(unit ${CMAKE_BINARY_DIR}/bin/bf-test)
