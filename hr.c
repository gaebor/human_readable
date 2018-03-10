#include "hr.h"

#include <math.h>

static int print_hr(char* result, int out_buffer_size, float num, const float changes[], const char* units[], const char* format, const char* suffix)
{
    for (num /= changes[0], ++changes; changes[0] > 0; ++units, ++changes)
    {
        if (fabsf(num) < changes[0])
        {
            return snprintf(result, out_buffer_size, format, num, *units, suffix);
        }
        else
            num /= changes[0];
    }
    return snprintf(result, out_buffer_size, format, num, *units, suffix);
}

int hr_time(char* result, int out_buffer_size, float num, const char* suffix)
{
    static const char* units[] = { " ns",  " us",  " ms",  "sec",  "min",  "  h",  "day",  "yrs" };
    static const float changes[] = { 1e-9f,  1000,  1000,   1000,     60,     60,     24,   365, 0 };

    return print_hr(result, out_buffer_size, num, changes, units, "%4.3g%s%s", suffix);
}

int hr_time_P(char* result, int out_buffer_size, float num, const char* suffix)
{
    static const char* units[] = { "ns",  "us",  "ms",  "sec",  "min",  "h",  "day",  "yrs" };
    static const float changes[] = { 1e-9f,1000, 1000,   1000,     60,   60,     24,   365, 0 };

    return print_hr(result, out_buffer_size, num, changes, units, "%.3g%s%s", suffix);
}

int hr_giga(char* result, int out_buffer_size, float num, const char* suffix)
{
    static const char* units[] = { " ",  "k",  "M",  "G",  "T",  "P",  "E",  "Z",  "Y" };
    static const float changes[] = { 1, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 0 };

    return print_hr(result, out_buffer_size, num, changes, units, "%5.4g%s%s", suffix);
}

int hr_giga_P(char* result, int out_buffer_size, float num, const char* suffix)
{
    static const char* units[] = { "",  "k",  "M",  "G",  "T",  "P",  "E",  "Z",  "Y" };
    static const float changes[] = { 1, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 0 };

    return print_hr(result, out_buffer_size, num, changes, units, "%.4g%s%s", suffix);
}

int hr_gibi(char* result, int out_buffer_size, float num, const char* suffix)
{
    static const char* units[] = { "  ",  "ki",  "Mi",  "Gi",  "Ti",  "Pi",  "Ei",  "Zi",  "Yi" };
    static const float changes[] = {  1,  1024,  1024,  1024,  1024,  1024,  1024,  1024,  1024, 0 };

    return print_hr(result, out_buffer_size, num, changes, units, "%5.4g%s%s", suffix);
}

int hr_gibi_P(char* result, int out_buffer_size, float num, const char* suffix)
{
    static const char* units[] = { "",  "ki",  "Mi",  "Gi",  "Ti",  "Pi",  "Ei",  "Zi",  "Yi" };
    static const float changes[] = { 1,  1024,  1024,  1024,  1024,  1024,  1024,  1024,  1024, 0 };

    return print_hr(result, out_buffer_size, num, changes, units, "%.4g%s%s", suffix);
}

hr_print_function_t hr_get_function(int i)
{
    static int(*hr_print_functions[])(char*, int, float, const char*) = { hr_giga, hr_giga_P, hr_gibi, hr_gibi_P, hr_time, hr_time_P };
    return hr_print_functions[i];
}
