ifneq ($(KERNELRELEASE), )
    obj-m := mybus.o
    obj-m += mydevice.o
    obj-m += mydriver.o

else
    KERNELDIR = /lib/modules/$(shell uname -r)/build
    PWD := $(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean 

endif
