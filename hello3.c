
#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/list.h>
#include <linux/ktime.h>
#include <linux/slab.h>

MODULE_AUTHOR("nazar");
MODULE_DESCRIPTION("Lab5 Module hello.c");
MODULE_LICENSE("Dual BSD/GPL");

static uint myParam = 1;

// 0444 = S_IRUGO - флаг дозволу на читання параметра
module_param(myParam, uint, 0444);
MODULE_PARM_DESC(myParam, "My description");

struct myStruct {
	struct list_head list;
	ktime_t myTime;
};

// статична зміна голови списку
static LIST_HEAD(myList);
int counter;

static void freeMemory(int message) {
	struct myStruct *ptr, *next; 
	list_for_each_entry_safe(ptr, next, &myList, list) {
		if (message == 1) {
			pr_emerg("Time in nanoseconds: %lld\n", ktime_to_ns(ptr->myTime));
		}
		list_del(&ptr->list);
		kfree(ptr);
	}
}

static int __init hello_init(void)
{
	BUG_ON(myParam > 10);

	if (myParam == 0 || (myParam <= 10 && myParam >= 5)) {
		pr_emerg("Warning: %u\n", myParam);
	}

	counter = 0;
	while (counter < myParam) {
		struct myStruct *ptr = kmalloc(sizeof(*ptr), GFP_KERNEL);
		// Примусово встановлення ptr на 0 для 9-го елементу
		if (counter == 9) {
			ptr = NULL;		
		}
		if (!ptr) {
			pr_err("kmalloc() returned 0\n");
			freeMemory(0);
			BUG();
		}
		ptr->myTime = ktime_get();
		list_add_tail(&ptr->list, &myList);
		pr_emerg("Hello world!\n");
		counter += 1;
	}

	return 0;
}

static void __exit hello_exit(void)
{
	freeMemory(1);
}

module_init(hello_init);
module_exit(hello_exit);
