################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../tools/procedurals/hairgen/emitter.cpp \
../tools/procedurals/hairgen/hair.cpp \
../tools/procedurals/hairgen/parenthairs.cpp 

OBJS += \
./tools/procedurals/hairgen/emitter.o \
./tools/procedurals/hairgen/hair.o \
./tools/procedurals/hairgen/parenthairs.o 

CPP_DEPS += \
./tools/procedurals/hairgen/emitter.d \
./tools/procedurals/hairgen/hair.d \
./tools/procedurals/hairgen/parenthairs.d 


# Each subdirectory must supply rules for building sources it contributes
tools/procedurals/hairgen/%.o: ../tools/procedurals/hairgen/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


