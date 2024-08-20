


#ifndef __CUDACC__

#define CUDA_HOST
#define CUDA_DEVICE
#define CUDA_BOTH
#define CU_FLT_CST(x) x
typedef double cu_double;

#else

#define checkCudaErrors(val) check_cuda( (val), #val, __FILE__, __LINE__ )
void check_cuda(cudaError_t result, char const *const func, const char *const file, int const line) {
    if (result) {
        std::cerr << "CUDA error = " << static_cast<unsigned int>(result) << " at " <<
        file << ":" << line << " '" << func << "' \n";
        // Make sure we call CUDA Device Reset before exiting
        cudaDeviceReset();
        exit(99);
    }
}



#define CUDA_HOST __host__
#define CUDA_DEVICE __device__
#define CUDA_BOTH CUDA_HOST CUDA_DEVICE
// Float much faster than double on loads of gpus
typedef float cu_double;

// fmin (from std) to call cuda min __device__ function
#define fmin(a,b) min(a,b)

#define CU_FLT_CST(x) x##f // 120.0 -> 120.0f



#endif