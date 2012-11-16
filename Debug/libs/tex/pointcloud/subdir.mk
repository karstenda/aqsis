################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../libs/tex/pointcloud/pointcloud.cpp 

OBJS += \
./libs/tex/pointcloud/pointcloud.o 

CPP_DEPS += \
./libs/tex/pointcloud/pointcloud.d 


# Each subdirectory must supply rules for building sources it contributes
libs/tex/pointcloud/%.o: ../libs/tex/pointcloud/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


