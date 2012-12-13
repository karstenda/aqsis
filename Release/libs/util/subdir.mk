################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../libs/util/argparse.cpp \
../libs/util/argparse_test.cpp \
../libs/util/bitvector.cpp \
../libs/util/enum_test.cpp \
../libs/util/exception.cpp \
../libs/util/file.cpp \
../libs/util/file_test.cpp \
../libs/util/logging.cpp \
../libs/util/plugins.cpp \
../libs/util/popen.cpp \
../libs/util/popen_pipethrough_helper_test.cpp \
../libs/util/popen_test.cpp \
../libs/util/sstring.cpp 

OBJS += \
./libs/util/argparse.o \
./libs/util/argparse_test.o \
./libs/util/bitvector.o \
./libs/util/enum_test.o \
./libs/util/exception.o \
./libs/util/file.o \
./libs/util/file_test.o \
./libs/util/logging.o \
./libs/util/plugins.o \
./libs/util/popen.o \
./libs/util/popen_pipethrough_helper_test.o \
./libs/util/popen_test.o \
./libs/util/sstring.o 

CPP_DEPS += \
./libs/util/argparse.d \
./libs/util/argparse_test.d \
./libs/util/bitvector.d \
./libs/util/enum_test.d \
./libs/util/exception.d \
./libs/util/file.d \
./libs/util/file_test.d \
./libs/util/logging.d \
./libs/util/plugins.d \
./libs/util/popen.d \
./libs/util/popen_pipethrough_helper_test.d \
./libs/util/popen_test.d \
./libs/util/sstring.d 


# Each subdirectory must supply rules for building sources it contributes
libs/util/%.o: ../libs/util/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


