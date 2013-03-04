################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../libs/pointrender/diffuse/DiffusePointOctree.cpp \
../libs/pointrender/diffuse/DiffusePointOctreeCache.cpp 

OBJS += \
./libs/pointrender/diffuse/DiffusePointOctree.o \
./libs/pointrender/diffuse/DiffusePointOctreeCache.o 

CPP_DEPS += \
./libs/pointrender/diffuse/DiffusePointOctree.d \
./libs/pointrender/diffuse/DiffusePointOctreeCache.d 


# Each subdirectory must supply rules for building sources it contributes
libs/pointrender/diffuse/%.o: ../libs/pointrender/diffuse/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


