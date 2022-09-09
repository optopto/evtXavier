#include "mainxavierevt.h"

void gpuInfo();

void gpuInfo(){
    cudaDeviceProp prop;
    cudaGetDeviceProperties(&prop,0);
    printf("START >>> GPU INFORMATION\n");
    printf("Name: %s \n",prop.name);
    printf("Compute capability %d.%d \n",prop.major,prop.minor);
    printf("Clock Rate %d \n",prop.clockRate);
    printf("Device copy overlap: ");
    if(prop.deviceOverlap)
        printf("Enabled \n");
    else
        printf("Disabled \n");
    printf("Kerel Execution time: ");
    if(prop.kernelExecTimeoutEnabled)
        printf("Enabled \n");
    else
        printf("Disabled \n");
    printf("Total global memory: %ld \n",prop.totalGlobalMem);
    printf("Total constant memory: %ld \n",prop.totalConstMem);
    printf("Max memory pitch: %ld \n",prop.memPitch);
    printf("Texture alignment: %ld \n",prop.textureAlignment);
    printf("Multiprocessor count: %d \n",prop.multiProcessorCount);
    printf("Shared memory per block: %ld \n",prop.sharedMemPerBlock);
    printf("Registers per block %d \n",prop.regsPerBlock);
    printf("Threads in warp: %d \n",prop.warpSize);
    printf("Max threads per block: %d \n",prop.maxThreadsPerBlock);
    printf("Max threads dimensions: (%d,%d,%d) \n",prop.maxThreadsDim[0],prop.maxThreadsDim[1],prop.maxThreadsDim[2]);
    printf("Max grids dimensions: (%d,%d,%d) \n",prop.maxGridSize[0],prop.maxGridSize[1],prop.maxGridSize[2]);
    printf("END >>> GPU INFORMATION\n");
    return;
}
