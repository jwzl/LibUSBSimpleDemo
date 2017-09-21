src =LibUSBSimpleDemo.c

CC :=arm-linux-gnueabihf-gcc
LDFLAGS:=-lusb-1.0 -pthread

TestUsbdemo: $(src)
	$(CC) -o $@  $(LDFLAGS)  $^

.PHONY:
clean:
	@rm -rf TestUsbdemo

