################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../tools/displays/dspyutil/dspyhlpr.c 

OBJS += \
./tools/displays/dspyutil/dspyhlpr.o 

C_DEPS += \
./tools/displays/dspyutil/dspyhlpr.d 


# Each subdirectory must supply rules for building sources it contributes
tools/displays/dspyutil/%.o: ../tools/displays/dspyutil/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


