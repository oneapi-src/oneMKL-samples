#===============================================================================
# Copyright (C) 2020 Intel Corporation
#
# SPDX-License-Identifier: MIT
#===============================================================================

# Makefile for GNU Make

default: run_all

all: run_all

run_all: fcorr_1d fcorr_2d
	./fcorr_1d 4096
	./fcorr_2d

DPCPP_OPTS = -DMKL_ILP64 -qmkl-ilp64 -qmkl-sycl-impl="blas,dft,rng,vm"

fcorr_1d: fcorr_1d.cpp
	icpx $< -fsycl -o $@ $(DPCPP_OPTS)

fcorr_2d: fcorr_2d.cpp
	icpx $< -fsycl -o $@ $(DPCPP_OPTS)

clean:
	-rm -f fcorr_1d fcorr_2d

.PHONY: run_all clean all
