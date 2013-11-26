#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h> //file_operations structure to CRUD Device
#include <linux/cdev.h> //make cdev (char device) available (register device to kernel)
#include <linux/semaphore.h> // used to access semaphores; sync behaviours
#include <asm/uaccess.h> //copy_to_user, copy_from_user

//create structure for our fake device

struct fake_device{
	char data[100];
	struct semaphore sem; //semaphore is used to prevent corruption due to multiple processors acting on the same device
} virtual_device;

struct cdev *mycdev;
int major_number;
int return_value; //hold return value of functions because the kernel stack is small
dev_t dev_num; //hold major number provided by kernel

#define DEVICE_NAME "char_device"

int device_open(struct inode* inode, struct file* filp){
	//inode is reference to physical file on disk and contains info about it
	//file is an abstract open file

	//only allow one process to open this device by using semaphore as a mutual exclusive lock - mutex
	if (down_interruptible(&virtual_device.sem) != 0)
	{
		printk(KERN_ALERT "%s:Could not lock device during open", DEVICE_NAME);
		return -1;
	}

	printk(KERN_INFO "%s:device opened", DEVICE_NAME);
	return 0;
}

ssize_t device_read(struct file* filp, char* bufStoreData, size_t bufCount, loff_t* curOffset){
	//take data from kernel space to user space
	//copy_to_user(destination, source, size_to_transfer)
	printk(KERN_INFO "%s : Reading from device", DEVICE_NAME);
	return_value = copy_to_user(bufStoreData, virtual_device.data, bufCount);
	return return_value;
}

ssize_t device_write(struct file* filp, const char* bufStoreData, size_t bufCount, loff_t* curOffset){
	//take data from kernel space to user space
	//copy_to_user(destination, source, size_to_transfer)
	printk(KERN_INFO "%s : Writing to device", DEVICE_NAME);
	return_value = copy_from_user(virtual_device.data, bufStoreData, bufCount);
	return return_value;//signifies total amount of bytes actually written
}

int device_close(struct inode* inode, struct file* filp){
	//after invoking this method, other processes can take control of the device
	up(&virtual_device.sem);
	printk(KERN_INFO "%s : device closed", DEVICE_NAME);
	return 0;
}

struct file_operations fops = {
	.owner = THIS_MODULE, 
	.open = device_open,
	.release = device_close,
	.write = device_write, 
	.read = device_read
};

static int driver_entry(void){
	//here we register the device to the kernel.
	//step 1 : Allocate device, get major, minor number
	return_value = alloc_chrdev_region(&dev_num,0,1,DEVICE_NAME); 
	if (return_value < 0)
	{
		printk(KERN_ALERT "Failed to allocate major number");
		return return_value; //propogate error
	}
	major_number = MAJOR(dev_num);
	printk(KERN_INFO "major number is : %d", major_number);
	printk(KERN_INFO "\tuse \"mknod /dev/%s c %d 0\" for device file", DEVICE_NAME, major_number);
	
	//step 2 : Allocate device structure
	mycdev = cdev_alloc();
	mycdev->ops = &fops; //struct file_operations
	mycdev->owner = THIS_MODULE;

	//now we need to add cdev to the kernel
	return_value = cdev_add(mycdev, dev_num, 1);
	if (return_value < 0)
	{
		printk(KERN_ALERT "Failed to add cdev to kernel");
		return return_value;
	}

	//create a semaphore
	sema_init(&virtual_device.sem, 1);
	return 0;
}

static void driver_exit(void){
	//undo stuff in reverse order

	cdev_del(mycdev);
	unregister_chrdev_region(dev_num, 1);
	printk(KERN_INFO "Unloaded module");
}

module_init(driver_entry);
module_exit(driver_exit);