#===============================================================================
# Copyright (C) 2026 Intel Corporation
#
# SPDX-License-Identifier: MIT
#===============================================================================

if(USE_DEVICE_API)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsycl -DWITH_FUSED_BETA=1 -DUSE_DEVICE_API=1 ")
    add_executable (american_monte_carlo_device longstaff_schwartz_svd_2.dp.cpp)
    target_link_libraries(american_monte_carlo_device sycl)
    add_custom_target (run_device_api cd ${CMAKE_SOURCE_DIR}/src/ && ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/american_monte_carlo_device)
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsycl -DWITH_FUSED_BETA=1 ")
    add_executable (american_monte_carlo_host longstaff_schwartz_svd_2.dp.cpp)
    target_link_libraries(american_monte_carlo_host sycl mkl_sycl_rng mkl_intel_ilp64 mkl_sequential mkl_core)
    add_custom_target (run_host_api cd ${CMAKE_SOURCE_DIR}/src/ && ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/american_monte_carlo_host)
endif()
