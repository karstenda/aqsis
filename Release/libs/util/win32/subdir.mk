################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../libs/util/win32/execute_system.cpp \
../libs/util/win32/socket_system.cpp 

OBJS += \
./libs/util/win32/execute_system.o \
./libs/util/win32/socket_system.o 

CPP_DEPS += \
./libs/util/win32/execute_system.d \
./libs/util/win32/socket_system.d 


# Each subdirectory must supply rules for building sources it contributes
libs/util/win32/%.o: ../libs/util/win32/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


