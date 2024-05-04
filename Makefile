SHELL := /bin/bash
#MAKEFLAGS := --jobs=1
MAKEFLAGS := --jobs=$(shell nproc --ignore 1)
MAKEFLAGS += --output-sync=target
MAKEFLAGS += --load-average=$(shell nproc)
######################################################
#         Makefile pro čísté SDCC
######################################################
#
#### STM8 device LowerCase
#
#DEVICE_FLASH=stm8s003k3
#DEVICE_FLASH=stm8s103f3
#DEVICE_FLASH=stm8s105c6
DEVICE_FLASH=stm8s208rb

### STlink version for falsh2 targer (stm8flash program)
#STLINK=stlink
#STLINK=stlinkv2
STLINK=stlinkv21

F_CPU=16000000

ifeq ($(OS),Windows_NT)
	CC_ROOT = "/c/Program Files/SDCC"
	STVP_ROOT = "/c/Program Files (x86)/STMicroelectronics/st_toolset/stvp"
else
	#CC_ROOT = /usr
	#CC_ROOT = /usr/local
	CC_ROOT = /usr/local/stow/sdcc-4.4.0
	#CC_ROOT = /usr/local/stow/sdcc-gas
endif
######################################################################################
CC = $(CC_ROOT)/bin/sdcc
AR = $(CC_ROOT)/bin/sdar
ifeq ($(wildcard $(CC)),)
	CC = sdcc
endif
ifeq ($(wildcard $(AR)),)
	AR = sdar
endif

#DEVICE=STM8S103
DEVICE=$(shell bash .make/device $(DEVICE_FLASH))
DEVICE_FLASH_STVP=$(shell bash .make/device_stvp $(DEVICE_FLASH))

MKDIR = mkdir
CP = cp

ifeq ($(findstring 2019, $(shell openocd --version 2>&1)), 2019)
    interface = interface/stlink.cfg
else
    interface = interface/stlink-dap.cfg
endif
ifeq ($(findstring 003, $(DEVICE)), 003)
	target = target/stm8s003.cfg
else ifeq ($(findstring 103, $(DEVICE)), 103)
	target = target/stm8s103.cfg
else ifeq ($(findstring 105, $(DEVICE)), 105)
	target = target/stm8s105.cfg
else
	target = target/stm8s.cfg
endif
OPENOCD = openocd -f .make/stm8s-flash.cfg -f $(interface) -f $(target)


ifeq ($(OS),Windows_NT)
    uname_S := Windows

	LN = cp
	PYTHON = python
	STVP = $(STVP_ROOT)/STVP_CmdLine.exe
else
    uname_S := $(shell uname -s)

	LN = ln -sf
	PYTHON = python3
endif

# trap handling requires SDCC >=v3.4.3
SKIP_TRAPS = 1

# set compiler path & parameters 
#CFLAGS  = -mstm8 -lstm8 --opt-code-size 
CFLAGS  = -mstm8 -lstm8 --opt-code-size --nogcse --all-callee-saves --stack-auto --fverbose-asm --float-reent --no-peep
#CFLAGS += --std-sdcc99 
CFLAGS += --std-sdcc2x
#CFLAGS += --disable-warning 283
CFLAGS += -I inc
CFLAGS += -D F_CPU=$(F_CPU)


ifeq ($(IHEX_NODEBUG),)
	FMT_ELF = --debug --out-fmt-elf
endif

# required for some examples for STM8S EVAL board
#CFLAGS += -DUSE_STM8_128_EVAL

# set output folder and target name
OUTPUT_DIR = ./build-$(DEVICE)
OUTPUT_DIR_ELF = ./build-$(DEVICE)-elf
TARGET     = $(OUTPUT_DIR)/out.ihx
TARGET_ELF     = $(OUTPUT_DIR_ELF)/out.elf
OUTPUT_SPL_DIR = ../build_SPLlib.ihx-$(DEVICE)
OUTPUT_SPL_DIR_ELF = ../build_SPLlib.elf-$(DEVICE)
SPLLIB = ../SPLSPL/spl-$(DEVICE).ihx.lib
SPLLIB_ELF = ../SPLSPL/spl-$(DEVICE).elf.lib

