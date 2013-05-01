################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../libs/pointrender/nondiffuse/approxhemi/HemiApprox.cpp \
../libs/pointrender/nondiffuse/approxhemi/MicroBufApprox.cpp \
../libs/pointrender/nondiffuse/approxhemi/SpherHarmonApprox.cpp 

OBJS += \
./libs/pointrender/nondiffuse/approxhemi/HemiApprox.o \
./libs/pointrender/nondiffuse/approxhemi/MicroBufApprox.o \
./libs/pointrender/nondiffuse/approxhemi/SpherHarmonApprox.o 

CPP_DEPS += \
./libs/pointrender/nondiffuse/approxhemi/HemiApprox.d \
./libs/pointrender/nondiffuse/approxhemi/MicroBufApprox.d \
./libs/pointrender/nondiffuse/approxhemi/SpherHarmonApprox.d 


# Each subdirectory must supply rules for building sources it contributes
libs/pointrender/nondiffuse/approxhemi/%.o: ../libs/pointrender/nondiffuse/approxhemi/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


