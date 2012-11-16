################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../thirdparty/partio/src/src/tools/partconv.cpp \
../thirdparty/partio/src/src/tools/partconvert.cpp \
../thirdparty/partio/src/src/tools/partinfo.cpp \
../thirdparty/partio/src/src/tools/partview.cpp 

OBJS += \
./thirdparty/partio/src/src/tools/partconv.o \
./thirdparty/partio/src/src/tools/partconvert.o \
./thirdparty/partio/src/src/tools/partinfo.o \
./thirdparty/partio/src/src/tools/partview.o 

CPP_DEPS += \
./thirdparty/partio/src/src/tools/partconv.d \
./thirdparty/partio/src/src/tools/partconvert.d \
./thirdparty/partio/src/src/tools/partinfo.d \
./thirdparty/partio/src/src/tools/partview.d 


# Each subdirectory must supply rules for building sources it contributes
thirdparty/partio/src/src/tools/%.o: ../thirdparty/partio/src/src/tools/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


