SRC=src
OUT=out

KERNEL=demos.exe
KERNEL_DIRTY=$(KERNEL).dirty

CC=gcc
CC_INCLUDES=-I$(SRC)
CC_FLAGS=-c -ansi -m32 -g -Wall $(CC_INCLUDES)
GAS_FLAGS=-c -m32 -g -Wall $(CC_INCLUDES)
LD=/usr/bin/ld
LD_FLAGS=-T link.ld -m i386pe
PYTHON=C:/Python34/python.exe

OBJECTS=header.o main.o string.o raw_video.o cpu.o gdt.o com.o bda.o vga.o gdb_impl.o \
	interrupts_isr.o interrupts.o pic.o

FILES=$(OUT)/$(KERNEL) $(OUT)/$(KERNEL_DIRTY) \
	$(foreach o,$(OBJECTS),$(OUT)/$(o)) \
	configuration.ld $(SRC)/configuration.h

CONFIG=./config.py

INSTALL_HOST=mateusz@192.168.0.106
BOOT_DIR=/media/demos/boot
ACQUIRE_COMMAND='mount /media/demos'
RELEASE_COMMAND='umount /media/demos || true'

SSH=ssh $(INSTALL_HOST)
SCP=scp

$(OUT):
	mkdir -p $@

configuration.%:
	$(PYTHON) $(CONFIG) conf $@

%.o: $(SRC)/%.c $(SRC)/configuration.h $(OUT)
	$(CC) $(CC_FLAGS) -o $(OUT)/$@ $<

%.o: $(SRC)/%.S $(SRC)/configuration.h $(OUT)
	$(CC) $(GAS_FLAGS) -o $(OUT)/$@ $<

$(KERNEL_DIRTY): $(OBJECTS) $(SRC)/configuration.h configuration.ld $(OUT)
	$(LD) $(LD_FLAGS) -o $(OUT)/$@ $(foreach o,$(OBJECTS),$(OUT)/$(o))

$(KERNEL): $(KERNEL_DIRTY)
	objcopy -g -O elf32-i386 $(OUT)/$< $(OUT)/$@

all: $(KERNEL)

install: all
	$(SSH) $(ACQUIRE_COMMAND)
	$(SCP) $(OUT)/$(KERNEL) $(INSTALL_HOST):$(BOOT_DIR)/$(KERNEL)
	$(SCP) grub.cfg $(INSTALL_HOST):$(BOOT_DIR)/grub/grub.cfg
	$(SSH) $(RELEASE_COMMAND)

release_remote:
	$(SSH) $(RELEASE_COMMAND)

clean: release_remote
	rm -f $(FILES)
