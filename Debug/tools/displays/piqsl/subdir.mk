################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../tools/displays/piqsl/piqsldisplay.cpp 

OBJS += \
./tools/displays/piqsl/piqsldisplay.o 

CPP_DEPS += \
./tools/displays/piqsl/piqsldisplay.d 


# Each subdirectory must supply rules for building sources it contributes
tools/displays/piqsl/%.o: ../tools/displays/piqsl/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


