################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ThirdParty/ThirdParty/FreeRTOS/croutine.c \
../ThirdParty/ThirdParty/FreeRTOS/event_groups.c \
../ThirdParty/ThirdParty/FreeRTOS/list.c \
../ThirdParty/ThirdParty/FreeRTOS/queue.c \
../ThirdParty/ThirdParty/FreeRTOS/stream_buffer.c \
../ThirdParty/ThirdParty/FreeRTOS/tasks.c \
../ThirdParty/ThirdParty/FreeRTOS/timers.c 

OBJS += \
./ThirdParty/ThirdParty/FreeRTOS/croutine.o \
./ThirdParty/ThirdParty/FreeRTOS/event_groups.o \
./ThirdParty/ThirdParty/FreeRTOS/list.o \
./ThirdParty/ThirdParty/FreeRTOS/queue.o \
./ThirdParty/ThirdParty/FreeRTOS/stream_buffer.o \
./ThirdParty/ThirdParty/FreeRTOS/tasks.o \
./ThirdParty/ThirdParty/FreeRTOS/timers.o 

C_DEPS += \
./ThirdParty/ThirdParty/FreeRTOS/croutine.d \
./ThirdParty/ThirdParty/FreeRTOS/event_groups.d \
./ThirdParty/ThirdParty/FreeRTOS/list.d \
./ThirdParty/ThirdParty/FreeRTOS/queue.d \
./ThirdParty/ThirdParty/FreeRTOS/stream_buffer.d \
./ThirdParty/ThirdParty/FreeRTOS/tasks.d \
./ThirdParty/ThirdParty/FreeRTOS/timers.d 


# Each subdirectory must supply rules for building sources it contributes
ThirdParty/ThirdParty/FreeRTOS/%.o ThirdParty/ThirdParty/FreeRTOS/%.su ThirdParty/ThirdParty/FreeRTOS/%.cyclo: ../ThirdParty/ThirdParty/FreeRTOS/%.c ThirdParty/ThirdParty/FreeRTOS/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I"D:/e/FreeRTOS tutorials/RTOS_workspace/Final_Graduation_Project/ThirdParty" -I"D:/e/FreeRTOS tutorials/RTOS_workspace/Final_Graduation_Project/ThirdParty/include" -I"D:/e/FreeRTOS tutorials/RTOS_workspace/Final_Graduation_Project/ThirdParty/portable/GCC/ARM_CM4F" -I"D:/e/FreeRTOS tutorials/RTOS_workspace/Final_Graduation_Project/ECUAL/I2C_LCD" -I"D:/e/FreeRTOS tutorials/RTOS_workspace/Final_Graduation_Project/util" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-ThirdParty-2f-ThirdParty-2f-FreeRTOS

clean-ThirdParty-2f-ThirdParty-2f-FreeRTOS:
	-$(RM) ./ThirdParty/ThirdParty/FreeRTOS/croutine.cyclo ./ThirdParty/ThirdParty/FreeRTOS/croutine.d ./ThirdParty/ThirdParty/FreeRTOS/croutine.o ./ThirdParty/ThirdParty/FreeRTOS/croutine.su ./ThirdParty/ThirdParty/FreeRTOS/event_groups.cyclo ./ThirdParty/ThirdParty/FreeRTOS/event_groups.d ./ThirdParty/ThirdParty/FreeRTOS/event_groups.o ./ThirdParty/ThirdParty/FreeRTOS/event_groups.su ./ThirdParty/ThirdParty/FreeRTOS/list.cyclo ./ThirdParty/ThirdParty/FreeRTOS/list.d ./ThirdParty/ThirdParty/FreeRTOS/list.o ./ThirdParty/ThirdParty/FreeRTOS/list.su ./ThirdParty/ThirdParty/FreeRTOS/queue.cyclo ./ThirdParty/ThirdParty/FreeRTOS/queue.d ./ThirdParty/ThirdParty/FreeRTOS/queue.o ./ThirdParty/ThirdParty/FreeRTOS/queue.su ./ThirdParty/ThirdParty/FreeRTOS/stream_buffer.cyclo ./ThirdParty/ThirdParty/FreeRTOS/stream_buffer.d ./ThirdParty/ThirdParty/FreeRTOS/stream_buffer.o ./ThirdParty/ThirdParty/FreeRTOS/stream_buffer.su ./ThirdParty/ThirdParty/FreeRTOS/tasks.cyclo ./ThirdParty/ThirdParty/FreeRTOS/tasks.d ./ThirdParty/ThirdParty/FreeRTOS/tasks.o ./ThirdParty/ThirdParty/FreeRTOS/tasks.su ./ThirdParty/ThirdParty/FreeRTOS/timers.cyclo ./ThirdParty/ThirdParty/FreeRTOS/timers.d ./ThirdParty/ThirdParty/FreeRTOS/timers.o ./ThirdParty/ThirdParty/FreeRTOS/timers.su

.PHONY: clean-ThirdParty-2f-ThirdParty-2f-FreeRTOS

