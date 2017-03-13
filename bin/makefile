################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/protocolStack/rrc/ho/subdir.mk
-include src/protocolStack/rrc/subdir.mk
-include src/protocolStack/rlc/subdir.mk
-include src/protocolStack/protocols/subdir.mk
-include src/protocolStack/pdcp/subdir.mk
-include src/protocolStack/packet/subdir.mk
-include src/protocolStack/nas/subdir.mk
-include src/protocolStack/mac/packet-scheduler/subdir.mk
-include src/protocolStack/mac/subdir.mk
-include src/protocolStack/subdir.mk
-include src/phy/subdir.mk
-include src/networkTopology/subdir.mk
-include src/mobility/subdir.mk
-include src/flows/application/subdir.mk
-include src/flows/QoS/subdir.mk
-include src/flows/subdir.mk
-include src/device/IPClassifier/subdir.mk
-include src/device/CqiManager/subdir.mk
-include src/device/subdir.mk
-include src/core/spectrum/subdir.mk
-include src/core/idealMessages/subdir.mk
-include src/core/eventScheduler/subdir.mk
-include src/core/cartesianCoodrdinates/subdir.mk
-include src/componentManagers/subdir.mk
-include src/channel/propagation-model/subdir.mk
-include src/channel/subdir.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: lte-sim-r5

# Tool invocations
lte-sim-r5: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++  -o "lte-sim-r5" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(CC_DEPS)$(C++_DEPS)$(EXECUTABLES)$(C_UPPER_DEPS)$(CXX_DEPS)$(OBJS)$(CPP_DEPS)$(C_DEPS) lte-sim-r5
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
