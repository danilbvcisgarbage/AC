#ifndef HELLO1_H
#define HELLO1_H

#include <linux/ktime.h>

// Структура для списку подій
struct hello_event {
    struct list_head list;
    ktime_t start_time;
    ktime_t end_time;
};

// Прототип функції print_hello
void print_hello(int count);

#endif // HELLO1_H

