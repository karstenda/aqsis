################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../libs/pointrender/NonDiffusePointCloud.cpp \
../libs/pointrender/microbuffer.cpp \
../libs/pointrender/pointcontainer.cpp 

OBJS += \
./libs/pointrender/NonDiffusePointCloud.o \
./libs/pointrender/microbuffer.o \
./libs/pointrender/pointcontainer.o 

CPP_DEPS += \
./libs/pointrender/NonDiffusePointCloud.d \
./libs/pointrender/microbuffer.d \
./libs/pointrender/pointcontainer.d 


# Each subdirectory must supply rules for building sources it contributes
libs/pointrender/%.o: ../libs/pointrender/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


