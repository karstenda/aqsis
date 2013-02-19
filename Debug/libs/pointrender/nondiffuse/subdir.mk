################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../libs/pointrender/nondiffuse/NonDiffusePoint.cpp \
../libs/pointrender/nondiffuse/NonDiffusePointOctree.cpp \
../libs/pointrender/nondiffuse/NonDiffusePointOctreeCache.cpp 

OBJS += \
./libs/pointrender/nondiffuse/NonDiffusePoint.o \
./libs/pointrender/nondiffuse/NonDiffusePointOctree.o \
./libs/pointrender/nondiffuse/NonDiffusePointOctreeCache.o 

CPP_DEPS += \
./libs/pointrender/nondiffuse/NonDiffusePoint.d \
./libs/pointrender/nondiffuse/NonDiffusePointOctree.d \
./libs/pointrender/nondiffuse/NonDiffusePointOctreeCache.d 


# Each subdirectory must supply rules for building sources it contributes
libs/pointrender/nondiffuse/%.o: ../libs/pointrender/nondiffuse/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


