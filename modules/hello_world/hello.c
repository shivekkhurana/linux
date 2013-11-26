#include <linux/init.h>
#include <linux/module.h>

static int hello_init(void){
	//ins module into kernel
	printk(KERN_ALERT "Hello World :)\n");
	return 0;
}

static void hello_exit(void){
	//deallocate resources
	printk(KERN_ALERT "Bye\n");
}

module_init(hello_init);
module_exit(hello_exit);