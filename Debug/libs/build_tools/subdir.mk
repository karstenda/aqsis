################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../libs/build_tools/boostautotestmain.cpp \
../libs/build_tools/testmain.cpp 

OBJS += \
./libs/build_tools/boostautotestmain.o \
./libs/build_tools/testmain.o 

CPP_DEPS += \
./libs/build_tools/boostautotestmain.d \
./libs/build_tools/testmain.d 


# Each subdirectory must supply rules for building sources it contributes
libs/build_tools/%.o: ../libs/build_tools/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


