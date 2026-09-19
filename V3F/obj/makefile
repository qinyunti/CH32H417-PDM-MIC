################################################################################
# MRS Version: 2.5.0
# Automatically-generated file. Do not edit!
################################################################################
-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include User/subdir.mk
-include Startup/subdir.mk
-include Peripheral/src/subdir.mk
-include Core/subdir.mk
-include Common/memtester/src/subdir.mk
-include Common/Debug/subdir.mk
-include Common/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
test_V3F.bin \
test_V3F.hex \

SECONDARY_LIST += \
test_V3F.lst \

SECONDARY_SIZE += \
test_V3F.siz \


# All Target
all: 
	$(MAKE) --no-print-directory main-build 

main-build: test_V3F.elf secondary-outputs

# Tool invocations
test_V3F.elf: $(OBJS) $(USER_OBJS_ESCAPE)
	@	riscv-wch-elf-gcc -march=rv32imac_zba_zbb_zbc_zbs_xw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g -T "d:\BaiduSyncdisk\BOARD\CH32H417QEU6-EVT-R1\proj\CH32H417-PDM-MIC\SRC\Ld\V3F\Link_v3f.ld" -nostartfiles -Xlinker --gc-sections -Wl,-Map,"test_V3F.map" --specs=nano.specs --specs=nosys.specs -o "test_V3F.elf" $(OBJS) $(LIBS)
test_V3F.bin: test_V3F.elf
	@	riscv-wch-elf-objcopy -O binary "test_V3F.elf" "test_V3F.bin"
test_V3F.hex: test_V3F.elf
	@	riscv-wch-elf-objcopy -O ihex "test_V3F.elf" "test_V3F.hex"
test_V3F.lst: test_V3F.elf
	@	riscv-wch-elf-objdump --all-headers --demangle --disassemble -M xw "test_V3F.elf" > "test_V3F.lst"
test_V3F.siz: test_V3F.elf
	riscv-wch-elf-size --format=berkeley "test_V3F.elf"

# Other Targets
clean:
	-$(RM) $(DIR_OBJS) $(SECONDARY_FLASH)$(SECONDARY_LIST)$(SECONDARY_SIZE) test_V3F.elf
	-$(RM) $(DIR_EXPANDS) $(CALLGRAPH_DOT)
	-$(RM) $(DIR_DEPS)
secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents

-include ../makefile.targets