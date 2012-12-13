################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../libs/slcomp/backend/codegengraphviz.cpp \
../libs/slcomp/backend/codegenvm.cpp \
../libs/slcomp/backend/parsetreeviz.cpp \
../libs/slcomp/backend/vmdatagather.cpp \
../libs/slcomp/backend/vmoutput.cpp 

OBJS += \
./libs/slcomp/backend/codegengraphviz.o \
./libs/slcomp/backend/codegenvm.o \
./libs/slcomp/backend/parsetreeviz.o \
./libs/slcomp/backend/vmdatagather.o \
./libs/slcomp/backend/vmoutput.o 

CPP_DEPS += \
./libs/slcomp/backend/codegengraphviz.d \
./libs/slcomp/backend/codegenvm.d \
./libs/slcomp/backend/parsetreeviz.d \
./libs/slcomp/backend/vmdatagather.d \
./libs/slcomp/backend/vmoutput.d 


# Each subdirectory must supply rules for building sources it contributes
libs/slcomp/backend/%.o: ../libs/slcomp/backend/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


