################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../libs/shadervm/shaderexecenv/shadeops_bake3d.cpp \
../libs/shadervm/shaderexecenv/shadeops_bake3d_brdf.cpp \
../libs/shadervm/shaderexecenv/shadeops_comp.cpp \
../libs/shadervm/shaderexecenv/shadeops_deriv.cpp \
../libs/shadervm/shaderexecenv/shadeops_dso.cpp \
../libs/shadervm/shaderexecenv/shadeops_illum.cpp \
../libs/shadervm/shaderexecenv/shadeops_indirect.cpp \
../libs/shadervm/shaderexecenv/shadeops_inter.cpp \
../libs/shadervm/shaderexecenv/shadeops_math.cpp \
../libs/shadervm/shaderexecenv/shadeops_matrx.cpp \
../libs/shadervm/shaderexecenv/shadeops_rand.cpp \
../libs/shadervm/shaderexecenv/shadeops_text.cpp \
../libs/shadervm/shaderexecenv/shadeops_tmap.cpp \
../libs/shadervm/shaderexecenv/shaderexecenv.cpp 

OBJS += \
./libs/shadervm/shaderexecenv/shadeops_bake3d.o \
./libs/shadervm/shaderexecenv/shadeops_bake3d_brdf.o \
./libs/shadervm/shaderexecenv/shadeops_comp.o \
./libs/shadervm/shaderexecenv/shadeops_deriv.o \
./libs/shadervm/shaderexecenv/shadeops_dso.o \
./libs/shadervm/shaderexecenv/shadeops_illum.o \
./libs/shadervm/shaderexecenv/shadeops_indirect.o \
./libs/shadervm/shaderexecenv/shadeops_inter.o \
./libs/shadervm/shaderexecenv/shadeops_math.o \
./libs/shadervm/shaderexecenv/shadeops_matrx.o \
./libs/shadervm/shaderexecenv/shadeops_rand.o \
./libs/shadervm/shaderexecenv/shadeops_text.o \
./libs/shadervm/shaderexecenv/shadeops_tmap.o \
./libs/shadervm/shaderexecenv/shaderexecenv.o 

CPP_DEPS += \
./libs/shadervm/shaderexecenv/shadeops_bake3d.d \
./libs/shadervm/shaderexecenv/shadeops_bake3d_brdf.d \
./libs/shadervm/shaderexecenv/shadeops_comp.d \
./libs/shadervm/shaderexecenv/shadeops_deriv.d \
./libs/shadervm/shaderexecenv/shadeops_dso.d \
./libs/shadervm/shaderexecenv/shadeops_illum.d \
./libs/shadervm/shaderexecenv/shadeops_indirect.d \
./libs/shadervm/shaderexecenv/shadeops_inter.d \
./libs/shadervm/shaderexecenv/shadeops_math.d \
./libs/shadervm/shaderexecenv/shadeops_matrx.d \
./libs/shadervm/shaderexecenv/shadeops_rand.d \
./libs/shadervm/shaderexecenv/shadeops_text.d \
./libs/shadervm/shaderexecenv/shadeops_tmap.d \
./libs/shadervm/shaderexecenv/shaderexecenv.d 


# Each subdirectory must supply rules for building sources it contributes
libs/shadervm/shaderexecenv/%.o: ../libs/shadervm/shaderexecenv/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


