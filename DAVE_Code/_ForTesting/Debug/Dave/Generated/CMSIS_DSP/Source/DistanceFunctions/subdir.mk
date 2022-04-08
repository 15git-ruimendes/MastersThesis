################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Dave/Generated/CMSIS_DSP/Source/DistanceFunctions/arm_boolean_distance.c \
../Dave/Generated/CMSIS_DSP/Source/DistanceFunctions/arm_braycurtis_distance_f32.c \
../Dave/Generated/CMSIS_DSP/Source/DistanceFunctions/arm_canberra_distance_f32.c \
../Dave/Generated/CMSIS_DSP/Source/DistanceFunctions/arm_chebyshev_distance_f32.c \
../Dave/Generated/CMSIS_DSP/Source/DistanceFunctions/arm_cityblock_distance_f32.c \
../Dave/Generated/CMSIS_DSP/Source/DistanceFunctions/arm_correlation_distance_f32.c \
../Dave/Generated/CMSIS_DSP/Source/DistanceFunctions/arm_cosine_distance_f32.c \
../Dave/Generated/CMSIS_DSP/Source/DistanceFunctions/arm_dice_distance.c \
../Dave/Generated/CMSIS_DSP/Source/DistanceFunctions/arm_euclidean_distance_f32.c \
../Dave/Generated/CMSIS_DSP/Source/DistanceFunctions/arm_hamming_distance.c \
../Dave/Generated/CMSIS_DSP/Source/DistanceFunctions/arm_jaccard_distance.c \
../Dave/Generated/CMSIS_DSP/Source/DistanceFunctions/arm_jensenshannon_distance_f32.c \
../Dave/Generated/CMSIS_DSP/Source/DistanceFunctions/arm_kulsinski_distance.c \
../Dave/Generated/CMSIS_DSP/Source/DistanceFunctions/arm_minkowski_distance_f32.c \
../Dave/Generated/CMSIS_DSP/Source/DistanceFunctions/arm_rogerstanimoto_distance.c \
../Dave/Generated/CMSIS_DSP/Source/DistanceFunctions/arm_russellrao_distance.c \
../Dave/Generated/CMSIS_DSP/Source/DistanceFunctions/arm_sokalmichener_distance.c \
../Dave/Generated/CMSIS_DSP/Source/DistanceFunctions/arm_sokalsneath_distance.c \
../Dave/Generated/CMSIS_DSP/Source/DistanceFunctions/arm_yule_distance.c 

OBJS += \
./Dave/Generated/CMSIS_DSP/Source/DistanceFunctions/arm_boolean_distance.o \
./Dave/Generated/CMSIS_DSP/Source/DistanceFunctions/arm_braycurtis_distance_f32.o \
./Dave/Generated/CMSIS_DSP/Source/DistanceFunctions/arm_canberra_distance_f32.o \
./Dave/Generated/CMSIS_DSP/Source/DistanceFunctions/arm_chebyshev_distance_f32.o \
./Dave/Generated/CMSIS_DSP/Source/DistanceFunctions/arm_cityblock_distance_f32.o \
./Dave/Generated/CMSIS_DSP/Source/DistanceFunctions/arm_correlation_distance_f32.o \
./Dave/Generated/CMSIS_DSP/Source/DistanceFunctions/arm_cosine_distance_f32.o \
./Dave/Generated/CMSIS_DSP/Source/DistanceFunctions/arm_dice_distance.o \
./Dave/Generated/CMSIS_DSP/Source/DistanceFunctions/arm_euclidean_distance_f32.o \
./Dave/Generated/CMSIS_DSP/Source/DistanceFunctions/arm_hamming_distance.o \
./Dave/Generated/CMSIS_DSP/Source/DistanceFunctions/arm_jaccard_distance.o \
./Dave/Generated/CMSIS_DSP/Source/DistanceFunctions/arm_jensenshannon_distance_f32.o \
./Dave/Generated/CMSIS_DSP/Source/DistanceFunctions/arm_kulsinski_distance.o \
./Dave/Generated/CMSIS_DSP/Source/DistanceFunctions/arm_minkowski_distance_f32.o \
./Dave/Generated/CMSIS_DSP/Source/DistanceFunctions/arm_rogerstanimoto_distance.o \
./Dave/Generated/CMSIS_DSP/Source/DistanceFunctions/arm_russellrao_distance.o \
./Dave/Generated/CMSIS_DSP/Source/DistanceFunctions/arm_sokalmichener_distance.o \
./Dave/Generated/CMSIS_DSP/Source/DistanceFunctions/arm_sokalsneath_distance.o \
./Dave/Generated/CMSIS_DSP/Source/DistanceFunctions/arm_yule_distance.o 

