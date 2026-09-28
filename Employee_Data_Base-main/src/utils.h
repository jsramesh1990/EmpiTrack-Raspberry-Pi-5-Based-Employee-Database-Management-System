#ifndef EMPITRACK_UTILS_H
#define EMPITRACK_UTILS_H

#include <stddef.h>

void clear_input_buffer(void);
int read_int(const char *prompt, int *out);
int read_float(const char *prompt, float *out);
int read_string(const char *prompt, char *buf, size_t size);
int validate_email(const char *email);
int validate_phone(const char *phone);
void to_lowercase(char *str);
void trim_newline(char *str);
void print_separator(char c, int len);
void pause_console(void);
int str_contains_ci(const char *haystack, const char *needle);

#endif
