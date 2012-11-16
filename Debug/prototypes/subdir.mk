################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../prototypes/genkeywords.cpp 

OBJS += \
./prototypes/genkeywords.o 

CPP_DEPS += \
./prototypes/genkeywords.d 


# Each subdirectory must supply rules for building sources it contributes
prototypes/%.o: ../prototypes/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


