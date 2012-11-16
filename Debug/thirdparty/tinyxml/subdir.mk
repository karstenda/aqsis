################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../thirdparty/tinyxml/tinystr.cpp \
../thirdparty/tinyxml/tinyxml.cpp \
../thirdparty/tinyxml/tinyxmlerror.cpp \
../thirdparty/tinyxml/tinyxmlparser.cpp 

OBJS += \
./thirdparty/tinyxml/tinystr.o \
./thirdparty/tinyxml/tinyxml.o \
./thirdparty/tinyxml/tinyxmlerror.o \
./thirdparty/tinyxml/tinyxmlparser.o 

CPP_DEPS += \
./thirdparty/tinyxml/tinystr.d \
./thirdparty/tinyxml/tinyxml.d \
./thirdparty/tinyxml/tinyxmlerror.d \
./thirdparty/tinyxml/tinyxmlparser.d 


# Each subdirectory must supply rules for building sources it contributes
thirdparty/tinyxml/%.o: ../thirdparty/tinyxml/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


