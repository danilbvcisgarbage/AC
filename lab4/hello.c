#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/ktime.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/errno.h>

// Параметр модуля
static uint hello_count = 1;
module_param(hello_count, uint, 0444);
MODULE_PARM_DESC(hello_count, "Number of 'Hello, world!' messages to print");

// Структура для списку подій
struct hello_event {
    struct list_head list;
    ktime_t time;
};

// Головний список подій
static LIST_HEAD(hello_list);

// Функція ініціалізації
static int __init hello_init(void) {
    struct hello_event *event;
    int i;

    // Перевірка параметра
    if (hello_count == 0 || (hello_count >= 5 && hello_count <= 10)) {
        pr_warn("Warning: hello_count = %u\n", hello_count);
    } else if (hello_count > 10) {
        pr_err("Error: hello_count = %u exceeds limit\n", hello_count);
        return -EINVAL;
    }

    pr_info("Hello module loaded with hello_count = %u\n", hello_count);

    // Друк повідомлень і збереження подій у список
    for (i = 0; i < hello_count; i++) {
        event = kmalloc(sizeof(*event), GFP_KERNEL);
        if (!event) {
            pr_err("Memory allocation failed\n");
            return -ENOMEM;
        }

        event->time = ktime_get();
        list_add_tail(&event->list, &hello_list);
        pr_info("Hello, world! [%d]\n", i + 1);
    }

    return 0;
}

// Функція завершення
static void __exit hello_exit(void) {
    struct hello_event *event, *tmp;

    pr_info("Hello module unloaded\n");

    // Прохід по списку, друк часу і звільнення пам'яті
    list_for_each_entry_safe(event, tmp, &hello_list, list) {
        pr_info("Time: %lld ns\n", ktime_to_ns(event->time));
        list_del(&event->list);
        kfree(event);
    }
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple hello world module with event tracking");

