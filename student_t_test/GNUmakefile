#===============================================================================
# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT
#===============================================================================

# Makefile for GNU Make

default: run

all: run

run: t_test
	./t_test

MKL_COPTS = -DMKL_ILP64  -qmkl-ilp64 -qmkl-sycl-impl="stats,rng"

DPCPP_OPTS = $(MKL_COPTS) -fsycl-device-code-split=per_kernel -fno-sycl-early-optimizations

t_test: t_test.cpp
	icpx $< -fsycl -o $@ $(DPCPP_OPTS)

clean:
	-rm -f t_test

.PHONY: clean run all
