################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../tools/dbo/plane/dbo_plane.c 

OBJS += \
./tools/dbo/plane/dbo_plane.o 

C_DEPS += \
./tools/dbo/plane/dbo_plane.d 


# Each subdirectory must supply rules for building sources it contributes
tools/dbo/plane/%.o: ../tools/dbo/plane/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


