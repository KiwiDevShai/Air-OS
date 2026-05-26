#pragma once

void panic_(const char *file, const char *func, int line,
            const char *fmt, ...);

#define PANIC(fmt, ...) \
    panic_(__FILE__, __func__, __LINE__, fmt, ##__VA_ARGS__)