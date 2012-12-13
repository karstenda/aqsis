################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../libs/core/texturing_old/environment_old.cpp \
../libs/core/texturing_old/shadowmap_old.cpp \
../libs/core/texturing_old/texturemap_old.cpp 

OBJS += \
./libs/core/texturing_old/environment_old.o \
./libs/core/texturing_old/shadowmap_old.o \
./libs/core/texturing_old/texturemap_old.o 

CPP_DEPS += \
./libs/core/texturing_old/environment_old.d \
./libs/core/texturing_old/shadowmap_old.d \
./libs/core/texturing_old/texturemap_old.d 


# Each subdirectory must supply rules for building sources it contributes
libs/core/texturing_old/%.o: ../libs/core/texturing_old/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


