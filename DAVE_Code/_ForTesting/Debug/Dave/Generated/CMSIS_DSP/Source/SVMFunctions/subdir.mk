################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Dave/Generated/CMSIS_DSP/Source/SVMFunctions/arm_svm_linear_init_f32.c \
../Dave/Generated/CMSIS_DSP/Source/SVMFunctions/arm_svm_linear_predict_f32.c \
../Dave/Generated/CMSIS_DSP/Source/SVMFunctions/arm_svm_polynomial_init_f32.c \
../Dave/Generated/CMSIS_DSP/Source/SVMFunctions/arm_svm_polynomial_predict_f32.c \
../Dave/Generated/CMSIS_DSP/Source/SVMFunctions/arm_svm_rbf_init_f32.c \
../Dave/Generated/CMSIS_DSP/Source/SVMFunctions/arm_svm_rbf_predict_f32.c \
../Dave/Generated/CMSIS_DSP/Source/SVMFunctions/arm_svm_sigmoid_init_f32.c \
../Dave/Generated/CMSIS_DSP/Source/SVMFunctions/arm_svm_sigmoid_predict_f32.c 

OBJS += \
./Dave/Generated/CMSIS_DSP/Source/SVMFunctions/arm_svm_linear_init_f32.o \
./Dave/Generated/CMSIS_DSP/Source/SVMFunctions/arm_svm_linear_predict_f32.o \
./Dave/Generated/CMSIS_DSP/Source/SVMFunctions/arm_svm_polynomial_init_f32.o \
./Dave/Generated/CMSIS_DSP/Source/SVMFunctions/arm_svm_polynomial_predict_f32.o \
./Dave/Generated/CMSIS_DSP/Source/SVMFunctions/arm_svm_rbf_init_f32.o \
./Dave/Generated/CMSIS_DSP/Source/SVMFunctions/arm_svm_rbf_predict_f32.o \
./Dave/Generated/CMSIS_DSP/Source/SVMFunctions/arm_svm_sigmoid_init_f32.o \
./Dave/Generated/CMSIS_DSP/Source/SVMFunctions/arm_svm_sigmoid_predict_f32.o 

C_DEPS += \
./Dave/Generated/CMSIS_DSP/Source/SVMFunctions/arm_svm_linear_init_f32.d \
./Dave/Generated/CMSIS_DSP/Source/SVMFunctions/arm_svm_linear_predict_f32.d \
./Dave/Generated/CMSIS_DSP/Source/SVMFunctions/arm_svm_polynomial_init_f32.d \
./Dave/Generated/CMSIS_DSP/Source/SVMFunctions/arm_svm_polynomial_predict_f32.d \
./Dave/Generated/CMSIS_DSP/Source/SVMFunctions/arm_svm_rbf_init_f32.d \
./Dave/Generated/CMSIS_DSP/Source/SVMFunctions/arm_svm_rbf_predict_f32.d \
./Dave/Generated/CMSIS_DSP/Source/SVMFunctions/arm_svm_sigmoid_init_f32.d \
./Dave/Generated/CMSIS_DSP/Source/SVMFunctions/arm_svm_sigmoid_predict_f32.d 


# Each subdirectory must supply rules for building sources it contributes
Dave/Generated/CMSIS_DSP/Source/SVMFunctions/%.o: ../Dave/Generated/CMSIS_DSP/Source/SVMFunctions/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM-GCC C Compiler'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc" -MMD -MT "$@" -DXMC4500_F100x1024 -I"$(PROJECT_LOC)/Libraries/XMCLib/inc" -I"$(PROJECT_LOC)/Libraries/CMSIS/Include" -I"$(PROJECT_LOC)/Libraries/CMSIS/Infineon/XMC4500_series/Include" -I"$(PROJECT_LOC)" -I"$(PROJECT_LOC)/Dave/Generated" -I"$(PROJECT_LOC)/Libraries" -O0 -ffunction-sections -fdata-sections -Wall -std=gnu99 -mfloat-abi=softfp -Wa,-adhlns="$@.lst" -pipe -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $@" -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -g -gdwarf-2 -o "$@" "$<" 
	@echo 'Finished building: $<'
	@echo.

