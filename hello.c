
/*
 * Copyright (c) 2017, GlobalLogic Ukraine LLC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    This product includes software developed by the GlobalLogic.
 * 4. Neither the name of the GlobalLogic nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY GLOBALLOGIC UKRAINE LLC ``AS IS`` AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL GLOBALLOGIC UKRAINE LLC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

// SPDX-License-Identifier: GPL-2.0

#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/list.h>
#include <linux/ktime.h>
#include <linux/slab.h>

MODULE_AUTHOR("Serhii Popovych <serhii.popovych@globallogic.com>");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training");
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

static int __init hello_init(void)
{
	if (myParam > 10) {
		pr_err("Error: myParam cannot be greater than 10\n");
		return -EINVAL;
	}

	if (myParam == 0 || (myParam <= 10 && myParam >= 5)) {
		pr_emerg("Warning: %u\n", myParam);
	}

	counter = 0;
	while (counter != myParam) {
		struct myStruct *ptr = kmalloc(sizeof(*ptr), GFP_KERNEL);

		ptr->myTime = ktime_get();

		list_add_tail(&ptr->list, &myList);
		pr_emerg("Hello world!\n");
		counter += 1;
	}

	return 0;
}

static void __exit hello_exit(void)
{
	struct myStruct *ptr, *next;

	list_for_each_entry_safe(ptr, next, &myList, list) {
		pr_emerg("Time in nanoseconds: %lld\n", ktime_to_ns(ptr->myTime));
		list_del(&ptr->list);
		kfree(ptr);
	}
}

module_init(hello_init);
module_exit(hello_exit);
