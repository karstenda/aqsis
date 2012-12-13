################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../libs/tex/buffers/channellist_test.cpp \
../libs/tex/buffers/imagechannel.cpp \
../libs/tex/buffers/imagechannel_test.cpp \
../libs/tex/buffers/mixedimagebuffer.cpp \
../libs/tex/buffers/mixedimagebuffer_test.cpp 

OBJS += \
./libs/tex/buffers/channellist_test.o \
./libs/tex/buffers/imagechannel.o \
./libs/tex/buffers/imagechannel_test.o \
./libs/tex/buffers/mixedimagebuffer.o \
./libs/tex/buffers/mixedimagebuffer_test.o 

CPP_DEPS += \
./libs/tex/buffers/channellist_test.d \
./libs/tex/buffers/imagechannel.d \
./libs/tex/buffers/imagechannel_test.d \
./libs/tex/buffers/mixedimagebuffer.d \
./libs/tex/buffers/mixedimagebuffer_test.d 


# Each subdirectory must supply rules for building sources it contributes
libs/tex/buffers/%.o: ../libs/tex/buffers/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


