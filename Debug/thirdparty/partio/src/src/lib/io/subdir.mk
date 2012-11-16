################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../thirdparty/partio/src/src/lib/io/BGEO.cpp \
../thirdparty/partio/src/src/lib/io/GEO.cpp \
../thirdparty/partio/src/src/lib/io/PDA.cpp \
../thirdparty/partio/src/src/lib/io/PDB.cpp \
../thirdparty/partio/src/src/lib/io/PTC.cpp \
../thirdparty/partio/src/src/lib/io/ParticleIO.cpp \
../thirdparty/partio/src/src/lib/io/ZIP.cpp 

OBJS += \
./thirdparty/partio/src/src/lib/io/BGEO.o \
./thirdparty/partio/src/src/lib/io/GEO.o \
./thirdparty/partio/src/src/lib/io/PDA.o \
./thirdparty/partio/src/src/lib/io/PDB.o \
./thirdparty/partio/src/src/lib/io/PTC.o \
./thirdparty/partio/src/src/lib/io/ParticleIO.o \
./thirdparty/partio/src/src/lib/io/ZIP.o 

CPP_DEPS += \
./thirdparty/partio/src/src/lib/io/BGEO.d \
./thirdparty/partio/src/src/lib/io/GEO.d \
./thirdparty/partio/src/src/lib/io/PDA.d \
./thirdparty/partio/src/src/lib/io/PDB.d \
./thirdparty/partio/src/src/lib/io/PTC.d \
./thirdparty/partio/src/src/lib/io/ParticleIO.d \
./thirdparty/partio/src/src/lib/io/ZIP.d 


# Each subdirectory must supply rules for building sources it contributes
thirdparty/partio/src/src/lib/io/%.o: ../thirdparty/partio/src/src/lib/io/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


