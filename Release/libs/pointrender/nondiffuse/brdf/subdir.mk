################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../libs/pointrender/nondiffuse/brdf/PhongBrdf.cpp 

OBJS += \
./libs/pointrender/nondiffuse/brdf/PhongBrdf.o 

CPP_DEPS += \
./libs/pointrender/nondiffuse/brdf/PhongBrdf.d 


# Each subdirectory must supply rules for building sources it contributes
libs/pointrender/nondiffuse/brdf/%.o: ../libs/pointrender/nondiffuse/brdf/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


