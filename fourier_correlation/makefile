#===============================================================================
# Copyright (C) 2020 Intel Corporation
#
# SPDX-License-Identifier: MIT
#===============================================================================

# Makefile for NMAKE

default: run_all

all: run_all

run_all: fcorr_1d.exe fcorr_2d.exe
	.\fcorr_1d 1024
	.\fcorr_2d

DPCPP_OPTS=-DMKL_ILP64 -I"%MKLROOT%\include" /Qmkl-ilp64 /Qmkl-sycl-impl="blas,dft,rng,vm" OpenCL.lib /EHsc

fcorr_1d.exe: fcorr_1d.cpp
	icx-cl -fsycl fcorr_1d.cpp /Fefcorr_1d.exe $(DPCPP_OPTS)

fcorr_2d.exe: fcorr_2d.cpp
	icx-cl -fsycl fcorr_2d.cpp /Fefcorr_2d.exe $(DPCPP_OPTS)

clean:
	del /q fcorr_1d.exe fcorr_2d.exe
	del /q fcorr_1d.exp fcorr_2d.exp

pseudo: run_all clean all
