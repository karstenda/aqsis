################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../libs/slcomp/parse/funcdef.cpp \
../libs/slcomp/parse/libslparse.cpp \
../libs/slcomp/parse/optimise.cpp \
../libs/slcomp/parse/parsenode.cpp \
../libs/slcomp/parse/typecheck.cpp \
../libs/slcomp/parse/vardef.cpp 

OBJS += \
./libs/slcomp/parse/funcdef.o \
./libs/slcomp/parse/libslparse.o \
./libs/slcomp/parse/optimise.o \
./libs/slcomp/parse/parsenode.o \
./libs/slcomp/parse/typecheck.o \
./libs/slcomp/parse/vardef.o 

CPP_DEPS += \
./libs/slcomp/parse/funcdef.d \
./libs/slcomp/parse/libslparse.d \
./libs/slcomp/parse/optimise.d \
./libs/slcomp/parse/parsenode.d \
./libs/slcomp/parse/typecheck.d \
./libs/slcomp/parse/vardef.d 


# Each subdirectory must supply rules for building sources it contributes
libs/slcomp/parse/%.o: ../libs/slcomp/parse/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


