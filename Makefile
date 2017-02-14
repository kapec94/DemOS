SRC=src
OUT=out

OUT_DIR=out

BUILD_DIR=$(OUT_DIR)/build
INSTALL_DIR=$(OUT_DIR)/install

KERNEL=demos.exe
KERNEL_DIRTY=$(KERNEL).dirty

IMAGE=$(OUT_DIR)/demos.img

CC=gcc
CC_INCLUDES=-I$(SRC)
CC_FLAGS=-c -ansi -m32 -g -Wall $(CC_INCLUDES)
GAS_FLAGS=-c -m32 -g -Wall $(CC_INCLUDES)
LD=/usr/bin/ld
LD_FLAGS=-T link.ld -m i386pe
PYTHON=python

OBJECTS=header.o main.o string.o raw_video.o cpu.o gdt.o com.o bda.o vga.o \
	interrupts_isr.o interrupts.o pic.o ps2.o keyboard.o conio.o cmd.o

FILES=$(OUT)/$(KERNEL) $(OUT)/$(KERNEL_DIRTY) \
	$(foreach o,$(OBJECTS),$(OUT)/$(o)) \
	configuration.ld $(SRC)/configuration.h

CONFIG=./config.py

$(OUT):
	mkdir -p $@

configuration.%:
	$(PYTHON) $(CONFIG) conf $@

tools: configuration.sh


%.o: $(SRC)/%.c $(SRC)/configuration.h $(OUT)
	$(CC) $(CC_FLAGS) -o $(OUT)/$@ $<

%.o: $(SRC)/%.S $(SRC)/configuration.h $(OUT)
	$(CC) $(GAS_FLAGS) -o $(OUT)/$@ $<

$(KERNEL_DIRTY): $(OBJECTS) $(SRC)/configuration.h configuration.ld $(OUT)
	$(LD) $(LD_FLAGS) -o $(OUT)/$@ $(foreach o,$(OBJECTS),$(OUT)/$(o))

$(KERNEL): $(KERNEL_DIRTY)
	objcopy -g -O elf32-i386 $(OUT)/$< $(OUT)/$@

$(IMAGE): tools $(KERNEL)


all: $(KERNEL)

install:


clean:
	rm -f $(FILES)
