# Makefile for MSP430 target
CROSS_COMPILE = /opt/msp430-gcc-linux-3.3.4/bin/msp430-elf-
# Define directories.	
	uSMARTX_INC_DIR = ../../../inc/
	uSMARTX_LIB_DIR = ../lib/
#	uSMARTX_HAL_DIR = ../../hal/
#	uSMARTX_HAL_INC_DIR = ../inc/
	
# Define programs.
    AR = $(CROSS_COMPILE)ar
    COMPILE = $(CROSS_COMPILE)gcc
    ASSEMBLE = $(CROSS_COMPILE)gcc -x assembler-with-cpp
    REMOVE = rm -f
    COPY = cp
    MOVE = mv

# MCU name
	MCU = msp430f2232

# Output format. Can be [srec|ihex].
    FORMAT = ihex

# Target file name (without extension).
	TARGET = test_$(MCU)

# List C source files here.
	SRC = test.c
					
# List Assembler source files here.
	ASRC = 

# Compiler flags.
	CPFLAGS = -c -g -O2 -Wall
	
# Assembler flags.
	ASFLAGS = -Wa,-ahlms=$(<:.s=.lst),--gstabs  

# Linker flags (passed via GCC).
	LDFLAGS = -L$(uSMARTX_LIB_DIR)

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
.PHONY : Lib All
Lib: Clean $(TARGET).a clear_output

All: Clean $(TARGET).elf

# Link: create ELF output file from object files.
.SECONDARY : $(TARGET).elf
.PRECIOUS : $(OBJ)
$(TARGET).elf: $(OBJ)
	$(COMPILE) $(LDFLAGS) $(LIBFLAGS) $(OBJ) -o $(TARGET).elf

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

