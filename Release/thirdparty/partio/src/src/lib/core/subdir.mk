################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../thirdparty/partio/src/src/lib/core/Particle.cpp \
../thirdparty/partio/src/src/lib/core/ParticleCaching.cpp \
../thirdparty/partio/src/src/lib/core/ParticleHeaders.cpp \
../thirdparty/partio/src/src/lib/core/ParticleSimple.cpp \
../thirdparty/partio/src/src/lib/core/ParticleSimpleInterleave.cpp 

OBJS += \
./thirdparty/partio/src/src/lib/core/Particle.o \
./thirdparty/partio/src/src/lib/core/ParticleCaching.o \
./thirdparty/partio/src/src/lib/core/ParticleHeaders.o \
./thirdparty/partio/src/src/lib/core/ParticleSimple.o \
./thirdparty/partio/src/src/lib/core/ParticleSimpleInterleave.o 

CPP_DEPS += \
./thirdparty/partio/src/src/lib/core/Particle.d \
./thirdparty/partio/src/src/lib/core/ParticleCaching.d \
./thirdparty/partio/src/src/lib/core/ParticleHeaders.d \
./thirdparty/partio/src/src/lib/core/ParticleSimple.d \
./thirdparty/partio/src/src/lib/core/ParticleSimpleInterleave.d 


# Each subdirectory must supply rules for building sources it contributes
thirdparty/partio/src/src/lib/core/%.o: ../thirdparty/partio/src/src/lib/core/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


