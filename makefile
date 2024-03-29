CROSS-COMPILER = arm-none-eabi-
CFGR = gcc -std=c11 -Wall -mcpu=cortex-m7 -mthumb -nostartfiles
FILES = main.c startup.c vector_table.s malloc.c stm32f7_GPIO.c stm32f7_SYSCLK.c stm32f7_USART.c
all: main.bin

main.bin: $(FILES)
	$(CROSS-COMPILER)$(CFGR) -T stm32f7.ld $(FILES) -o main.elf
	$(CROSS-COMPILER)objcopy -O binary main.elf main.bin

flash:
	st-flash --reset write main.bin 0x08000000

clean:
	rm -f *.o *.elf *.bin
