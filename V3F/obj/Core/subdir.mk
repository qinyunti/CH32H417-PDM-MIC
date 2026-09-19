################################################################################
# MRS Version: 2.5.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/SRC/Core/core_riscv.c 

C_DEPS += \
./Core/core_riscv.d 

OBJS += \
./Core/core_riscv.o 

DIR_OBJS += \
./Core/*.o \

DIR_DEPS += \
./Core/*.d \

DIR_EXPANDS += \
./Core/*.253r.expand \


# Each subdirectory must supply rules for building sources it contributes
Core/core_riscv.o: d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/SRC/Core/core_riscv.c
	@	riscv-wch-elf-gcc -march=rv32imac_zba_zbb_zbc_zbs_xw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g -DCore_V3F -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/Debug" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/SRC/Core" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/V3F/User" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/SRC/Peripheral/inc" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/memtester" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/memtester/inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

