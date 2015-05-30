# Makefile for ARM target
	CROSS_CC=/usr/local/arm/4.1.0/bin/arm-elf-
#	CROSS_CC=/usr/local/arm/3.4.1/bin/arm-linux-
	LDSCRIPTS = at91.ld
# Define directories.
	uSMARTX_INC_DIR = ../inc
	uSMARTX_LIB_DIR = ../arm/gcc/lib
	uSMARTXLIB = usmartx
	AT_DIR = atmel-arch
	SKYIO_DIR = skyeye-stdio
# Define programs.
    SHELL = sh
    AR = $(CROSS_CC)ar
    COMPILE = $(CROSS_CC)gcc
    ASSEMBLE = $(CROSS_CC)gcc -x assembler-with-cpp
    REMOVE = rm -f
    COPY = cp
    MOVE = mv
    OBJCOPY = $(CROSS_CC)objcopy
    OBJDUMP = $(CROSS_CC)objdump
    HEXSIZE = @$(CROS_CC)size --target=$(FORMAT) $(TARGET).hex
    ELFSIZE = @$(CROSS_CC)size $(TARGET).elf

# MCU name
	MCU = arm7tdmi

# Output format. Can be [srec|ihex].
    FORMAT = ihex

# Target file name (without extension).
	TARGET = hello

# List C source files here.
	SRC = $(TARGET).c $(SKYIO_DIR)/skyeye_printf.c $(SKYIO_DIR)/skyeye_stdio.c \
		$(AT_DIR)/at91_aic.c $(AT_DIR)/at91_tc.c \
		$(AT_DIR)/at91_usart.c $(AT_DIR)/at91_init.c 
			
# List Assembler source files here.
	ASRC =  crt0.s

# Compiler flags.
	CPFLAGS = -gdwarf-2 -Os -Wall -Wa,-ahlms=$(<:.c=.lst) -I$(AT_DIR) -I$(SKYIO_DIR) \
		-I.

# Assembler flags.
    ASFLAGS = -Wa,-ahlms=$(<:.s=.lst),--gstabs 

# Linker flags (passed via GCC).
	LDFLAGS = -L$(uSMARTX_LIB_DIR) -nostartfiles \
		    -Wl,-Map=$(TARGET).map,--cref -Wl,--no-warn-mismatch \
		    -T$(LDSCRIPTS) 

# Additional library flags (-lm = math library).
	LIBFLAGS = -lc -lgcc -l$(uSMARTXLIB) 

# Define all project specific object files.
	OBJ	= $(SRC:.c=.o) $(ASRC:.s=.o) 	
	
# Define all listing files.
	LST = $(ASRC:.s=.lst) $(SRC:.c=.lst)
	
# Compiler flags to generate dependency files.
	GENDEPFLAGS = -Wp,-M,-MP,-MT,$(*F).o,-MF,.dep/$(@F).d

# Add target processor to flags.
   	CPFLAGS += -mcpu=$(MCU) $(GENDEPFLAGS)
	ASFLAGS += -mcpu=$(MCU)
	LDFLAGS += -mcpu=$(MCU)	
  
.PHONY : Build
Build: $(TARGET).elf $(TARGET).hex $(TARGET).lss $(TARGET).bin line1 overallsize line2

.PHONY : BuildAll
BuildAll: clean $(TARGET).elf $(TARGET).hex $(TARGET).lss $(TARGET).bin line1 overallsize line2

.PHONY : overallsize
overallsize:
	@echo Elf size:
	$(ELFSIZE)
	
%.bin: %.elf
	$(OBJCOPY) -O binary $< $@

# Create final output files (.hex, .lss) from ELF output file.
%.hex: %.elf
	$(OBJCOPY) -O $(FORMAT) $< $@

# Create extended listing file from ELF output file.
%.lss: %.elf
	$(OBJDUMP) -h -S $< > $@

# Link: create ELF output file from object files.
.SECONDARY : $(TRT).elf
.PRECIOUS : $(OBJ)
$(TARGET).elf: $(OBJ)
	$(COMPILE) $(LDFLAGS) $(OBJ) $(LIBFLAGS) --output $@

# Compile: create object files from C source files.
%.o : %.c
	$(COMPILE) -c $(CPFLAGS) -I$(uSMARTX_INC_DIR) $< -o $@ 

# Assemble: create object files from assembler files.

%.o : %.s
	$(ASSEMBLE) -c $(ASFLAGS) $< -o $@

# Target: line1 project.
.PHONY : line1
line1 :
	@echo ---------------------------------------------------------------------------------------------------

# Target: line2 project.
.PHONY : line2
line2 :
	@echo ---------------------------------------------------------------------------------------------------

# Target: clean project.
.SILENT : Clean
.PHONY : Clean
Clean :
	$(REMOVE) $(TARGET).hex
	$(REMOVE) $(TARGET).obj
	$(REMOVE) $(TARGET).elf
	$(REMOVE) $(TARGET).bin
	$(REMOVE) $(TARGET).map
	$(REMOVE) $(TARGET).o
	$(REMOVE) $(TARGET).sym
	$(REMOVE) $(TARGET).lnk
	$(REMOVE) $(TARGET).lss
	$(REMOVE) $(OBJ)
	$(REMOVE) $(LST)
	$(REMOVE) $(SRC:.c=.s)

# Target copy output files
.SILENT : copy_output
.PHONY: copy_output
copy_output :
	$(COPY)  $(TARGET).hex obj/$(TARGET).hex
	$(COPY)  $(TARGET).elf obj/$(TARGET).elf
	$(COPY)  $(TARGET).bin obj/$(TARGET).bin
	
# Include the dependency files.
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)


# List assembly only source file dependencies here:

