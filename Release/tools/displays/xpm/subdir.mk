################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../tools/displays/xpm/aspXpm.cpp \
../tools/displays/xpm/xpm.cpp \
../tools/displays/xpm/xpm4pixie.cpp 

OBJS += \
./tools/displays/xpm/aspXpm.o \
./tools/displays/xpm/xpm.o \
./tools/displays/xpm/xpm4pixie.o 

CPP_DEPS += \
./tools/displays/xpm/aspXpm.d \
./tools/displays/xpm/xpm.d \
./tools/displays/xpm/xpm4pixie.d 


# Each subdirectory must supply rules for building sources it contributes
tools/displays/xpm/%.o: ../tools/displays/xpm/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


