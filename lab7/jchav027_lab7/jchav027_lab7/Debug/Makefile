################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := cmd.exe
RM := rm -rf

USER_OBJS :=

LIBS := 
PROJ := 

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
PREPROCESSING_SRCS := 
OBJS := 
OBJS_AS_ARGS := 
C_DEPS := 
C_DEPS_AS_ARGS := 
EXECUTABLES := 
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
AVR_APP_PATH :=$$$AVR_APP_PATH$$$
QUOTE := "
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=
LIB_DEP:=
LINKER_SCRIPT_DEP:=

# Every subdirectory with source files must be described here
SUBDIRS := 


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../croutine.c \
../heap_1.c \
../jchav027_lab7_part1_l.c \
../jchav027_lab7_part2.c \
../jchav027_lab7_part3.c \
../list.c \
../jchav027_lab7_part1_f.c \
../port.c \
../queue.c \
../tasks.c \
../timers.c


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
croutine.o \
heap_1.o \
jchav027_lab7_part1_l.o \
jchav027_lab7_part2.o \
jchav027_lab7_part3.o \
list.o \
jchav027_lab7_part1_f.o \
port.o \
queue.o \
tasks.o \
timers.o

OBJS_AS_ARGS +=  \
croutine.o \
heap_1.o \
jchav027_lab7_part1_l.o \
jchav027_lab7_part2.o \
jchav027_lab7_part3.o \
list.o \
jchav027_lab7_part1_f.o \
port.o \
queue.o \
tasks.o \
timers.o

C_DEPS +=  \
croutine.d \
heap_1.d \
jchav027_lab7_part1_l.d \
jchav027_lab7_part2.d \
jchav027_lab7_part3.d \
list.d \
jchav027_lab7_part1_f.d \
port.d \
queue.d \
tasks.d \
timers.d

C_DEPS_AS_ARGS +=  \
croutine.d \
heap_1.d \
jchav027_lab7_part1_l.d \
jchav027_lab7_part2.d \
jchav027_lab7_part3.d \
list.d \
jchav027_lab7_part1_f.d \
port.d \
queue.d \
tasks.d \
timers.d

OUTPUT_FILE_PATH +=jchav027_lab7.elf

OUTPUT_FILE_PATH_AS_ARGS +=jchav027_lab7.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+= 

LINKER_SCRIPT_DEP+= 


# AVR32/GNU C Compiler























./%.o: .././%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 4.9.2
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -x c -funsigned-char -funsigned-bitfields -DDEBUG  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.0.90\include" -I"C:\Users\cathy\Documents\UCR\fall2015\CS122A\FreeRTOS_Lab\FreeRTOS_Lab\FreeRTOS\Source" -I"../../../../FreeRTOS_Lab/FreeRTOS_Lab/FreeRTOS/Source/include" -I"../../../../FreeRTOS_Lab/FreeRTOS_Lab/FreeRTOS/Source/portable/GCC/ATMega323" -I"C:\Users\cathy\Documents\UCR\fall2015" -I"C:\Users\cathy\Documents\UCR\fall2015\CS122A"  -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -mrelax -g2 -Wall -mmcu=atmega1284 -B "C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.0.90\gcc\dev\atmega1284" -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	



# AVR32/GNU Preprocessing Assembler



# AVR32/GNU Assembler




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP) $(LIB_DEP) $(LINKER_SCRIPT_DEP)
	@echo Building target: $@
	@echo Invoking: AVR/GNU Linker : 4.9.2
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -Wl,-Map="jchav027_lab7.map" -Wl,--start-group -Wl,-lm  -Wl,--end-group -Wl,--gc-sections -mrelax -mmcu=atmega1284 -B "C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\ATmega_DFP\1.0.90\gcc\dev\atmega1284"  
	@echo Finished building target: $@
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures  "jchav027_lab7.elf" "jchav027_lab7.hex"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "jchav027_lab7.elf" "jchav027_lab7.eep" || exit 0
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objdump.exe" -h -S "jchav027_lab7.elf" > "jchav027_lab7.lss"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures "jchav027_lab7.elf" "jchav027_lab7.srec"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-size.exe" "jchav027_lab7.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "jchav027_lab7.elf" "jchav027_lab7.a" "jchav027_lab7.hex" "jchav027_lab7.lss" "jchav027_lab7.eep" "jchav027_lab7.map" "jchav027_lab7.srec" "jchav027_lab7.usersignatures"
	