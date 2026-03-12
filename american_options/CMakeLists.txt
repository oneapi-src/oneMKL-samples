#===============================================================================
# Copyright (C) 2026 Intel Corporation
#
# SPDX-License-Identifier: MIT
#===============================================================================

cmake_minimum_required (VERSION 3.4.0)
set(CMAKE_CXX_COMPILER "icpx")
project (american_options)
# Set default build type to RelWithDebInfo if not specified
if (NOT CMAKE_BUILD_TYPE)
        message (STATUS "Default CMAKE_BUILD_TYPE not set using Release with Debug Info")
        set (CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE
             STRING "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel"
             FORCE)
endif ()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory (src)
