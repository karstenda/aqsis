################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../tools/procedurals/hairgen/kdtree/kdtree2.cpp 

OBJS += \
./tools/procedurals/hairgen/kdtree/kdtree2.o 

CPP_DEPS += \
./tools/procedurals/hairgen/kdtree/kdtree2.d 


# Each subdirectory must supply rules for building sources it contributes
tools/procedurals/hairgen/kdtree/%.o: ../tools/procedurals/hairgen/kdtree/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


