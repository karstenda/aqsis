################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../libs/pointrender/nondiffuse/NonDiffuseSurphel.cpp 

OBJS += \
./libs/pointrender/nondiffuse/NonDiffuseSurphel.o 

CPP_DEPS += \
./libs/pointrender/nondiffuse/NonDiffuseSurphel.d 


# Each subdirectory must supply rules for building sources it contributes
libs/pointrender/nondiffuse/%.o: ../libs/pointrender/nondiffuse/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


