obj-m += hello.o

KDIR = /usr/src/linux-headers-3.11.0-12-generic

all:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

clean:
	rm -rf *.o *.ko *.mod.* *.symvers *.order .*.cmd .tmp_versions