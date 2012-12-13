################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../examples/procedurals/menger/menger.cpp 

OBJS += \
./examples/procedurals/menger/menger.o 

CPP_DEPS += \
./examples/procedurals/menger/menger.d 


# Each subdirectory must supply rules for building sources it contributes
examples/procedurals/menger/%.o: ../examples/procedurals/menger/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


