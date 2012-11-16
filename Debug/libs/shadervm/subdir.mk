################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../libs/shadervm/dsoshadeops.cpp \
../libs/shadervm/shaderstack.cpp \
../libs/shadervm/shadervm.cpp \
../libs/shadervm/shadervm1.cpp \
../libs/shadervm/shadervm2.cpp 

OBJS += \
./libs/shadervm/dsoshadeops.o \
./libs/shadervm/shaderstack.o \
./libs/shadervm/shadervm.o \
./libs/shadervm/shadervm1.o \
./libs/shadervm/shadervm2.o 

CPP_DEPS += \
./libs/shadervm/dsoshadeops.d \
./libs/shadervm/shaderstack.d \
./libs/shadervm/shadervm.d \
./libs/shadervm/shadervm1.d \
./libs/shadervm/shadervm2.d 


# Each subdirectory must supply rules for building sources it contributes
libs/shadervm/%.o: ../libs/shadervm/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


