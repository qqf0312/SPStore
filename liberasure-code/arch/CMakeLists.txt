set(arch_srcs
  probe.cc)

list(APPEND arch_srcs intel.c)
add_library(arch STATIC ${arch_srcs})
