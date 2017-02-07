################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../TOOLS/make_avg.cc \
../TOOLS/make_cdf.cc \
../TOOLS/make_cell_spectral_efficiency.cc \
../TOOLS/make_fairness_index.cc \
../TOOLS/make_goodput.cc \
../TOOLS/make_gput.cc \
../TOOLS/make_plr.cc \
../TOOLS/make_sum.cc 

CC_DEPS += \
./TOOLS/make_avg.d \
./TOOLS/make_cdf.d \
./TOOLS/make_cell_spectral_efficiency.d \
./TOOLS/make_fairness_index.d \
./TOOLS/make_goodput.d \
./TOOLS/make_gput.d \
./TOOLS/make_plr.d \
./TOOLS/make_sum.d 

OBJS += \
./TOOLS/make_avg.o \
./TOOLS/make_cdf.o \
./TOOLS/make_cell_spectral_efficiency.o \
./TOOLS/make_fairness_index.o \
./TOOLS/make_goodput.o \
./TOOLS/make_gput.o \
./TOOLS/make_plr.o \
./TOOLS/make_sum.o 


# Each subdirectory must supply rules for building sources it contributes
TOOLS/%.o: ../TOOLS/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


