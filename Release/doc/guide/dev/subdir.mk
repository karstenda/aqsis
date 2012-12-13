################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../doc/guide/dev/codestyle.cpp 

OBJS += \
./doc/guide/dev/codestyle.o 

CPP_DEPS += \
./doc/guide/dev/codestyle.d 


# Each subdirectory must supply rules for building sources it contributes
doc/guide/dev/%.o: ../doc/guide/dev/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


