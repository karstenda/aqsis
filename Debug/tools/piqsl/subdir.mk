################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../tools/piqsl/displayserverimage.cpp \
../tools/piqsl/image.cpp \
../tools/piqsl/imagelistmodel.cpp \
../tools/piqsl/piqsl.cpp \
../tools/piqsl/piqsl_ui.cpp 

OBJS += \
./tools/piqsl/displayserverimage.o \
./tools/piqsl/image.o \
./tools/piqsl/imagelistmodel.o \
./tools/piqsl/piqsl.o \
./tools/piqsl/piqsl_ui.o 

CPP_DEPS += \
./tools/piqsl/displayserverimage.d \
./tools/piqsl/image.d \
./tools/piqsl/imagelistmodel.d \
./tools/piqsl/piqsl.d \
./tools/piqsl/piqsl_ui.d 


# Each subdirectory must supply rules for building sources it contributes
tools/piqsl/%.o: ../tools/piqsl/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


