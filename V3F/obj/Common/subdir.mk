################################################################################
# MRS Version: 2.5.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/fifo.c \
d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/hardware.c \
d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/pdm_mic.c \
d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/sai_spk.c \
d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/sdram.c \
d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/shell.c \
d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/shell_func.c \
d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/systick.c \
d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/uart.c \
d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/wav.c \
d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/xmodem.c \
d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/xprintf.c 

C_DEPS += \
./Common/fifo.d \
./Common/hardware.d \
./Common/pdm_mic.d \
./Common/sai_spk.d \
./Common/sdram.d \
./Common/shell.d \
./Common/shell_func.d \
./Common/systick.d \
./Common/uart.d \
./Common/wav.d \
./Common/xmodem.d \
./Common/xprintf.d 

OBJS += \
./Common/fifo.o \
./Common/hardware.o \
./Common/pdm_mic.o \
./Common/sai_spk.o \
./Common/sdram.o \
./Common/shell.o \
./Common/shell_func.o \
./Common/systick.o \
./Common/uart.o \
./Common/wav.o \
./Common/xmodem.o \
./Common/xprintf.o 

DIR_OBJS += \
./Common/*.o \

DIR_DEPS += \
./Common/*.d \

DIR_EXPANDS += \
./Common/*.253r.expand \


# Each subdirectory must supply rules for building sources it contributes
Common/fifo.o: d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/fifo.c
	@	riscv-wch-elf-gcc -march=rv32imac_zba_zbb_zbc_zbs_xw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g -DCore_V3F -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/Debug" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/SRC/Core" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/V3F/User" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/SRC/Peripheral/inc" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/memtester" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/memtester/inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
Common/hardware.o: d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/hardware.c
	@	riscv-wch-elf-gcc -march=rv32imac_zba_zbb_zbc_zbs_xw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g -DCore_V3F -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/Debug" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/SRC/Core" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/V3F/User" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/SRC/Peripheral/inc" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/memtester" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/memtester/inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
Common/pdm_mic.o: d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/pdm_mic.c
	@	riscv-wch-elf-gcc -march=rv32imac_zba_zbb_zbc_zbs_xw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g -DCore_V3F -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/Debug" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/SRC/Core" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/V3F/User" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/SRC/Peripheral/inc" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/memtester" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/memtester/inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
Common/sai_spk.o: d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/sai_spk.c
	@	riscv-wch-elf-gcc -march=rv32imac_zba_zbb_zbc_zbs_xw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g -DCore_V3F -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/Debug" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/SRC/Core" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/V3F/User" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/SRC/Peripheral/inc" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/memtester" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/memtester/inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
Common/sdram.o: d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/sdram.c
	@	riscv-wch-elf-gcc -march=rv32imac_zba_zbb_zbc_zbs_xw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g -DCore_V3F -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/Debug" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/SRC/Core" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/V3F/User" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/SRC/Peripheral/inc" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/memtester" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/memtester/inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
Common/shell.o: d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/shell.c
	@	riscv-wch-elf-gcc -march=rv32imac_zba_zbb_zbc_zbs_xw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g -DCore_V3F -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/Debug" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/SRC/Core" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/V3F/User" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/SRC/Peripheral/inc" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/memtester" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/memtester/inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
Common/shell_func.o: d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/shell_func.c
	@	riscv-wch-elf-gcc -march=rv32imac_zba_zbb_zbc_zbs_xw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g -DCore_V3F -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/Debug" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/SRC/Core" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/V3F/User" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/SRC/Peripheral/inc" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/memtester" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/memtester/inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
Common/systick.o: d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/systick.c
	@	riscv-wch-elf-gcc -march=rv32imac_zba_zbb_zbc_zbs_xw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g -DCore_V3F -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/Debug" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/SRC/Core" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/V3F/User" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/SRC/Peripheral/inc" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/memtester" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/memtester/inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
Common/uart.o: d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/uart.c
	@	riscv-wch-elf-gcc -march=rv32imac_zba_zbb_zbc_zbs_xw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g -DCore_V3F -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/Debug" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/SRC/Core" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/V3F/User" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/SRC/Peripheral/inc" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/memtester" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/memtester/inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
Common/wav.o: d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/wav.c
	@	riscv-wch-elf-gcc -march=rv32imac_zba_zbb_zbc_zbs_xw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g -DCore_V3F -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/Debug" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/SRC/Core" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/V3F/User" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/SRC/Peripheral/inc" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/memtester" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/memtester/inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
Common/xmodem.o: d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/xmodem.c
	@	riscv-wch-elf-gcc -march=rv32imac_zba_zbb_zbc_zbs_xw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g -DCore_V3F -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/Debug" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/SRC/Core" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/V3F/User" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/SRC/Peripheral/inc" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/memtester" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/memtester/inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
Common/xprintf.o: d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/xprintf.c
	@	riscv-wch-elf-gcc -march=rv32imac_zba_zbb_zbc_zbs_xw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g -DCore_V3F -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/Debug" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/SRC/Core" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/V3F/User" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/SRC/Peripheral/inc" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/memtester" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/CH32H417-PDM-MIC/Common/memtester/inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

