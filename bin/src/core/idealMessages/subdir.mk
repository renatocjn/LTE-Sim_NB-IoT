################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/core/idealMessages/ideal-control-messages.cc 

CC_DEPS += \
./src/core/idealMessages/ideal-control-messages.d 

OBJS += \
./src/core/idealMessages/ideal-control-messages.o 


# Each subdirectory must supply rules for building sources it contributes
src/core/idealMessages/%.o: ../src/core/idealMessages/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -w -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


