################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../src/core/eventScheduler/simulator.cc 

CPP_SRCS += \
../src/core/eventScheduler/calendar.cpp \
../src/core/eventScheduler/event.cpp 

CC_DEPS += \
./src/core/eventScheduler/simulator.d 

OBJS += \
./src/core/eventScheduler/calendar.o \
./src/core/eventScheduler/event.o \
./src/core/eventScheduler/simulator.o 

CPP_DEPS += \
./src/core/eventScheduler/calendar.d \
./src/core/eventScheduler/event.d 


# Each subdirectory must supply rules for building sources it contributes
src/core/eventScheduler/%.o: ../src/core/eventScheduler/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -w -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/core/eventScheduler/%.o: ../src/core/eventScheduler/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -w -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


