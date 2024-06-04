# ISA

set(isal_src_dir ${CMAKE_SOURCE_DIR}/isa-l)
include_directories(${isal_src_dir}/include)

  set(CMAKE_ASM_FLAGS "-i ${isal_src_dir}/include/ ${CMAKE_ASM_FLAGS}")
  set(isa_srcs
    ${isal_src_dir}/erasure_code/ec_base.c
    ${isal_src_dir}/erasure_code/gf_2vect_dot_prod_sse.asm
    ${isal_src_dir}/erasure_code/gf_3vect_dot_prod_sse.asm
    ${isal_src_dir}/erasure_code/gf_4vect_dot_prod_sse.asm
    ${isal_src_dir}/erasure_code/gf_5vect_dot_prod_sse.asm
    ${isal_src_dir}/erasure_code/gf_6vect_dot_prod_sse.asm
    ${isal_src_dir}/erasure_code/gf_vect_dot_prod_sse.asm
    ${isal_src_dir}/erasure_code/gf_2vect_mad_avx2.asm
    ${isal_src_dir}/erasure_code/gf_3vect_mad_avx2.asm
    ${isal_src_dir}/erasure_code/gf_4vect_mad_avx2.asm
    ${isal_src_dir}/erasure_code/gf_5vect_mad_avx2.asm
    ${isal_src_dir}/erasure_code/gf_6vect_mad_avx2.asm
    ${isal_src_dir}/erasure_code/gf_vect_mad_avx2.asm
    ${isal_src_dir}/erasure_code/ec_highlevel_func.c
    ${isal_src_dir}/erasure_code/gf_2vect_mad_avx.asm
    ${isal_src_dir}/erasure_code/gf_3vect_mad_avx.asm
    ${isal_src_dir}/erasure_code/gf_4vect_mad_avx.asm
    ${isal_src_dir}/erasure_code/gf_5vect_mad_avx.asm
    ${isal_src_dir}/erasure_code/gf_6vect_mad_avx.asm
    ${isal_src_dir}/erasure_code/gf_vect_mad_avx.asm
    ${isal_src_dir}/erasure_code/ec_multibinary.asm
    ${isal_src_dir}/erasure_code/gf_2vect_mad_sse.asm
    ${isal_src_dir}/erasure_code/gf_3vect_mad_sse.asm
    ${isal_src_dir}/erasure_code/gf_4vect_mad_sse.asm
    ${isal_src_dir}/erasure_code/gf_5vect_mad_sse.asm
    ${isal_src_dir}/erasure_code/gf_6vect_mad_sse.asm
    ${isal_src_dir}/erasure_code/gf_vect_mad_sse.asm
    ${isal_src_dir}/erasure_code/gf_2vect_dot_prod_avx2.asm
    ${isal_src_dir}/erasure_code/gf_3vect_dot_prod_avx2.asm
    ${isal_src_dir}/erasure_code/gf_4vect_dot_prod_avx2.asm
    ${isal_src_dir}/erasure_code/gf_5vect_dot_prod_avx2.asm
    ${isal_src_dir}/erasure_code/gf_6vect_dot_prod_avx2.asm
    ${isal_src_dir}/erasure_code/gf_vect_dot_prod_avx2.asm
    ${isal_src_dir}/erasure_code/gf_vect_mul_avx.asm
    ${isal_src_dir}/erasure_code/gf_2vect_dot_prod_avx.asm
    ${isal_src_dir}/erasure_code/gf_3vect_dot_prod_avx.asm
    ${isal_src_dir}/erasure_code/gf_4vect_dot_prod_avx.asm
    ${isal_src_dir}/erasure_code/gf_5vect_dot_prod_avx.asm
    ${isal_src_dir}/erasure_code/gf_6vect_dot_prod_avx.asm
    ${isal_src_dir}/erasure_code/gf_vect_dot_prod_avx.asm
    ${isal_src_dir}/erasure_code/gf_vect_mul_sse.asm
    ${isal_src_dir}/erasure_code/gf_2vect_dot_prod_avx512.asm
    ${isal_src_dir}/erasure_code/gf_2vect_mad_avx512.asm
    ${isal_src_dir}/erasure_code/gf_3vect_dot_prod_avx512.asm
    ${isal_src_dir}/erasure_code/gf_3vect_mad_avx512.asm
    ${isal_src_dir}/erasure_code/gf_4vect_dot_prod_avx512.asm
    ${isal_src_dir}/erasure_code/gf_4vect_mad_avx512.asm
    ${isal_src_dir}/erasure_code/gf_vect_dot_prod_avx512.asm
    ${isal_src_dir}/erasure_code/gf_vect_mad_avx512.asm
    ErasureCodeIsa.cc
    ErasureCodeIsaTableCache.cc
    xor_op.cc
  )

add_library(ec_isa ${isa_srcs})
SET_TARGET_PROPERTIES(ec_isa PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(ec_isa PRIVATE ${CMAKE_SOURCE_DIR})
