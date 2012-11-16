################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../prototypes/allocators/region_allocator.cpp \
../prototypes/allocators/region_allocator2.cpp 

OBJS += \
./prototypes/allocators/region_allocator.o \
./prototypes/allocators/region_allocator2.o 

CPP_DEPS += \
./prototypes/allocators/region_allocator.d \
./prototypes/allocators/region_allocator2.d 


# Each subdirectory must supply rules for building sources it contributes
prototypes/allocators/%.o: ../prototypes/allocators/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


