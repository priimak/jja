all: bin/1jja_c bin/jja_cpp_time_eval

#NVCC            ?= $(CUDA_BIN_PATH)/nvcc
GCC             ?= gcc
GPP             ?= g++

.PHONY: clean check-nvcc

#CUDA_PATH       ?= /usr/local/cuda-5.5
#CUDA_INC_PATH   ?= $(CUDA_PATH)/include
#CUDA_BIN_PATH   ?= $(CUDA_PATH)/bin
#CUDA_LIB_PATH   ?= $(CUDA_PATH)/lib64
#NVCC            ?= $(CUDA_BIN_PATH)/nvcc

#GENCODE_FLAGS := -gencode arch=compute_30,code=sm_30 -gencode arch=compute_35,code=sm_35

#NVCCFLAGS := -m64
CCFLAGS   := -m64
LDFLAGS   := -L$(CUDA_LIB_PATH) -lcudart -lgsl -lgslcblas
LDCFLAGS  := 
EXTRA_LDFLAGS := -lm

# check-nvcc:
# ifeq (,$(wildcard $(NVCC)))
#     $(error Unable to find $(NVCC). You need to specify env. variable CUDA_PATH)
# endif

bin/1jja_c: src/1jja_c_solver.c src/jja_cli.c
	$(GCC) $(CCFLAGS) -O3 -std=gnu99 -o $@ $+ $(LDCFLAGS) $(EXTRA_LDFLAGS)

bin/jja_cpp_time_eval: src/singleJJinresonatorabsorpTimeEval.cpp
	$(GPP) $+ -o $@

clean: 
	find . -type f -name '*~' -exec rm -f {} \; && rm -f bin/*jja*

