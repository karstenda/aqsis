################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../libs/core/attributes.cpp \
../libs/core/bilinear_test.cpp \
../libs/core/bound.cpp \
../libs/core/bucket.cpp \
../libs/core/bucketprocessor.cpp \
../libs/core/csgtree.cpp \
../libs/core/filters.cpp \
../libs/core/grid.cpp \
../libs/core/imagebuffer.cpp \
../libs/core/imagepixel.cpp \
../libs/core/imagers.cpp \
../libs/core/lights.cpp \
../libs/core/micropolygon.cpp \
../libs/core/mpdump.cpp \
../libs/core/multijitter.cpp \
../libs/core/occlusion.cpp \
../libs/core/occlusion_test.cpp \
../libs/core/optioncache.cpp \
../libs/core/options.cpp \
../libs/core/parameters.cpp \
../libs/core/refcount.cpp \
../libs/core/renderer.cpp \
../libs/core/shaders.cpp \
../libs/core/stats.cpp \
../libs/core/threadscheduler.cpp \
../libs/core/transform.cpp 

OBJS += \
./libs/core/attributes.o \
./libs/core/bilinear_test.o \
./libs/core/bound.o \
./libs/core/bucket.o \
./libs/core/bucketprocessor.o \
./libs/core/csgtree.o \
./libs/core/filters.o \
./libs/core/grid.o \
./libs/core/imagebuffer.o \
./libs/core/imagepixel.o \
./libs/core/imagers.o \
./libs/core/lights.o \
./libs/core/micropolygon.o \
./libs/core/mpdump.o \
./libs/core/multijitter.o \
./libs/core/occlusion.o \
./libs/core/occlusion_test.o \
./libs/core/optioncache.o \
./libs/core/options.o \
./libs/core/parameters.o \
./libs/core/refcount.o \
./libs/core/renderer.o \
./libs/core/shaders.o \
./libs/core/stats.o \
./libs/core/threadscheduler.o \
./libs/core/transform.o 

CPP_DEPS += \
./libs/core/attributes.d \
./libs/core/bilinear_test.d \
./libs/core/bound.d \
./libs/core/bucket.d \
./libs/core/bucketprocessor.d \
./libs/core/csgtree.d \
./libs/core/filters.d \
./libs/core/grid.d \
./libs/core/imagebuffer.d \
./libs/core/imagepixel.d \
./libs/core/imagers.d \
./libs/core/lights.d \
./libs/core/micropolygon.d \
./libs/core/mpdump.d \
./libs/core/multijitter.d \
./libs/core/occlusion.d \
./libs/core/occlusion_test.d \
./libs/core/optioncache.d \
./libs/core/options.d \
./libs/core/parameters.d \
./libs/core/refcount.d \
./libs/core/renderer.d \
./libs/core/shaders.d \
./libs/core/stats.d \
./libs/core/threadscheduler.d \
./libs/core/transform.d 


# Each subdirectory must supply rules for building sources it contributes
libs/core/%.o: ../libs/core/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


