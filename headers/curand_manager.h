#ifndef CURAND_MANAGER_H
#define CURAND_MANAGER_H

#include "cuda.h"

#ifdef __CUDACC__
#include <curand_kernel.h>


static currandState *states;

__device__ void setup_states(int stt_count){
    check
}


#endif
#endif