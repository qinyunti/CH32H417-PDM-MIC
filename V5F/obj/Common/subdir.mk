################################################################################
# MRS Version: 2.5.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/Common/fifo.c \
d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/Common/hardware.c \
d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/Common/pdm_mic.c \
d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/Common/sdram.c \
d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/Common/shell.c \
d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/Common/shell_func.c \
d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/Common/systick.c \
d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/Common/uart.c \
d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/Common/xmodem.c \
d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/Common/xprintf.c 

C_DEPS += \
./Common/fifo.d \
./Common/hardware.d \
./Common/pdm_mic.d \
./Common/sdram.d \
./Common/shell.d \
./Common/shell_func.d \
./Common/systick.d \
./Common/uart.d \
./Common/xmodem.d \
./Common/xprintf.d 

OBJS += \
./Common/fifo.o \
./Common/hardware.o \
./Common/pdm_mic.o \
./Common/sdram.o \
./Common/shell.o \
./Common/shell_func.o \
./Common/systick.o \
./Common/uart.o \
./Common/xmodem.o \
./Common/xprintf.o 

DIR_OBJS += \
./Common/*.o \

DIR_DEPS += \
./Common/*.d \

DIR_EXPANDS += \
./Common/*.253r.expand \


# Each subdirectory must supply rules for building sources it contributes
Common/fifo.o: d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/Common/fifo.c
	@	riscv-wch-elf-gcc -march=rv32imac_zba_zbb_zbc_zbs_xw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g -DCore_V5F -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/Common/Debug" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/SRC/Core" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/V5F/User" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/SRC/Peripheral/inc" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/Common" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/Common/memtester/inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
Common/hardware.o: d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/Common/hardware.c
	@	riscv-wch-elf-gcc -march=rv32imac_zba_zbb_zbc_zbs_xw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g -DCore_V5F -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/Common/Debug" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/SRC/Core" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/V5F/User" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/SRC/Peripheral/inc" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/Common" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/Common/memtester/inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
Common/pdm_mic.o: d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/Common/pdm_mic.c
	@	riscv-wch-elf-gcc -march=rv32imac_zba_zbb_zbc_zbs_xw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g -DCore_V5F -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/Common/Debug" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/SRC/Core" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/V5F/User" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/SRC/Peripheral/inc" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/Common" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/Common/memtester/inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
Common/sdram.o: d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/Common/sdram.c
	@	riscv-wch-elf-gcc -march=rv32imac_zba_zbb_zbc_zbs_xw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g -DCore_V5F -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/Common/Debug" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/SRC/Core" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/V5F/User" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/SRC/Peripheral/inc" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/Common" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/Common/memtester/inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
Common/shell.o: d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/Common/shell.c
	@	riscv-wch-elf-gcc -march=rv32imac_zba_zbb_zbc_zbs_xw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g -DCore_V5F -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/Common/Debug" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/SRC/Core" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/V5F/User" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/SRC/Peripheral/inc" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/Common" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/Common/memtester/inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
Common/shell_func.o: d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/Common/shell_func.c
	@	riscv-wch-elf-gcc -march=rv32imac_zba_zbb_zbc_zbs_xw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g -DCore_V5F -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/Common/Debug" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/SRC/Core" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/V5F/User" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/SRC/Peripheral/inc" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/Common" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/Common/memtester/inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
Common/systick.o: d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/Common/systick.c
	@	riscv-wch-elf-gcc -march=rv32imac_zba_zbb_zbc_zbs_xw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g -DCore_V5F -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/Common/Debug" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/SRC/Core" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/V5F/User" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/SRC/Peripheral/inc" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/Common" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/Common/memtester/inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
Common/uart.o: d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/Common/uart.c
	@	riscv-wch-elf-gcc -march=rv32imac_zba_zbb_zbc_zbs_xw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g -DCore_V5F -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/Common/Debug" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/SRC/Core" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/V5F/User" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/SRC/Peripheral/inc" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/Common" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/Common/memtester/inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
Common/xmodem.o: d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/Common/xmodem.c
	@	riscv-wch-elf-gcc -march=rv32imac_zba_zbb_zbc_zbs_xw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g -DCore_V5F -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/Common/Debug" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/SRC/Core" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/V5F/User" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/SRC/Peripheral/inc" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/Common" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/Common/memtester/inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
Common/xprintf.o: d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/Common/xprintf.c
	@	riscv-wch-elf-gcc -march=rv32imac_zba_zbb_zbc_zbs_xw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g -DCore_V5F -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/Common/Debug" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/SRC/Core" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/V5F/User" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/SRC/Peripheral/inc" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/Common" -I"d:/BaiduSyncdisk/BOARD/CH32H417QEU6-EVT-R1/proj/Common/memtester/inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

