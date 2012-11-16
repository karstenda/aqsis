################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../tools/aqsl/aqsl.cpp 

OBJS += \
./tools/aqsl/aqsl.o 

CPP_DEPS += \
./tools/aqsl/aqsl.d 


# Each subdirectory must supply rules for building sources it contributes
tools/aqsl/%.o: ../tools/aqsl/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


