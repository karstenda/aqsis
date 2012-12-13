################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../libs/core/api/condition.cpp \
../libs/core/api/genpoly.cpp \
../libs/core/api/graphicsstate.cpp \
../libs/core/api/ri.cpp \
../libs/core/api/rif.cpp \
../libs/core/api/rif_test.cpp 

OBJS += \
./libs/core/api/condition.o \
./libs/core/api/genpoly.o \
./libs/core/api/graphicsstate.o \
./libs/core/api/ri.o \
./libs/core/api/rif.o \
./libs/core/api/rif_test.o 

CPP_DEPS += \
./libs/core/api/condition.d \
./libs/core/api/genpoly.d \
./libs/core/api/graphicsstate.d \
./libs/core/api/ri.d \
./libs/core/api/rif.d \
./libs/core/api/rif_test.d 


# Each subdirectory must supply rules for building sources it contributes
libs/core/api/%.o: ../libs/core/api/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


