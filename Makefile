obj-m += whisper.o
whisper-objs := whisper_linux.o  asm_function_x64.o whisper_handler.o

all:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	rm -rf *.o *.cmd