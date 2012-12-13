################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../prototypes/newcore/api.cpp \
../prototypes/newcore/arrayview_test.cpp \
../prototypes/newcore/displaymanager.cpp \
../prototypes/newcore/filterprocessor.cpp \
../prototypes/newcore/filters.cpp \
../prototypes/newcore/gridstorage_test.cpp \
../prototypes/newcore/interactive.cpp \
../prototypes/newcore/main.cpp \
../prototypes/newcore/memdebug.cpp \
../prototypes/newcore/pointinquad_test.cpp \
../prototypes/newcore/primvar.cpp \
../prototypes/newcore/primvar_test.cpp \
../prototypes/newcore/renderer.cpp \
../prototypes/newcore/samplegen.cpp \
../prototypes/newcore/samplegen_test.cpp \
../prototypes/newcore/shader.cpp \
../prototypes/newcore/splitstore_test.cpp \
../prototypes/newcore/surfaces.cpp \
../prototypes/newcore/tessellation.cpp \
../prototypes/newcore/treearraystorage_test.cpp \
../prototypes/newcore/util_test.cpp \
../prototypes/newcore/varspec.cpp \
../prototypes/newcore/varspec_test.cpp 

OBJS += \
./prototypes/newcore/api.o \
./prototypes/newcore/arrayview_test.o \
./prototypes/newcore/displaymanager.o \
./prototypes/newcore/filterprocessor.o \
./prototypes/newcore/filters.o \
./prototypes/newcore/gridstorage_test.o \
./prototypes/newcore/interactive.o \
./prototypes/newcore/main.o \
./prototypes/newcore/memdebug.o \
./prototypes/newcore/pointinquad_test.o \
./prototypes/newcore/primvar.o \
./prototypes/newcore/primvar_test.o \
./prototypes/newcore/renderer.o \
./prototypes/newcore/samplegen.o \
./prototypes/newcore/samplegen_test.o \
./prototypes/newcore/shader.o \
./prototypes/newcore/splitstore_test.o \
./prototypes/newcore/surfaces.o \
./prototypes/newcore/tessellation.o \
./prototypes/newcore/treearraystorage_test.o \
./prototypes/newcore/util_test.o \
./prototypes/newcore/varspec.o \
./prototypes/newcore/varspec_test.o 

CPP_DEPS += \
./prototypes/newcore/api.d \
./prototypes/newcore/arrayview_test.d \
./prototypes/newcore/displaymanager.d \
./prototypes/newcore/filterprocessor.d \
./prototypes/newcore/filters.d \
./prototypes/newcore/gridstorage_test.d \
./prototypes/newcore/interactive.d \
./prototypes/newcore/main.d \
./prototypes/newcore/memdebug.d \
./prototypes/newcore/pointinquad_test.d \
./prototypes/newcore/primvar.d \
./prototypes/newcore/primvar_test.d \
./prototypes/newcore/renderer.d \
./prototypes/newcore/samplegen.d \
./prototypes/newcore/samplegen_test.d \
./prototypes/newcore/shader.d \
./prototypes/newcore/splitstore_test.d \
./prototypes/newcore/surfaces.d \
./prototypes/newcore/tessellation.d \
./prototypes/newcore/treearraystorage_test.d \
./prototypes/newcore/util_test.d \
./prototypes/newcore/varspec.d \
./prototypes/newcore/varspec_test.d 


# Each subdirectory must supply rules for building sources it contributes
prototypes/newcore/%.o: ../prototypes/newcore/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


