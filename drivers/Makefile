CC := /usr/local/CodeSourcery/Sourcery_G++_Lite/bin/arm-none-eabi-gcc
obj-m := ieb_dipsw.o
KDIR := /Smart4412Linux/Development/Source/Kernel/kernel_4412

all:
	make -C $(KDIR) M=$(shell pwd)
clean:
	rm -rf *.o *.ko
	rm -f $(TEST_TARGET)