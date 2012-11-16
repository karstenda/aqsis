################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../libs/util/posix/execute_system.cpp \
../libs/util/posix/socket_system.cpp 

OBJS += \
./libs/util/posix/execute_system.o \
./libs/util/posix/socket_system.o 

CPP_DEPS += \
./libs/util/posix/execute_system.d \
./libs/util/posix/socket_system.d 


# Each subdirectory must supply rules for building sources it contributes
libs/util/posix/%.o: ../libs/util/posix/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


