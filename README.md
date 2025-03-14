# GenVectorX

Extented GenVector library for multi-target execution. 

# cmake configuration

## CUDA
A basic CUDA configuration looks like this:
```
cmake .. -Dcuda=ON -DCMAKE_CUDA_HOST_COMPILER=/usr/bin/g++ -DCMAKE_CUDA_COMPILER=/path/to/cuda/bin/nvcc -DCMAKE_CUDA_ARCHITECTURES=xx -DCMAKE_CUDA_FLAGS="-arch=sm_XX"  
```
where `XX` indicates the value of target CUDA capability.

## HIP
A basic HIP configuration looks like this:
```
cmake .. -Dhip=ON  
```
## Alpaka
A basic alpaka configuration looks like this:
```
cmake .. -Dalpaka=ON -Dalpakacpu=ON -Dalpaka_DIR=/home/mdessole/Projects/alpaka/build/lib/cmake/alpaka 
```
In order to enable alpaka CUDA, set: `-Dalpakacuda=ON`.
In order to enable alpaka HIP, set: `-Dalpakahip=ON`

## SYCL

### Memory management
SYCL offers different memory management strategies. Here, we implement buffers+accessors or device pointers. Default memory management is handled with device pointers. In order to enable buffers+accessors, it is sufficient to set `-Dsycl_buffers=ON` in the cmake configuration. 

### AdaptiveCpp
A basic SYCL configuration with AdaptiveCpp looks like this:
```
cmake .. -Dadaptivecpp=ON -DAdaptiveCpp_DIR=/path/to/AdaptiveCpp/install/lib/cmake/AdaptiveCpp -DACPP_TARGETS="<targets>"  
```

AdaptiveCpp targets specification defines which compilation flows AdaptiveCpp should enable, and which devices from a compilation flow AdaptiveCpp should target during compilation. A CUDA example lookd like this: `-DACPP_TARGETS="cuda:sm_86"`. A HIP exaple looks like this: `-DACPP_TARGETS="hip:gfx90a"`

### oneAPI
A basic SYCL configuration with oneAPI targeting CUDA backends looks like this:
```
cmake ..  -Doneapi=ON -Dsyclcuda=ON -DCMAKE_CUDA_ARCHITECTURES=XX -DCUDA_TOOLKIT_ROOT_DIR=/path/to/cuda/
```
where `XX` indicates the value of target CUDA capability. 

A basic SYCL configuration with oneAPI targeting HIP backends looks like this:
```
cmake ..  -Doneapi=ON -Dsyclamd=ON -DCMAKE_OFFLOAD_ARCHITECTURES=gfxXXX 
```
where `XXX` is set accordingly to the AMD GPU model.

## Testing
In order to compile test targets, set `-Dtesting=ON` in the cmake configuration. Set `-Dsingle_precision=ON` for compiling single precision test targets.

In order to enable time measurements (and their print to stdout), set `-Dtiming=ON` in the cmake configuration.
