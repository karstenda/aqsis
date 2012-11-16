################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../libs/core/ddmanager/ddmanager.cpp \
../libs/core/ddmanager/debugdd.cpp 

OBJS += \
./libs/core/ddmanager/ddmanager.o \
./libs/core/ddmanager/debugdd.o 

CPP_DEPS += \
./libs/core/ddmanager/ddmanager.d \
./libs/core/ddmanager/debugdd.d 


# Each subdirectory must supply rules for building sources it contributes
libs/core/ddmanager/%.o: ../libs/core/ddmanager/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


