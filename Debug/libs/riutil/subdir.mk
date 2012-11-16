################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../libs/riutil/errorhandler_test.cpp \
../libs/riutil/framedrop_filter.cpp \
../libs/riutil/primvartoken.cpp \
../libs/riutil/primvartoken_test.cpp \
../libs/riutil/renderutil_filter.cpp \
../libs/riutil/ri2ricxx.cpp \
../libs/riutil/ribinputbuffer.cpp \
../libs/riutil/ribinputbuffer_test.cpp \
../libs/riutil/riblexer.cpp \
../libs/riutil/riblexer_test.cpp \
../libs/riutil/ribparser.cpp \
../libs/riutil/ribparser_test.cpp \
../libs/riutil/ribtokenizer.cpp \
../libs/riutil/ribtokenizer_test.cpp \
../libs/riutil/ribwriter.cpp \
../libs/riutil/ricxx2ri.cpp \
../libs/riutil/ricxx_filter.cpp \
../libs/riutil/ricxx_validate.cpp \
../libs/riutil/risyms.cpp \
../libs/riutil/tee_filter.cpp \
../libs/riutil/tokendictionary.cpp 

OBJS += \
./libs/riutil/errorhandler_test.o \
./libs/riutil/framedrop_filter.o \
./libs/riutil/primvartoken.o \
./libs/riutil/primvartoken_test.o \
./libs/riutil/renderutil_filter.o \
./libs/riutil/ri2ricxx.o \
./libs/riutil/ribinputbuffer.o \
./libs/riutil/ribinputbuffer_test.o \
./libs/riutil/riblexer.o \
./libs/riutil/riblexer_test.o \
./libs/riutil/ribparser.o \
./libs/riutil/ribparser_test.o \
./libs/riutil/ribtokenizer.o \
./libs/riutil/ribtokenizer_test.o \
./libs/riutil/ribwriter.o \
./libs/riutil/ricxx2ri.o \
./libs/riutil/ricxx_filter.o \
./libs/riutil/ricxx_validate.o \
./libs/riutil/risyms.o \
./libs/riutil/tee_filter.o \
./libs/riutil/tokendictionary.o 

CPP_DEPS += \
./libs/riutil/errorhandler_test.d \
./libs/riutil/framedrop_filter.d \
./libs/riutil/primvartoken.d \
./libs/riutil/primvartoken_test.d \
./libs/riutil/renderutil_filter.d \
./libs/riutil/ri2ricxx.d \
./libs/riutil/ribinputbuffer.d \
./libs/riutil/ribinputbuffer_test.d \
./libs/riutil/riblexer.d \
./libs/riutil/riblexer_test.d \
./libs/riutil/ribparser.d \
./libs/riutil/ribparser_test.d \
./libs/riutil/ribtokenizer.d \
./libs/riutil/ribtokenizer_test.d \
./libs/riutil/ribwriter.d \
./libs/riutil/ricxx2ri.d \
./libs/riutil/ricxx_filter.d \
./libs/riutil/ricxx_validate.d \
./libs/riutil/risyms.d \
./libs/riutil/tee_filter.d \
./libs/riutil/tokendictionary.d 


# Each subdirectory must supply rules for building sources it contributes
libs/riutil/%.o: ../libs/riutil/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