C_DEPS += \
./Dave/Generated/CMSIS_DSP/Source/DistanceFunctions/arm_boolean_distance.d \
./Dave/Generated/CMSIS_DSP/Source/DistanceFunctions/arm_braycurtis_distance_f32.d \
./Dave/Generated/CMSIS_DSP/Source/DistanceFunctions/arm_canberra_distance_f32.d \
./Dave/Generated/CMSIS_DSP/Source/DistanceFunctions/arm_chebyshev_distance_f32.d \
./Dave/Generated/CMSIS_DSP/Source/DistanceFunctions/arm_cityblock_distance_f32.d \
./Dave/Generated/CMSIS_DSP/Source/DistanceFunctions/arm_correlation_distance_f32.d \
./Dave/Generated/CMSIS_DSP/Source/DistanceFunctions/arm_cosine_distance_f32.d \
./Dave/Generated/CMSIS_DSP/Source/DistanceFunctions/arm_dice_distance.d \
./Dave/Generated/CMSIS_DSP/Source/DistanceFunctions/arm_euclidean_distance_f32.d \
./Dave/Generated/CMSIS_DSP/Source/DistanceFunctions/arm_hamming_distance.d \
./Dave/Generated/CMSIS_DSP/Source/DistanceFunctions/arm_jaccard_distance.d \
./Dave/Generated/CMSIS_DSP/Source/DistanceFunctions/arm_jensenshannon_distance_f32.d \
./Dave/Generated/CMSIS_DSP/Source/DistanceFunctions/arm_kulsinski_distance.d \
./Dave/Generated/CMSIS_DSP/Source/DistanceFunctions/arm_minkowski_distance_f32.d \
./Dave/Generated/CMSIS_DSP/Source/DistanceFunctions/arm_rogerstanimoto_distance.d \
./Dave/Generated/CMSIS_DSP/Source/DistanceFunctions/arm_russellrao_distance.d \
./Dave/Generated/CMSIS_DSP/Source/DistanceFunctions/arm_sokalmichener_distance.d \
./Dave/Generated/CMSIS_DSP/Source/DistanceFunctions/arm_sokalsneath_distance.d \
./Dave/Generated/CMSIS_DSP/Source/DistanceFunctions/arm_yule_distance.d 


# Each subdirectory must supply rules for building sources it contributes
Dave/Generated/CMSIS_DSP/Source/DistanceFunctions/%.o: ../Dave/Generated/CMSIS_DSP/Source/DistanceFunctions/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM-GCC C Compiler'
	"$(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc" -MMD -MT "$@" -DXMC4500_F100x1024 -I"$(PROJECT_LOC)/Libraries/XMCLib/inc" -I"$(PROJECT_LOC)/Libraries/CMSIS/Include" -I"$(PROJECT_LOC)/Libraries/CMSIS/Infineon/XMC4500_series/Include" -I"$(PROJECT_LOC)" -I"$(PROJECT_LOC)/Dave/Generated" -I"$(PROJECT_LOC)/Libraries" -O0 -ffunction-sections -fdata-sections -Wall -std=gnu99 -mfloat-abi=softfp -Wa,-adhlns="$@.lst" -pipe -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $@" -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -g -gdwarf-2 -o "$@" "$<" 
	@echo 'Finished building: $<'
	@echo.

