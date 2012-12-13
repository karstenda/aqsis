################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../libs/core/raytrace/raytrace.cpp 

OBJS += \
./libs/core/raytrace/raytrace.o 

CPP_DEPS += \
./libs/core/raytrace/raytrace.d 


# Each subdirectory must supply rules for building sources it contributes
libs/core/raytrace/%.o: ../libs/core/raytrace/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


