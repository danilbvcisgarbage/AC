#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/ktime.h>
#include "hello1.h"

// Головний список подій
static LIST_HEAD(hello_list);

// Функція друку і засічення часу
void print_hello(int count) {
    struct hello_event *event;
    int i;

    for (i = 0; i < count; i++) {
        event = kmalloc(sizeof(*event), GFP_KERNEL);
        if (!event) {
            pr_err("hello1: Memory allocation failed\n");
            return;
        }

        // Засікти час до друку
        event->start_time = ktime_get();
        pr_info("Hello, world! [%d]\n", i + 1);
        // Засікти час після друку
        event->end_time = ktime_get();

        list_add_tail(&event->list, &hello_list);
    }
}
EXPORT_SYMBOL(print_hello);

// Функція вивантаження модуля
static void __exit hello1_exit(void) {
    struct hello_event *event, *tmp;

    pr_info("hello1: Module unloaded\n");

    // Друк часу подій та очищення списку
    list_for_each_entry_safe(event, tmp, &hello_list, list) {
        pr_info("Event: start = %lld ns, end = %lld ns, duration = %lld ns\n",
                ktime_to_ns(event->start_time),
                ktime_to_ns(event->end_time),
                ktime_to_ns(ktime_sub(event->end_time, event->start_time)));
        list_del(&event->list);
        kfree(event);
    }
}

module_exit(hello1_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hlushchenko Roman");
MODULE_DESCRIPTION("Hello1 module exporting print_hello()");

