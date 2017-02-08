ifneq ($(KERNELRELEASE), )
    obj-m := myplatfm_drv.o
    obj-m += myplatfm_dev.o

else
    KERNELDIR = /lib/modules/$(shell uname -r)/build
    PWD := $(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean 

endif
