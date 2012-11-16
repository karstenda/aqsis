################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../libs/tex/io/exrinputfile.cpp \
../libs/tex/io/itexinputfile.cpp \
../libs/tex/io/itexoutputfile.cpp \
../libs/tex/io/itiledtexinputfile.cpp \
../libs/tex/io/magicnumber.cpp \
../libs/tex/io/magicnumber_test.cpp \
../libs/tex/io/pnginputfile.cpp \
../libs/tex/io/pnginputfile_test.cpp \
../libs/tex/io/texfileheader.cpp \
../libs/tex/io/texfileheader_test.cpp \
../libs/tex/io/tiffdirhandle.cpp \
../libs/tex/io/tiffdirhandle_test.cpp \
../libs/tex/io/tiffinputfile.cpp \
../libs/tex/io/tiffinputfile_test.cpp \
../libs/tex/io/tiffoutputfile.cpp \
../libs/tex/io/tifftest_examples.cpp \
../libs/tex/io/tiledanyinputfile.cpp \
../libs/tex/io/tiledtiffinputfile.cpp \
../libs/tex/io/zinputfile.cpp 

OBJS += \
./libs/tex/io/exrinputfile.o \
./libs/tex/io/itexinputfile.o \
./libs/tex/io/itexoutputfile.o \
./libs/tex/io/itiledtexinputfile.o \
./libs/tex/io/magicnumber.o \
./libs/tex/io/magicnumber_test.o \
./libs/tex/io/pnginputfile.o \
./libs/tex/io/pnginputfile_test.o \
./libs/tex/io/texfileheader.o \
./libs/tex/io/texfileheader_test.o \
./libs/tex/io/tiffdirhandle.o \
./libs/tex/io/tiffdirhandle_test.o \
./libs/tex/io/tiffinputfile.o \
./libs/tex/io/tiffinputfile_test.o \
./libs/tex/io/tiffoutputfile.o \
./libs/tex/io/tifftest_examples.o \
./libs/tex/io/tiledanyinputfile.o \
./libs/tex/io/tiledtiffinputfile.o \
./libs/tex/io/zinputfile.o 

CPP_DEPS += \
./libs/tex/io/exrinputfile.d \
./libs/tex/io/itexinputfile.d \
./libs/tex/io/itexoutputfile.d \
./libs/tex/io/itiledtexinputfile.d \
./libs/tex/io/magicnumber.d \
./libs/tex/io/magicnumber_test.d \
./libs/tex/io/pnginputfile.d \
./libs/tex/io/pnginputfile_test.d \
./libs/tex/io/texfileheader.d \
./libs/tex/io/texfileheader_test.d \
./libs/tex/io/tiffdirhandle.d \
./libs/tex/io/tiffdirhandle_test.d \
./libs/tex/io/tiffinputfile.d \
./libs/tex/io/tiffinputfile_test.d \
./libs/tex/io/tiffoutputfile.d \
./libs/tex/io/tifftest_examples.d \
./libs/tex/io/tiledanyinputfile.d \
./libs/tex/io/tiledtiffinputfile.d \
./libs/tex/io/zinputfile.d 


# Each subdirectory must supply rules for building sources it contributes
libs/tex/io/%.o: ../libs/tex/io/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


