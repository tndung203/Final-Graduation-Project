################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ThirdParty/ThirdParty/FreeRTOS/portable/MemMang/heap_4.c 

OBJS += \
./ThirdParty/ThirdParty/FreeRTOS/portable/MemMang/heap_4.o 

C_DEPS += \
./ThirdParty/ThirdParty/FreeRTOS/portable/MemMang/heap_4.d 


# Each subdirectory must supply rules for building sources it contributes
ThirdParty/ThirdParty/FreeRTOS/portable/MemMang/%.o ThirdParty/ThirdParty/FreeRTOS/portable/MemMang/%.su ThirdParty/ThirdParty/FreeRTOS/portable/MemMang/%.cyclo: ../ThirdParty/ThirdParty/FreeRTOS/portable/MemMang/%.c ThirdParty/ThirdParty/FreeRTOS/portable/MemMang/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I"D:/e/FreeRTOS tutorials/RTOS_workspace/Final_Graduation_Project/ThirdParty" -I"D:/e/FreeRTOS tutorials/RTOS_workspace/Final_Graduation_Project/ThirdParty/include" -I"D:/e/FreeRTOS tutorials/RTOS_workspace/Final_Graduation_Project/ThirdParty/portable/GCC/ARM_CM4F" -I"D:/e/FreeRTOS tutorials/RTOS_workspace/Final_Graduation_Project/ECUAL/I2C_LCD" -I"D:/e/FreeRTOS tutorials/RTOS_workspace/Final_Graduation_Project/util" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-ThirdParty-2f-ThirdParty-2f-FreeRTOS-2f-portable-2f-MemMang

clean-ThirdParty-2f-ThirdParty-2f-FreeRTOS-2f-portable-2f-MemMang:
	-$(RM) ./ThirdParty/ThirdParty/FreeRTOS/portable/MemMang/heap_4.cyclo ./ThirdParty/ThirdParty/FreeRTOS/portable/MemMang/heap_4.d ./ThirdParty/ThirdParty/FreeRTOS/portable/MemMang/heap_4.o ./ThirdParty/ThirdParty/FreeRTOS/portable/MemMang/heap_4.su

.PHONY: clean-ThirdParty-2f-ThirdParty-2f-FreeRTOS-2f-portable-2f-MemMang

