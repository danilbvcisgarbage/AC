#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include "hello1.h"

// Параметр модуля
static uint hello_count = 1;
module_param(hello_count, uint, 0444);
MODULE_PARM_DESC(hello_count, "Number of 'Hello, world!' messages to print");

// Функція ініціалізації
static int __init hello2_init(void) {
    if (hello_count == 0 || (hello_count >= 5 && hello_count <= 10)) {
        pr_warn("hello2: Warning: hello_count = %u\n", hello_count);
    } else if (hello_count > 10) {
        pr_err("hello2: Error: hello_count = %u exceeds limit\n", hello_count);
        return -EINVAL;
    }

    pr_info("hello2: Module loaded with hello_count = %u\n", hello_count);
    print_hello(hello_count);
    return 0;
}

// Функція завершення
static void __exit hello2_exit(void) {
    pr_info("hello2: Module unloaded\n");
}

module_init(hello2_init);
module_exit(hello2_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Hello2 module using print_hello()");

