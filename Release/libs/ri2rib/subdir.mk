################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../libs/ri2rib/ri2rib.cpp \
../libs/ri2rib/ri2rib_test.cpp 

OBJS += \
./libs/ri2rib/ri2rib.o \
./libs/ri2rib/ri2rib_test.o 

CPP_DEPS += \
./libs/ri2rib/ri2rib.d \
./libs/ri2rib/ri2rib_test.d 


# Each subdirectory must supply rules for building sources it contributes
libs/ri2rib/%.o: ../libs/ri2rib/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


