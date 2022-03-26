#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/hash.h>
#include<linux/gcd.h>
#include<linux/jiffies.h>
#include<linux/param.h>

int my_init(void){
	printk("Loading kernel module--\ngolden ratio prime:%lu \njffies:%u\n HZ:%u\n", \ 
	     GOLDEN_RATIO_PRIME, jiffies, HZ);
	return 0;
}

void my_exit(void){
	printk(KERN_INFO "Removing kernel module---\n gcd(3300,24) = %d\n jiffies:%u\n", \
			gcd(3300, 24), jiffies);
}


module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("First Mod");
MODULE_AUTHOR("XHT");



