################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../include/calcSunTimes.c 

OBJS += \
./include/calcSunTimes.o 

C_DEPS += \
./include/calcSunTimes.d 


# Each subdirectory must supply rules for building sources it contributes
include/%.o: ../include/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-linux-gnueabihf-gcc -I"/home/thomas/workspace_neon/calcOpTimes/include" -I/usr/include/arm-linux-gnueabihf -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


