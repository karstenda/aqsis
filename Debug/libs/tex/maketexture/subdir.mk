################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../libs/tex/maketexture/bake.cpp \
../libs/tex/maketexture/maketexture.cpp 

OBJS += \
./libs/tex/maketexture/bake.o \
./libs/tex/maketexture/maketexture.o 

CPP_DEPS += \
./libs/tex/maketexture/bake.d \
./libs/tex/maketexture/maketexture.d 


# Each subdirectory must supply rules for building sources it contributes
libs/tex/maketexture/%.o: ../libs/tex/maketexture/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


