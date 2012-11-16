################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../tools/shadeops/sci_volumes/src/sample_vol_shadeops.cpp \
../tools/shadeops/sci_volumes/src/volumesampler.cpp 

OBJS += \
./tools/shadeops/sci_volumes/src/sample_vol_shadeops.o \
./tools/shadeops/sci_volumes/src/volumesampler.o 

CPP_DEPS += \
./tools/shadeops/sci_volumes/src/sample_vol_shadeops.d \
./tools/shadeops/sci_volumes/src/volumesampler.d 


# Each subdirectory must supply rules for building sources it contributes
tools/shadeops/sci_volumes/src/%.o: ../tools/shadeops/sci_volumes/src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


