################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../prototypes/ri_cpp/ri_in_cpp_test.cpp 

OBJS += \
./prototypes/ri_cpp/ri_in_cpp_test.o 

CPP_DEPS += \
./prototypes/ri_cpp/ri_in_cpp_test.d 


# Each subdirectory must supply rules for building sources it contributes
prototypes/ri_cpp/%.o: ../prototypes/ri_cpp/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


