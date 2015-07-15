# Makefile for MSP430 target
CROSS_DIR = /opt/msp430-gcc-lts-4_6
CROSS_COMPILE = $(CROSS_DIR)/bin/msp430-
# Define directories.	
	uSMARTX_INC_DIR = ../../../inc
	uSMARTX_LIB_DIR = ../lib
#	uSMARTX_HAL_DIR = ../../hal/
#	uSMARTX_HAL_INC_DIR = ../inc/
	
# Define programs.
    AR = $(CROSS_COMPILE)ar
    COMPILE = $(CROSS_COMPILE)gcc
    ASSEMBLE = $(CROSS_COMPILE)gcc -x assembler-with-cpp
    OBJCOPY = $(CROSS_COMPILE)objcopy
    SIZE = $(CROSS_COMPILE)size
    REMOVE = rm -f
    COPY = cp
    MOVE = mv

# MCU name
	MCU = msp430g2553

# Output format. Can be [srec|ihex].
    FORMAT = ihex

# Target file name (without extension).
	TARGET = test_$(MCU)

# List C source files here.
	SRC = test_$(MCU).c
					
# List Assembler source files here.
	ASRC = 

# Compiler flags.
	CPFLAGS = -g -O2 -Wall -I$(CROSS_DIR)/include -I$(uSMARTX_INC_DIR)
	
# Assembler flags.
	ASFLAGS = -Wa,-ahlms=$(<:.s=.lst),--gstabs  

# Linker flags (passed via GCC).
	LDFLAGS = -L$(uSMARTX_LIB_DIR) -L$(CROSS_DIR)/include

# Additional library flags (-lm = math library).
	LIBFLAGS = -lusmartx_$(MCU)

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
  
# Default target.
.PHONY : Lib All Size
#Lib: Clean $(TARGET).a clear_output

All: Clean $(TARGET).elf Size

Size: $(TARGET).elf
	@echo ELF file size---------------------------------------
	@$(SIZE) $(TARGET).elf
	@echo HEX file size---------------------------------------
	@$(SIZE) --target=$(FORMAT) $(TARGET).hex

# Link: create ELF output file from object files.
.SECONDARY : $(TARGET).elf
.PRECIOUS : $(OBJ)
$(TARGET).elf: $(OBJ)
	$(COMPILE) $(LDFLAGS) $(OBJ) $(LIBFLAGS) -o $(TARGET).elf
	$(OBJCOPY) -O $(FORMAT) $(TARGET).elf $(TARGET).hex

# Compile: create object files from C source files.
%.o : %.c
	$(COMPILE) -c $(CPFLAGS) -I$(uSMARTX_INC_DIR)  $< -o $@ 

# Assemble: create object files from assembler files.
%.o : %.s
	$(ASSEMBLE) -c $(ASFLAGS) $< -o $@

# Target: clean project.
.SILENT : Clean
.PHONY : Clean
Clean :
	$(REMOVE) $(TARGET).a43
	$(REMOVE) $(TARGET).hex
	$(REMOVE) $(TARGET).o
	$(REMOVE) $(TARGET).elf
	$(REMOVE) $(TARGET).map
	$(REMOVE) $(OBJ)
	$(REMOVE) $(LST)
	$(REMOVE) $(SRC:.c=.s)
#	$(REMOVE) $(TARGET).a
	
# Target clear output files
.SILENT : clear_output
.PHONY: clear_output
clear_output :
#	$(RM)  $(uSMARTX_SRC_DIR)*.o
	$(RM)  *.o
		
# Include the dependency files.
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)


# List assembly only source file dependencies here:

