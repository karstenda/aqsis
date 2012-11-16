################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../thirdparty/aqiv/getopt.c \
../thirdparty/aqiv/iv.c \
../thirdparty/aqiv/sgigt.c 

OBJS += \
./thirdparty/aqiv/getopt.o \
./thirdparty/aqiv/iv.o \
./thirdparty/aqiv/sgigt.o 

C_DEPS += \
./thirdparty/aqiv/getopt.d \
./thirdparty/aqiv/iv.d \
./thirdparty/aqiv/sgigt.d 


# Each subdirectory must supply rules for building sources it contributes
thirdparty/aqiv/%.o: ../thirdparty/aqiv/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


