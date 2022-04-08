################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Dave/Generated/UCPROBE/cpu_bsp.c \
../Dave/Generated/UCPROBE/cpu_c.c \
../Dave/Generated/UCPROBE/cpu_core.c \
../Dave/Generated/UCPROBE/lib_ascii.c \
../Dave/Generated/UCPROBE/lib_math.c \
../Dave/Generated/UCPROBE/lib_mem.c \
../Dave/Generated/UCPROBE/lib_str.c \
../Dave/Generated/UCPROBE/probe_com.c \
../Dave/Generated/UCPROBE/probe_com_os.c \
../Dave/Generated/UCPROBE/probe_rs232.c \
../Dave/Generated/UCPROBE/probe_rs232_os.c \
../Dave/Generated/UCPROBE/probe_rs232c.c \
../Dave/Generated/UCPROBE/probe_scope.c \
../Dave/Generated/UCPROBE/ucprobe.c \
../Dave/Generated/UCPROBE/ucprobe_conf.c 

S_UPPER_SRCS += \
../Dave/Generated/UCPROBE/cpu_a.S \
../Dave/Generated/UCPROBE/lib_mem_a.S 

OBJS += \
./Dave/Generated/UCPROBE/cpu_a.o \
./Dave/Generated/UCPROBE/cpu_bsp.o \
./Dave/Generated/UCPROBE/cpu_c.o \
./Dave/Generated/UCPROBE/cpu_core.o \
./Dave/Generated/UCPROBE/lib_ascii.o \
./Dave/Generated/UCPROBE/lib_math.o \
./Dave/Generated/UCPROBE/lib_mem.o \
./Dave/Generated/UCPROBE/lib_mem_a.o \
./Dave/Generated/UCPROBE/lib_str.o \
./Dave/Generated/UCPROBE/probe_com.o \
./Dave/Generated/UCPROBE/probe_com_os.o \
./Dave/Generated/UCPROBE/probe_rs232.o \
./Dave/Generated/UCPROBE/probe_rs232_os.o \
./Dave/Generated/UCPROBE/probe_rs232c.o \
./Dave/Generated/UCPROBE/probe_scope.o \
./Dave/Generated/UCPROBE/ucprobe.o \
./Dave/Generated/UCPROBE/ucprobe_conf.o 

S_UPPER_DEPS += \
./Dave/Generated/UCPROBE/cpu_a.d \
./Dave/Generated/UCPROBE/lib_mem_a.d 

C_DEPS += \
./Dave/Generated/UCPROBE/cpu_bsp.d \
./Dave/Generated/UCPROBE/cpu_c.d \
./Dave/Generated/UCPROBE/cpu_core.d \
./Dave/Generated/UCPROBE/lib_ascii.d \
./Dave/Generated/UCPROBE/lib_math.d \
./Dave/Generated/UCPROBE/lib_mem.d \
./Dave/Generated/UCPROBE/lib_str.d \
./Dave/Generated/UCPROBE/probe_com.d \
./Dave/Generated/UCPROBE/probe_com_os.d \
./Dave/Generated/UCPROBE/probe_rs232.d \
./Dave/Generated/UCPROBE/probe_rs232_os.d \
./Dave/Generated/UCPROBE/probe_rs232c.d \
./Dave/Generated/UCPROBE/probe_scope.d \
./Dave/Generated/UCPROBE/ucprobe.d \
./Dave/Generated/UCPROBE/ucprobe_conf.d 


# Each subdirectory must supply rules for building sources it contributes
Dave/Generated/UCPROBE/%.o: ../Dave/Generated/UCPROBE/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: ARM-GCC Assembler'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc" -MMD -MT "$@" -x assembler-with-cpp -DXMC4500_F144x1024 -I"$(PROJECT_LOC)/Libraries/XMCLib/inc" -I"$(PROJECT_LOC)" -I"$(PROJECT_LOC)/Dave/Generated" -Wall -Wa,-adhlns="$@.lst" -mfloat-abi=softfp -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $@" -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -g -gdwarf-2 -o "$@" "$<" 
	@echo 'Finished building: $<'
	@echo.
Dave/Generated/UCPROBE/%.o: ../Dave/Generated/UCPROBE/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM-GCC C Compiler'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc" -MMD -MT "$@" -DXMC4500_F144x1024 -I"$(PROJECT_LOC)/Libraries/XMCLib/inc" -I"$(PROJECT_LOC)/Libraries/CMSIS/Include" -I"$(PROJECT_LOC)/Libraries/CMSIS/Infineon/XMC4500_series/Include" -I"$(PROJECT_LOC)" -I"$(PROJECT_LOC)/Dave/Generated" -I"$(PROJECT_LOC)/Libraries" -O0 -ffunction-sections -fdata-sections -Wall -std=gnu99 -mfloat-abi=softfp -Wa,-adhlns="$@.lst" -pipe -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $@" -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -g -gdwarf-2 -o "$@" "$<" 
	@echo 'Finished building: $<'
	@echo.

