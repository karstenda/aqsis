################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../prototypes/float_array_view/vec_array_view.cpp 

OBJS += \
./prototypes/float_array_view/vec_array_view.o 

CPP_DEPS += \
./prototypes/float_array_view/vec_array_view.d 


# Each subdirectory must supply rules for building sources it contributes
prototypes/float_array_view/%.o: ../prototypes/float_array_view/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


