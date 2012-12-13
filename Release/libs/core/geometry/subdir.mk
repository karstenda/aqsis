################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../libs/core/geometry/blobby.cpp \
../libs/core/geometry/bunny.cpp \
../libs/core/geometry/cubiccurves.cpp \
../libs/core/geometry/curves.cpp \
../libs/core/geometry/jules_bloomenthal.cpp \
../libs/core/geometry/lath.cpp \
../libs/core/geometry/linearcurves.cpp \
../libs/core/geometry/marchingcubes.cpp \
../libs/core/geometry/nurbs.cpp \
../libs/core/geometry/patch.cpp \
../libs/core/geometry/points.cpp \
../libs/core/geometry/polygon.cpp \
../libs/core/geometry/procedural.cpp \
../libs/core/geometry/quadrics.cpp \
../libs/core/geometry/subdivision2.cpp \
../libs/core/geometry/surface.cpp \
../libs/core/geometry/teapot.cpp \
../libs/core/geometry/trimcurve.cpp 

OBJS += \
./libs/core/geometry/blobby.o \
./libs/core/geometry/bunny.o \
./libs/core/geometry/cubiccurves.o \
./libs/core/geometry/curves.o \
./libs/core/geometry/jules_bloomenthal.o \
./libs/core/geometry/lath.o \
./libs/core/geometry/linearcurves.o \
./libs/core/geometry/marchingcubes.o \
./libs/core/geometry/nurbs.o \
./libs/core/geometry/patch.o \
./libs/core/geometry/points.o \
./libs/core/geometry/polygon.o \
./libs/core/geometry/procedural.o \
./libs/core/geometry/quadrics.o \
./libs/core/geometry/subdivision2.o \
./libs/core/geometry/surface.o \
./libs/core/geometry/teapot.o \
./libs/core/geometry/trimcurve.o 

CPP_DEPS += \
./libs/core/geometry/blobby.d \
./libs/core/geometry/bunny.d \
./libs/core/geometry/cubiccurves.d \
./libs/core/geometry/curves.d \
./libs/core/geometry/jules_bloomenthal.d \
./libs/core/geometry/lath.d \
./libs/core/geometry/linearcurves.d \
./libs/core/geometry/marchingcubes.d \
./libs/core/geometry/nurbs.d \
./libs/core/geometry/patch.d \
./libs/core/geometry/points.d \
./libs/core/geometry/polygon.d \
./libs/core/geometry/procedural.d \
./libs/core/geometry/quadrics.d \
./libs/core/geometry/subdivision2.d \
./libs/core/geometry/surface.d \
./libs/core/geometry/teapot.d \
./libs/core/geometry/trimcurve.d 


# Each subdirectory must supply rules for building sources it contributes
libs/core/geometry/%.o: ../libs/core/geometry/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


