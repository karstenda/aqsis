################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../libs/math/cellnoise.cpp \
../libs/math/cellnoise_test.cpp \
../libs/math/color.cpp \
../libs/math/color_test.cpp \
../libs/math/lowdiscrep.cpp \
../libs/math/math_test.cpp \
../libs/math/matrix.cpp \
../libs/math/matrix2d_test.cpp \
../libs/math/matrix_test.cpp \
../libs/math/noise.cpp \
../libs/math/noise1234.cpp \
../libs/math/noise1234_test.cpp \
../libs/math/noise_test.cpp \
../libs/math/random.cpp \
../libs/math/spline.cpp \
../libs/math/spline_test.cpp \
../libs/math/vector2d_test.cpp \
../libs/math/vector3d_test.cpp \
../libs/math/vector4d_test.cpp 

OBJS += \
./libs/math/cellnoise.o \
./libs/math/cellnoise_test.o \
./libs/math/color.o \
./libs/math/color_test.o \
./libs/math/lowdiscrep.o \
./libs/math/math_test.o \
./libs/math/matrix.o \
./libs/math/matrix2d_test.o \
./libs/math/matrix_test.o \
./libs/math/noise.o \
./libs/math/noise1234.o \
./libs/math/noise1234_test.o \
./libs/math/noise_test.o \
./libs/math/random.o \
./libs/math/spline.o \
./libs/math/spline_test.o \
./libs/math/vector2d_test.o \
./libs/math/vector3d_test.o \
./libs/math/vector4d_test.o 

CPP_DEPS += \
./libs/math/cellnoise.d \
./libs/math/cellnoise_test.d \
./libs/math/color.d \
./libs/math/color_test.d \
./libs/math/lowdiscrep.d \
./libs/math/math_test.d \
./libs/math/matrix.d \
./libs/math/matrix2d_test.d \
./libs/math/matrix_test.d \
./libs/math/noise.d \
./libs/math/noise1234.d \
./libs/math/noise1234_test.d \
./libs/math/noise_test.d \
./libs/math/random.d \
./libs/math/spline.d \
./libs/math/spline_test.d \
./libs/math/vector2d_test.d \
./libs/math/vector3d_test.d \
./libs/math/vector4d_test.d 


# Each subdirectory must supply rules for building sources it contributes
libs/math/%.o: ../libs/math/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


