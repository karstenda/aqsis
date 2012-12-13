################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../thirdparty/partio/src/src/tests/makecircle.cpp \
../thirdparty/partio/src/src/tests/makeline.cpp \
../thirdparty/partio/src/src/tests/test.cpp \
../thirdparty/partio/src/src/tests/testcache.cpp \
../thirdparty/partio/src/src/tests/testiterator.cpp \
../thirdparty/partio/src/src/tests/testkdtree.cpp 

OBJS += \
./thirdparty/partio/src/src/tests/makecircle.o \
./thirdparty/partio/src/src/tests/makeline.o \
./thirdparty/partio/src/src/tests/test.o \
./thirdparty/partio/src/src/tests/testcache.o \
./thirdparty/partio/src/src/tests/testiterator.o \
./thirdparty/partio/src/src/tests/testkdtree.o 

CPP_DEPS += \
./thirdparty/partio/src/src/tests/makecircle.d \
./thirdparty/partio/src/src/tests/makeline.d \
./thirdparty/partio/src/src/tests/test.d \
./thirdparty/partio/src/src/tests/testcache.d \
./thirdparty/partio/src/src/tests/testiterator.d \
./thirdparty/partio/src/src/tests/testkdtree.d 


# Each subdirectory must supply rules for building sources it contributes
thirdparty/partio/src/src/tests/%.o: ../thirdparty/partio/src/src/tests/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


