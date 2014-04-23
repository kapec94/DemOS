SRC=src
OUT=out

KERNEL=demos.exe

CC=gcc
CC_INCLUDES=-I. -I$(SRC)
CC_FLAGS=-c -m32 -g -Wall $(CC_INCLUDES)
LD=/usr/bin/ld
LD_FLAGS=-T link.ld -m i386pe --file-alignment=0 -S

OBJECTS=header.o main.o string.o raw_video.o cpu.o com.o
FILES=$(OUT)/$(KERNEL) $(foreach o,$(OBJECTS),$(OUT)/$(o)) configuration.ld $(SRC)/configuration.h

CONFIG=./config.py

INSTALL_HOST=mateusz@192.168.0.106
BOOT_DIR=/media/demos/boot
ACQUIRE_COMMAND='mount /media/demos'
RELEASE_COMMAND='umount /media/demos || true'

SSH=ssh $(INSTALL_HOST)
SCP=scp

configuration.%:
	$(CONFIG) conf $@

%.o: $(SRC)/%.c $(SRC)/configuration.h
	$(CC) $(CC_FLAGS) -o $(OUT)/$@ $<

%.o: $(SRC)/%.S $(SRC)/configuration.h
	$(CC) $(CC_FLAGS) -o $(OUT)/$@ $<

$(KERNEL): $(OBJECTS) configuration.ld
	$(LD) $(LD_FLAGS) -o $(OUT)/$@ $(foreach o,$(OBJECTS),$(OUT)/$(o))

all: $(KERNEL)

install: all
	$(SSH) $(ACQUIRE_COMMAND)
	$(SCP) $(OUT)/$(KERNEL) $(INSTALL_HOST):$(BOOT_DIR)/$(KERNEL)
	$(SSH) $(RELEASE_COMMAND)

release_remote:
	$(SSH) $(RELEASE_COMMAND)

clean: release_remote
	rm -f $(FILES)