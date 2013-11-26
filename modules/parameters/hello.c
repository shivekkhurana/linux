#include <linux/init.h>
#include <linux/module.h>

#include <linux/moduleparam.h>


int param_var = 0;

//register
//module_param(name_var, type, permissions)
module_param(param_var, int, S_IRUSR | S_IWUSR);

//proto
void display(void);

void display(){
	printk(KERN_ALERT "Test param : %d", param_var);
}

static int hello_init(void){
	printk(KERN_ALERT "Hello, this is first param passing");
	display();
	return 0;
}

static void hello_exit(void){
	printk(KERN_ALERT "Bye");
}

module_init(hello_init);
module_exit(hello_exit);