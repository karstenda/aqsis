################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../prototypes/shaderVM_codegen/codegen.cpp 

OBJS += \
./prototypes/shaderVM_codegen/codegen.o 

CPP_DEPS += \
./prototypes/shaderVM_codegen/codegen.d 


# Each subdirectory must supply rules for building sources it contributes
prototypes/shaderVM_codegen/%.o: ../prototypes/shaderVM_codegen/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


