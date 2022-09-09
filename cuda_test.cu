//comment
#include "stdio.h"
#include "cuda.h"
#include "cuda_runtime.h"
#include "opencv4/opencv2/opencv.hpp"

#define BUF 616*800


extern "C" int test(int value);
extern "C" uchar getFrameCam(uchar *Frame,uchar *Frame_2,int buffer);



__global__ void add(int a, int b, int *c){
    *c = a + b;
}

__global__ void cudatxt(uchar *frame, uchar *frame_2, uchar *imagen){
    int j = threadIdx.x + blockIdx.x * blockDim.x;
    if(j < BUF){
        //imagen[j] = pow(frame[j]-frame_2[j],2);
        imagen[j] = frame[j]*frame_2[j];
    }
}



int test(int value){
    printf("Hello %d \n", value);
    int c;
    int *dev_c;
    cudaMalloc((void**)&dev_c,sizeof(int));
    add<<<1,1>>>(10,23,dev_c);
    cudaMemcpy(&c,dev_c,sizeof(int),cudaMemcpyDeviceToHost);
    printf("2+7 = %d\n",c);
    cudaFree(dev_c);
    return value;
}

uchar getFrameCam(uchar *Frame,uchar *Frame_2 ,int buffer){
    cv::Mat mat;
    uchar * dev_frame;
    uchar * dev_frame_2;
    uchar * dev_img;
    uchar imagen[616][800];
    uchar imbuff[buffer];
    cudaMalloc((void**)&dev_frame, buffer*sizeof(uchar));
    cudaMalloc((void**)&dev_frame_2, buffer*sizeof(uchar));
    cudaMalloc((void**)&dev_img, buffer*sizeof(uchar));
    cudaMemcpy(dev_frame,Frame,buffer*sizeof(uchar),cudaMemcpyHostToDevice);
    cudaMemcpy(dev_frame_2,Frame_2,buffer*sizeof(uchar),cudaMemcpyHostToDevice);
    cudaMemcpy(dev_img,imagen,buffer*sizeof(uchar),cudaMemcpyHostToDevice);
    cudatxt<<<800,616>>>(dev_frame, dev_frame_2, dev_img);
    cudaMemcpy(imagen,dev_img,buffer*sizeof(uchar),cudaMemcpyDeviceToHost);
    mat = cv::Mat(616, 800, CV_8UC1, imagen);
    cv::imwrite("/home/nvxv/Pictures/out/sumaImagenes.bmp",mat);
    cudaFree(dev_frame);
    cudaFree(dev_frame_2);
    cudaFree(dev_img);
    for(int i = 0; i<616; i++){
        for(int j = 0; j<800; j++){
            imbuff[j+(i*j)] = imagen[i][j];
        }
    }
    return imbuff[buffer];
}
