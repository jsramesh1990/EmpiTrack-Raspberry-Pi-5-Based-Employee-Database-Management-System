#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void clear_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

void trim_newline(char *str) {
    if (!str) return;
    size_t len = strlen(str);
    while (len > 0 && (str[len-1] == '\n' || str[len-1] == '\r')) {
        str[--len] = '\0';
    }
}

int read_string(const char *prompt, char *buf, size_t size) {
    if (!prompt || !buf || size == 0) return -1;
    printf("%s", prompt);
    fflush(stdout);
    if (!fgets(buf, (int)size, stdin)) {
        return -1;
    }
    trim_newline(buf);
    return 0;
}

int read_int(const char *prompt, int *out) {
    char buf[64];
    if (read_string(prompt, buf, sizeof(buf)) != 0) return -1;
    if (buf[0] == '\0') return -1;
    char *end = NULL;
    long v = strtol(buf, &end, 10);
    if (end == buf || *end != '\0') return -1;
    *out = (int)v;
    return 0;
}

int read_float(const char *prompt, float *out) {
    char buf[64];
    if (read_string(prompt, buf, sizeof(buf)) != 0) return -1;
    if (buf[0] == '\0') return -1;
    char *end = NULL;
    float v = strtof(buf, &end);
    if (end == buf || *end != '\0') return -1;
    *out = v;
    return 0;
}

void to_lowercase(char *str) {
    if (!str) return;
    for (; *str; ++str) *str = (char)tolower((unsigned char)*str);
}

int validate_email(const char *email) {
    if (!email || !*email) return 0;
    const char *at = strchr(email, '@');
    if (!at || at == email) return 0;
    const char *dot = strchr(at + 1, '.');
    if (!dot || dot == at + 1) return 0;
    if (*(dot + 1) == '\0') return 0;
    // no spaces
    for (const char *p = email; *p; ++p) {
        if (isspace((unsigned char)*p)) return 0;
    }
    return 1;
}

int validate_phone(const char *phone) {
    if (!phone || !*phone) return 0;
    int digits = 0;
    for (const char *p = phone; *p; ++p) {
        unsigned char c = (unsigned char)*p;
        if (isdigit(c)) {
            digits++;
        } else if (c == '+' || c == '-' || c == ' ' || c == '(' || c == ')') {
            continue;
        } else {
            return 0;
        }
    }
    return digits >= 7 && digits <= 15;
}

void print_separator(char c, int len) {
    for (int i = 0; i < len; ++i) putchar(c);
    putchar('\n');
}

void pause_console(void) {
    printf("\nPress Enter to continue...");
    fflush(stdout);
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

int str_contains_ci(const char *haystack, const char *needle) {
    if (!haystack || !needle) return 0;
    if (!*needle) return 1;
    size_t nlen = strlen(needle);
    size_t hlen = strlen(haystack);
    if (nlen > hlen) return 0;
    for (size_t i = 0; i + nlen <= hlen; ++i) {
        size_t j = 0;
        for (; j < nlen; ++j) {
            if (tolower((unsigned char)haystack[i+j]) !=
                tolower((unsigned char)needle[j])) break;
        }
        if (j == nlen) return 1;
    }
    return 0;
}