# set project folder and files (all *.c)
PRJ_ROOT    = .
PRJ_SRC_DIR = $(PRJ_ROOT)/src
PRJ_INC_DIR = $(PRJ_ROOT)/inc
# all project sources included by default
PRJ_SOURCE  = $(notdir $(wildcard $(PRJ_SRC_DIR)/*.c))
PRJ_OBJECTS := $(addprefix $(OUTPUT_DIR)/, $(PRJ_SOURCE:.c=.rel))
PRJ_OBJECTS_ELF := $(addprefix $(OUTPUT_DIR_ELF)/, $(PRJ_SOURCE:.c=.rel))
PRJ_INCS  = $(wildcard $(PRJ_INC_DIR)/*.h)

# set SPL paths
SPL_SRC_DIR = ../SPLSPL/src/
SPL_INC_DIR = ../SPLSPL/inc/
#SPL_SRC_DIR = ../SPL-$(DEVICE)/src/
#SPL_INC_DIR = ../SPL-$(DEVICE)/inc/
# add all library sources used here singly ... or all .c files in SPL src dir
#SPL_SOURCE  = stm8s_gpio.c stm8s_clk.c stm8s_tim4.c stm8s_itc.c 
#SPL_SOURCE += stm8s_uart1.c
#SPL_SOURCE += stm8s_adc2.c
#SPL_SOURCE += stm8s_flash.c
#SPL_SOURCE += stm8s_tim1.c
#SPL_SOURCE += stm8s_tim2.c
#SPL_SOURCE += stm8s_tim3.c
#SPL_SOURCE  = $(notdir $(wildcard $(SPL_SRC_DIR)/*.c))
ifneq ($(wildcard ../SPLSPL),)
	include ../SPLSPL/mk/$(DEVICE).mk
else
start: spl
endif

SPL_SOURCE = $(foreach prefix, $(SPL_SOURCE_PREFIX), $(notdir $(wildcard $(SPL_SRC_DIR)/$(prefix)*.c)))
SPL_OBJECTS := $(addprefix $(OUTPUT_SPL_DIR)/, $(SPL_SOURCE:.c=.rel))
SPL_OBJECTS_ELF := $(addprefix $(OUTPUT_SPL_DIR_ELF)/, $(SPL_SOURCE:.c=.rel))

# collect all include folders
INCLUDE = -I$(PRJ_INC_DIR) -I$(SPL_INC_DIR) 

# collect all source directories
VPATH=$(PRJ_SRC_DIR):$(SPL_SRC_DIR)

.PHONY: ihx elf all

ihx: $(TARGET)
elf: $(TARGET_ELF)
all: ihx elf

$(TARGET_ELF): $(PRJ_OBJECTS_ELF)  $(SPLLIB_ELF)
	$(CC) $(CFLAGS) $(FMT_ELF) -o $(TARGET_ELF) $^
	$(LN) $@ ./out.elf || $(CP) $@ ./out.elf

$(TARGET): $(PRJ_OBJECTS)  $(SPLLIB)
	$(CC) $(CFLAGS) -o $(TARGET) $^
	$(LN) $@ ./out.ihx || $(CP) $@ ./out.ihx
	@echo
	@$(PYTHON) .make/sizecal.py ./out.ihx
	@echo

$(OUTPUT_DIR)/%.rel: %.c $(PRJ_INCS) Makefile | $(OUTPUT_DIR) ../SPLSPL
	$(CC) $(CFLAGS) -D$(DEVICE) $(INCLUDE) -DSKIP_TRAPS=$(SKIP_TRAPS) -c $< -o $@ 2>&1 | $(PYTHON) .make/recolor.py; exit $${PIPESTATUS[0]}
$(OUTPUT_DIR_ELF)/%.rel: %.c $(PRJ_INCS) Makefile | $(OUTPUT_DIR_ELF) ../SPLSPL
	$(CC) $(CFLAGS) $(FMT_ELF) -D$(DEVICE) $(INCLUDE) -DSKIP_TRAPS=$(SKIP_TRAPS) -c $< -o $@ 2>&1 | $(PYTHON) .make/recolor.py; exit $${PIPESTATUS[0]}

$(OUTPUT_SPL_DIR)/%.rel: %.c $(PRJ_INCS) Makefile | $(OUTPUT_SPL_DIR) ../SPLSPL
	$(CC) $(CFLAGS) -D$(DEVICE) $(INCLUDE) -DSKIP_TRAPS=$(SKIP_TRAPS) -c $< -o $@ 2>&1 | $(PYTHON) .make/recolor.py; exit $${PIPESTATUS[0]}
$(OUTPUT_SPL_DIR_ELF)/%.rel: %.c $(PRJ_INCS) Makefile | $(OUTPUT_SPL_DIR_ELF) ../SPLSPL
	$(CC) $(CFLAGS) $(FMT_ELF) -D$(DEVICE) $(INCLUDE) -DSKIP_TRAPS=$(SKIP_TRAPS) -c $< -o $@ 2>&1 | $(PYTHON) .make/recolor.py; exit $${PIPESTATUS[0]}


ifeq ($(wildcard $(SPLLIB)),)
$(SPLLIB): $(SPL_OBJECTS) | $(OUTPUT_SPL_DIR)
	$(AR) -rc $(SPLLIB) $^
endif
ifeq ($(wildcard $(SPLLIB_ELF)),)
$(SPLLIB_ELF): $(SPL_OBJECTS_ELF) | $(OUTPUT_SPL_DIR_ELF)
	$(AR) -rc $(SPLLIB_ELF) $^
endif


$(OUTPUT_DIR):
	$(MKDIR) -p $(OUTPUT_DIR)
$(OUTPUT_DIR_ELF):
	$(MKDIR) -p $(OUTPUT_DIR_ELF)
$(OUTPUT_SPL_DIR):
	$(MKDIR) -p $@
$(OUTPUT_SPL_DIR_ELF):
	$(MKDIR) -p $@


compile_commands.json: $(PRJ_SRC_DIR) $(SPL_SRC_DIR)
	$(PYTHON) .make/compile_database_generator.py $(DEVICE)

.PHONY: clean clean-spl-objects clean-spl-lib  cleanall

clean:
	rm -f ./out.ihx
	rm -f ./out.elf
	rm -f out-$(DEVICE).ihex
	rm -Rf $(OUTPUT_DIR)
	rm -Rf $(OUTPUT_DIR_ELF)

clean-spl-objects:
	rm -Rf $(OUTPUT_SPL_DIR)
	rm -Rf $(OUTPUT_SPL_DIR_ELF)

clean-spl-lib: clean-spl-objects
	rm -f $(SPLLIB)
	rm -f $(SPLLIB_ELF)

cleanall: clean-spl-objects clean 


.PHONY: flash flash-elf flash-ihex stm8flash rebuild reflash

flash: $(TARGET)
ifeq ($(OS),Windows_NT)
	+$(STVP) -BoardName=ST-LINK -Device=$(DEVICE_FLASH_STVP) -Port=USB -ProgMode=SWIM -no_loop -no_log -FileProg=$(TARGET)
else
	+$(OPENOCD) -c "init" -c "program_device $(TARGET) 0"
endif

flash-elf: $(TARGET_ELF)
	+$(OPENOCD) -c "init" -c "program_device $(TARGET_ELF) 0"
flash-ihex: $(TARGET_ELF)
	stm8-objcopy -O ihex $(TARGET_ELF) out-$(DEVICE).ihex
	@echo
	@$(PYTHON) .make/sizecal.py ./out-$(DEVICE).ihex
	@echo
	+$(OPENOCD) -c "init" -c "program_device out-$(DEVICE).ihex 0"

stm8flash: $(TARGET)
	+stm8flash -c $(STLINK) -p $(DEVICE_FLASH) -s flash -w out.ihx

rebuild: clean
	$(MAKE) elf

reflash: clean
	$(MAKE) flash


.PHONY: openocd debug switch-sdccrm switch-sdcc-gas 

openocd:
	+$(OPENOCD) -c "init" -c "reset halt"

debug: $(TARGET_ELF)
	+stm8-gdb --tui $^

switch-device: clean
	+bash .make/switch_device.sh

switch-sdcc-gas: clean
	$(LN) .make/Makefile-sdcc-gas Makefile || $(CP) .make/Makefile-sdcc-gas Makefile
switch-sdccrm: clean
	$(LN) .make/Makefile-sdccrm Makefile || $(CP) .make/Makefile-sdccrm Makefile

../SPL.tgz:
	curl https://gitlab.com/spseol/mit-no/spl/-/archive/main/spl-main.tar.gz --output ../SPL.tgz
../SPL: | ../SPL.tgz
	tar -C .. --transform='s%spl-main/%%' -xzf ../SPL.tgz spl-main/SPL/
	@printf "***********************************************************\n*  SPL byla rozbalena. Možná je třeba znovu spusti make   *\n***********************************************************\n"
../SPLSPL: | ../SPL.tgz
	tar -C .. --transform='s%spl-main/%%' -xzf ../SPL.tgz spl-main/SPLSPL/
	@printf "***********************************************************\n*  SPL byla rozbalena. Možná je třeba znovu spusti make   *\n***********************************************************\n"

.PHONY: spl spl-renew tree

spl: ../SPL ../SPLSPL

spl-renew:
	rm -Rf ../SPL* 
	$(MAKE) spl

tree:
	+bash .make/tree.sh

