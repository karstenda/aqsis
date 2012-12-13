################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../tools/aqsltell/aqsltell.cpp 

OBJS += \
./tools/aqsltell/aqsltell.o 

CPP_DEPS += \
./tools/aqsltell/aqsltell.d 


# Each subdirectory must supply rules for building sources it contributes
tools/aqsltell/%.o: ../tools/aqsltell/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


