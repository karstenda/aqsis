################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../prototypes/binarytree/array_storage_index.cpp \
../prototypes/binarytree/depth_tree_update.cpp 

OBJS += \
./prototypes/binarytree/array_storage_index.o \
./prototypes/binarytree/depth_tree_update.o 

CPP_DEPS += \
./prototypes/binarytree/array_storage_index.d \
./prototypes/binarytree/depth_tree_update.d 


# Each subdirectory must supply rules for building sources it contributes
prototypes/binarytree/%.o: ../prototypes/binarytree/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


