# Makefile for ARM target

# Define directories.
	uSMARTX_INC_DIR = ../../inc
	uSMARTX_LIB_DIR = ../../avr/mega64/lib
	uSMARTXLIB = usmartx
	
# Define programs.
    SHELL = sh
    AR = ar
    COMPILE = avr-gcc
    ASSEMBLE = avr-gcc -x assembler-with-cpp
    REMOVE = rm -f
    COPY = cp
    MOVE = mv
    OBJCOPY = avr-objcopy
    OBJDUMP = avr-objdump
    HEXSIZE = @avr-size --target=$(FORMAT) $(TARGET).hex
    ELFSIZE = @avr-size $(TARGET).elf

# MCU name
	MCU = atmega64

# Output format. Can be [srec|ihex].
    FORMAT = ihex

# Target file name (without extension).
	TARGET = buffer

# List C source files here.
	SRC = buffer.c
			
# List Assembler source files here.
	ASRC = 

# Compiler flags.
	CPFLAGS = -gdwarf-2 -Os -Wall -Wa,-ahlms=$(<:.c=.lst)

# Assembler flags.
    ASFLAGS = -Wa,-ahlms=$(<:.s=.lst),--gstabs 

# Linker flags (passed via GCC).
	LDFLAGS = -L$(uSMARTX_LIB_DIR) -Wl,-Map=$(TARGET).map,--cref

# Additional library flags (-lm = math library).
	LIBFLAGS = -l$(uSMARTXLIB)

# Define all project specific object files.
	OBJ	= $(SRC:.c=.o) $(ASRC:.s=.o) 	
	
# Define all listing files.
	LST = $(ASRC:.s=.lst) $(SRC:.c=.lst)
	
# Compiler flags to generate dependency files.
	GENDEPFLAGS = -Wp,-M,-MP,-MT,$(*F).o,-MF,.dep/$(@F).d

# Add target processor to flags.
   	CPFLAGS += -mmcu=$(MCU) $(GENDEPFLAGS)
	ASFLAGS += -mmcu=$(MCU)
	LDFLAGS += -mmcu=$(MCU)	
  
.PHONY : Build
Build: $(TARGET).elf $(TARGET).hex $(TARGET).lss $(TARGET).bin copy_output line1 overallsize line2

.PHONY : BuildAll
BuildAll: clean $(TARGET).elf $(TARGET).hex $(TARGET).lss $(TARGET).bin  copy_output line1 overallsize line2

.PHONY : overallsize
overallsize:
	@echo Elf size:
	$(ELFSIZE)
	
%.bin: %.elf
	$(OBJCOPY) -j .text -j .data -O binary $< $@

# Create final output files (.hex, .lss) from ELF output file.
%.hex: %.elf
	$(OBJCOPY) -O $(FORMAT) $< $@

# Create extended listing file from ELF output file.
%.lss: %.elf
	$(OBJDUMP) -h -S $< > $@

# Link: create ELF output file from object files.
.SECONDARY : $(TRG).elf
.PRECIOUS : $(OBJ)
%.elf: $(OBJ)
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
	$(REMOVE) $(TARGET).obj
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

