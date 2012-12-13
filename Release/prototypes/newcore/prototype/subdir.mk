################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../prototypes/newcore/prototype/vartable_test.cpp \
../prototypes/newcore/prototype/vset_test.cpp 

OBJS += \
./prototypes/newcore/prototype/vartable_test.o \
./prototypes/newcore/prototype/vset_test.o 

CPP_DEPS += \
./prototypes/newcore/prototype/vartable_test.d \
./prototypes/newcore/prototype/vset_test.d 


# Each subdirectory must supply rules for building sources it contributes
prototypes/newcore/prototype/%.o: ../prototypes/newcore/prototype/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


