################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../tools/miqser/miqser.cpp 

OBJS += \
./tools/miqser/miqser.o 

CPP_DEPS += \
./tools/miqser/miqser.d 


# Each subdirectory must supply rules for building sources it contributes
tools/miqser/%.o: ../tools/miqser/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


