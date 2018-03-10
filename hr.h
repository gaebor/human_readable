#ifndef INCLUDE_HR_H
#define INCLUDE_HR_H

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined _MSC_VER && _MSC_VER < 1900
#   define snprintf sprintf_s
#endif

int hr_time(char* result, int out_buffer_size, float num, const char* suffix);
int hr_time_P(char* result, int out_buffer_size, float num, const char* suffix);

int hr_giga(char* result, int out_buffer_size, float num, const char* suffix);
int hr_giga_P(char* result, int out_buffer_size, float num, const char* suffix);

int hr_gibi(char* result, int out_buffer_size, float num, const char* suffix);
int hr_gibi_P(char* result, int out_buffer_size, float num, const char* suffix);

typedef int(*hr_print_function_t)(char* result, int out_buffer_size, float num, const char* suffix);
hr_print_function_t hr_get_function(int);

#ifdef __cplusplus
}
#endif

#endif
