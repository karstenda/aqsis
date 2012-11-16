################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../tools/displays/sdcWin32/d_sdcWin32.cpp 

C_SRCS += \
../tools/displays/sdcWin32/interface.c 

OBJS += \
./tools/displays/sdcWin32/d_sdcWin32.o \
./tools/displays/sdcWin32/interface.o 

C_DEPS += \
./tools/displays/sdcWin32/interface.d 

CPP_DEPS += \
./tools/displays/sdcWin32/d_sdcWin32.d 


# Each subdirectory must supply rules for building sources it contributes
tools/displays/sdcWin32/%.o: ../tools/displays/sdcWin32/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

tools/displays/sdcWin32/%.o: ../tools/displays/sdcWin32/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


