# ------------------------------------------------------------------------------
# CMake Includes
# ------------------------------------------------------------------------------

include(${CMAKE_INSTALL_PREFIX}/cmake/CMakeGlobal_Includes.txt)

# ------------------------------------------------------------------------------
# Targets
# ------------------------------------------------------------------------------

macro(do_test str)
    add_executable(test_${str} test_${str}.cpp)
    target_compile_definitions(test_${str} PRIVATE STATIC_DEBUG_RING)
    target_link_libraries(test_${str} bfvmm_debug_ring_static)
    target_link_libraries(test_${str} bfvmm_catch_static)
    add_test(test_${str} test_${str})
endmacro(do_test)

do_test(debug_ring)
