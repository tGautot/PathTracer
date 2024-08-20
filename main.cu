#include <iostream>
#include <cuda_runtime.h>
#include "headers/vec3.h"
#include "headers/ray.h"
#include "headers/cuda.h"
#include <curand_kernel.h>


__device__ vec3 color(const ray& r) {
   vec3 unit_direction = r.direction().normalized();
   float t = 0.5f*(unit_direction.y() + 1.0f);
   return ((1.0f-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0)) * 255.0f;
}

__global__ 
void render(int szx, int szy, vec3 *colors, vec3 lower_left, vec3 du, vec3 dv){
    int stride_x = blockDim.x, stride_y = blockDim.y;

    int pixid;
    for(int i = threadIdx.x; i < szx; i+= stride_x ){
        for(int j = threadIdx.y; j < szy; j+= stride_y ){
            ray r = ray(vec3(0,0,0), lower_left+i*du+j*dv);
            pixid = (j*szx+i);
            colors[pixid] = color(r);
        }
    }

}

int main(){
    int nx = 600, ny = 600;
    int num_pix = nx*ny;

    size_t color_arr_size = num_pix*sizeof(vec3);
    vec3 *colors;
    checkCudaErrors(cudaMallocManaged((void**)&colors, color_arr_size));
    
    dim3 threadDim(8,8);
    render<<<1,threadDim>>>(nx,ny,colors, vec3(-300, -300, -10), vec3(1,0,0), vec3(0,1,0));
    checkCudaErrors(cudaGetLastError());
    checkCudaErrors(cudaDeviceSynchronize());

    std::cout << "P3\n" << nx << " " << ny << "\n255\n";
    for (int j = ny-1; j >= 0; j--) {
        for (int i = 0; i < nx; i++) {
            size_t pixid = j*nx + i;
            
            std::cout << (int) colors[pixid].x() << " " << (int) colors[pixid+1].y() << " " << (int) colors[pixid+2].z() << "\n";
        }
    }
    checkCudaErrors(cudaFree(colors));
}