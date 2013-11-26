#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>

#define DEVICE_NAME "TEST USB"

//probe function
static int test_probe(struct usb_interface* interface, const struct usb_device_id* id)
{
	printk(KERN_INFO "%s : (%04X:%04X)) plugged in", DEVICE_NAME, id->idVendor, id->idProduct);
	return 0; //indicates management rights
}

//disconnect
static void test_disconnect(struct usb_interface* interface){
	printk(KERN_INFO "%s Disconnected", DEVICE_NAME);
}


//usb_device_id
static struct usb_device_id test_table = 
{
	//138a:003c (vendor id, device id) . This one is for my laptop's fingerprint reader
	{USB_DEVICE(0x138a, 0x003c)},
	//{USB_DEVICE(a,b)} //can be more than 1
	{}
};

MODULE_DEVICE_TABLE(usb, test_table);

//usb structure
static struct usb_driver test_usb = 
{
	.name = DEVICE_NAME,
	.id_table = test_table, //type : usb_device_id
	.probe = test_probe, //called when device is plugged onto the system however will not be called if another driver is managing the device
	.disconnect = test_disconnect
};

static int __init test_init(void){
	int ret = -1;
	return ret;
}

static void __exit test_exit(void){

}

module_init(test_init);
module_exit(test_exit);