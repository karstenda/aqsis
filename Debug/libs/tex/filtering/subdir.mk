################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../libs/tex/filtering/cachedfilter.cpp \
../libs/tex/filtering/dummyenvironmentsampler.cpp \
../libs/tex/filtering/dummytexturesampler.cpp \
../libs/tex/filtering/ewafilter.cpp \
../libs/tex/filtering/ienvironmentsampler.cpp \
../libs/tex/filtering/iocclusionsampler.cpp \
../libs/tex/filtering/ishadowsampler.cpp \
../libs/tex/filtering/itexturesampler.cpp \
../libs/tex/filtering/occlusionsampler.cpp \
../libs/tex/filtering/randomtable.cpp \
../libs/tex/filtering/samplequad_test.cpp \
../libs/tex/filtering/shadowsampler.cpp \
../libs/tex/filtering/texturecache.cpp 

OBJS += \
./libs/tex/filtering/cachedfilter.o \
./libs/tex/filtering/dummyenvironmentsampler.o \
./libs/tex/filtering/dummytexturesampler.o \
./libs/tex/filtering/ewafilter.o \
./libs/tex/filtering/ienvironmentsampler.o \
./libs/tex/filtering/iocclusionsampler.o \
./libs/tex/filtering/ishadowsampler.o \
./libs/tex/filtering/itexturesampler.o \
./libs/tex/filtering/occlusionsampler.o \
./libs/tex/filtering/randomtable.o \
./libs/tex/filtering/samplequad_test.o \
./libs/tex/filtering/shadowsampler.o \
./libs/tex/filtering/texturecache.o 

CPP_DEPS += \
./libs/tex/filtering/cachedfilter.d \
./libs/tex/filtering/dummyenvironmentsampler.d \
./libs/tex/filtering/dummytexturesampler.d \
./libs/tex/filtering/ewafilter.d \
./libs/tex/filtering/ienvironmentsampler.d \
./libs/tex/filtering/iocclusionsampler.d \
./libs/tex/filtering/ishadowsampler.d \
./libs/tex/filtering/itexturesampler.d \
./libs/tex/filtering/occlusionsampler.d \
./libs/tex/filtering/randomtable.d \
./libs/tex/filtering/samplequad_test.d \
./libs/tex/filtering/shadowsampler.d \
./libs/tex/filtering/texturecache.d 


# Each subdirectory must supply rules for building sources it contributes
libs/tex/filtering/%.o: ../libs/tex/filtering/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


