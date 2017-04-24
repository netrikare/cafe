obj-m := cafe.o 
cafe-y := src/cafe_mod.o src/cafe_icmp.o
ccflags-y := -Wno-incompatible-pointer-types
 
KDIR ?= /lib/modules/$(shell uname -r)/build
PWD:= $(shell pwd)
 
all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	rm cafe_client

insmod: all
	sudo insmod cafe.ko

rmmod:
	sudo rmmod cafe

client:
	gcc -o cafe_client src/cafe_client.c