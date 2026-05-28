#include "panic.h"
#include "printf/printf.h"
#include "common.h"
#include "string.h"
#include <stdarg.h>

__attribute__((noreturn))
void panic_(const char *file, const char *func, int line, const char *fmt, ...) {
    va_list pargs; // printf_() variable arguments
    char buf[2048] = "";

    va_start(pargs, fmt);
    
    printf_("PANIC: ");
    vsnprintf_(buf, sizeof(buf), fmt, pargs);

    int len = strlen(buf);
    printf_("%s", (const char*)buf); 
    if (len == 0 || buf[len - 1] != '\n') {
        printf_("\n");
    }

    printf_("(%s:%s:%i)", file, func, line);

    va_end(pargs);
    hcf();
    __builtin_unreachable();
}