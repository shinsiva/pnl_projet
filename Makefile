.PHONY: all 
#.SECONDARY:

ifneq ($(KERNELRELEASE),)

obj-m := our_modc.o

else
  KERNELDIR ?= ./linux-4.2.3/
  PWD := $(shell pwd)

all :
	$(info obj-m : $(obj-m))
	make -C $(KERNELDIR) M=$(PWD) modules

clean:
	make -C $(KERNELDIR) M=$(PWD) clean

endif
