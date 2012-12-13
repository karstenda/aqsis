################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../thirdparty/pdiff/CompareArgs.cpp \
../thirdparty/pdiff/LPyramid.cpp \
../thirdparty/pdiff/Metric.cpp \
../thirdparty/pdiff/PerceptualDiff.cpp \
../thirdparty/pdiff/RGBAImage.cpp 

OBJS += \
./thirdparty/pdiff/CompareArgs.o \
./thirdparty/pdiff/LPyramid.o \
./thirdparty/pdiff/Metric.o \
./thirdparty/pdiff/PerceptualDiff.o \
./thirdparty/pdiff/RGBAImage.o 

CPP_DEPS += \
./thirdparty/pdiff/CompareArgs.d \
./thirdparty/pdiff/LPyramid.d \
./thirdparty/pdiff/Metric.d \
./thirdparty/pdiff/PerceptualDiff.d \
./thirdparty/pdiff/RGBAImage.d 


# Each subdirectory must supply rules for building sources it contributes
thirdparty/pdiff/%.o: ../thirdparty/pdiff/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


