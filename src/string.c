#include "string.h"


void* memset(void* dest, int ch, size_t count) {
    unsigned char* dest_ptr = dest;
    for (size_t i = 0; i < count; i++) {
        dest_ptr[i] = (unsigned char)ch;
    }
    return dest;
}

void* memcpy(void* restrict dest, const void* restrict src, size_t count) {
    unsigned char* dest_ptr = dest;
    const unsigned char* src_ptr = src;
    for (size_t i = 0; i < count; i++) {
        dest_ptr[i] = src_ptr[i];
    }
    return dest;
}

void* memmove(void* dest, const void* src, size_t count) {
    unsigned char* dest_ptr = dest;
    const unsigned char* src_ptr = src;
    
    if (dest_ptr < src_ptr) {
        memcpy(dest, src, count);
    } else {
        for (size_t i = count; i != 0; i--) {
            dest_ptr[i - 1] = src_ptr[i - 1];
        }
    }
    return dest;
}

int memcmp(const void* lhs, const void* rhs, size_t count) {
    const unsigned char* lhs_ptr = lhs;
    const unsigned char* rhs_ptr = rhs;
    for (size_t i = 0; i < count; i++) {
        if (lhs_ptr[i] != rhs_ptr[i]) {
            return lhs_ptr[i] - rhs_ptr[i];
        }
    }
    return 0;
}

size_t strlen(const char* str) {
    if (!str) return 0;
    size_t i = 0;
    while (str[i++]);
    return i-1;
}

int strcmp(const char* lhs, const char* rhs) {
    size_t i = 0;
    while (lhs[i] == rhs[i] && lhs[i] != 0) {
        i++;
    }
    return (unsigned char)lhs[i] - (unsigned char)rhs[i];
}

int strncmp(const char* lhs, const char* rhs, size_t count) {
    size_t i = 0;
    while (i < count && lhs[i] == rhs[i] && lhs[i] != 0) {
        i++;
    }
    if (i == count) {
        return 0;
    }
    return (unsigned char)lhs[i] - (unsigned char)rhs[i];
}

char* strcpy(char* restrict dest, const char* restrict src) {
    size_t i = 0;
    while ((dest[i] = src[i]) != 0) {
        i++;
    }
    return dest;
}

char* strncpy(char* restrict dest, const char* restrict src, size_t count) {
    size_t i = 0;
    while (i < count && src[i] != 0) {
        dest[i] = src[i];
        i++;
    }
    while (i < count) {
        dest[i] = 0;
        i++;
    }
    return dest;
}

char* strcat(char* restrict dest, const char* restrict src) {
    size_t dest_len = strlen(dest);
    size_t i = 0;
    while ((dest[dest_len + i] = src[i]) != 0) {
        i++;
    }
    return dest;
}