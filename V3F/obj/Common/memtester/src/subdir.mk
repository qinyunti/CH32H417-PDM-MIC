################################################################################
# MRS Version: 2.5.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/memtester/src/memtester.c \
d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/memtester/src/memtester_tests.c 

C_DEPS += \
./Common/memtester/src/memtester.d \
./Common/memtester/src/memtester_tests.d 

OBJS += \
./Common/memtester/src/memtester.o \
./Common/memtester/src/memtester_tests.o 

DIR_OBJS += \
./Common/memtester/src/*.o \

DIR_DEPS += \
./Common/memtester/src/*.d \

DIR_EXPANDS += \
./Common/memtester/src/*.253r.expand \


# Each subdirectory must supply rules for building sources it contributes
Common/memtester/src/memtester.o: d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/memtester/src/memtester.c
	@	riscv-wch-elf-gcc -march=rv32imac_zba_zbb_zbc_zbs_xw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g -DCore_V3F -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/Debug" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/SRC/Core" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/V3F/User" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/SRC/Peripheral/inc" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/memtester" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/memtester/inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
Common/memtester/src/memtester_tests.o: d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/memtester/src/memtester_tests.c
	@	riscv-wch-elf-gcc -march=rv32imac_zba_zbb_zbc_zbs_xw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g -DCore_V3F -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/Debug" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/SRC/Core" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/V3F/User" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/SRC/Peripheral/inc" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/memtester" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/memtester/inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

