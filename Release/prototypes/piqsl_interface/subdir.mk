################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../prototypes/piqsl_interface/main.cpp 

OBJS += \
./prototypes/piqsl_interface/main.o 

CPP_DEPS += \
./prototypes/piqsl_interface/main.d 


# Each subdirectory must supply rules for building sources it contributes
prototypes/piqsl_interface/%.o: ../prototypes/piqsl_interface/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


