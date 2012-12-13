################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../libs/pointrender/MicroBuf.cpp \
../libs/pointrender/OcclusionIntegrator.cpp \
../libs/pointrender/PointOctree.cpp \
../libs/pointrender/PointOctreeCache.cpp \
../libs/pointrender/RadiosityIntegrator.cpp \
../libs/pointrender/microbuf_proj_func.cpp 

OBJS += \
./libs/pointrender/MicroBuf.o \
./libs/pointrender/OcclusionIntegrator.o \
./libs/pointrender/PointOctree.o \
./libs/pointrender/PointOctreeCache.o \
./libs/pointrender/RadiosityIntegrator.o \
./libs/pointrender/microbuf_proj_func.o 

CPP_DEPS += \
./libs/pointrender/MicroBuf.d \
./libs/pointrender/OcclusionIntegrator.d \
./libs/pointrender/PointOctree.d \
./libs/pointrender/PointOctreeCache.d \
./libs/pointrender/RadiosityIntegrator.d \
./libs/pointrender/microbuf_proj_func.d 


# Each subdirectory must supply rules for building sources it contributes
libs/pointrender/%.o: ../libs/pointrender/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


