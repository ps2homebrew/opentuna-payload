LOADADDR  = 0x90000
STACKADDR = 0xA8000
STACKSIZE = 0x04000


EE_TARGET=payload
EE_BIN = $(EE_TARGET).elf
EE_BIN_STRIPPED = $(EE_TARGET)-stripped.elf
EE_BIN_PACKED= $(EE_TARGET)-packed.elf
EE_OBJS = main.o         pad.o
EE_LDFLAGS +=   -Wl,--gc-sections  -Wl,-Ttext -Wl,$(LOADADDR)  -Wl,--defsym -Wl,_stack_size=$(STACKSIZE) -Wl,--defsym -Wl,_stack=$(STACKADDR)
EE_LIBS = -lc -lpatches -lpad 
EE_INCS= -I$(GSKIT)/include -I$(PS2SDK)/ports/include
EE_CFLAGS=-Os 



all:
	$(MAKE) $(EE_BIN_PACKED) 
	

clean:
	rm -f *.elf   *.o *.s *.bin
	
			

$(EE_BIN_STRIPPED): $(EE_BIN)
	$(EE_STRIP) -s -R .comment -R .gnu.version --strip-unneeded -o $@ $<
	

	
$(EE_BIN_PACKED): $(EE_BIN_STRIPPED)
ifdef OS
	../ps2_packer/ps2_packer.exe -v $< $@
else
	wine ../ps2_packer/ps2_packer.exe -v $< $@
endif
	



	
include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/samples/Makefile.eeglobal

