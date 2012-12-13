################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../tools/displays/exr/d_exr.cpp 

OBJS += \
./tools/displays/exr/d_exr.o 

CPP_DEPS += \
./tools/displays/exr/d_exr.d 


# Each subdirectory must supply rules for building sources it contributes
tools/displays/exr/%.o: ../tools/displays/exr/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


