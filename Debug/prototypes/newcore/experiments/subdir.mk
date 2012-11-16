################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../prototypes/newcore/experiments/edge_eqn_test.cpp \
../prototypes/newcore/experiments/edge_eqn_test2.cpp \
../prototypes/newcore/experiments/float4_test.cpp \
../prototypes/newcore/experiments/fractal_threading.cpp \
../prototypes/newcore/experiments/microquad_sse_test.cpp \
../prototypes/newcore/experiments/simple_threadpool.cpp 

OBJS += \
./prototypes/newcore/experiments/edge_eqn_test.o \
./prototypes/newcore/experiments/edge_eqn_test2.o \
./prototypes/newcore/experiments/float4_test.o \
./prototypes/newcore/experiments/fractal_threading.o \
./prototypes/newcore/experiments/microquad_sse_test.o \
./prototypes/newcore/experiments/simple_threadpool.o 

CPP_DEPS += \
./prototypes/newcore/experiments/edge_eqn_test.d \
./prototypes/newcore/experiments/edge_eqn_test2.d \
./prototypes/newcore/experiments/float4_test.d \
./prototypes/newcore/experiments/fractal_threading.d \
./prototypes/newcore/experiments/microquad_sse_test.d \
./prototypes/newcore/experiments/simple_threadpool.d 


# Each subdirectory must supply rules for building sources it contributes
prototypes/newcore/experiments/%.o: ../prototypes/newcore/experiments/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


