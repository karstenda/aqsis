################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../prototypes/newcore/scenes/default.cpp \
../prototypes/newcore/scenes/dofamounttest.cpp \
../prototypes/newcore/scenes/mbnoisetest.cpp \
../prototypes/newcore/scenes/simpledeformation.cpp \
../prototypes/newcore/scenes/tenpatch.cpp 

OBJS += \
./prototypes/newcore/scenes/default.o \
./prototypes/newcore/scenes/dofamounttest.o \
./prototypes/newcore/scenes/mbnoisetest.o \
./prototypes/newcore/scenes/simpledeformation.o \
./prototypes/newcore/scenes/tenpatch.o 

CPP_DEPS += \
./prototypes/newcore/scenes/default.d \
./prototypes/newcore/scenes/dofamounttest.d \
./prototypes/newcore/scenes/mbnoisetest.d \
./prototypes/newcore/scenes/simpledeformation.d \
./prototypes/newcore/scenes/tenpatch.d 


# Each subdirectory must supply rules for building sources it contributes
prototypes/newcore/scenes/%.o: ../prototypes/newcore/scenes/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


