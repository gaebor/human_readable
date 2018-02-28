#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <locale.h>

#ifdef _MSC_VER
# define my_sprintf sprintf_s
#elif defined __GNUC__
# define my_sprintf snprintf
#endif

const char* suffix = "";
char* out_buffer;
char* in_buffer;
size_t out_buffer_size;
size_t input_buffer_size = 1024;

int print_hr(char* result, float num, const float changes[], const char* units[], const char* format)
{
    for (num /= changes[0], ++changes; changes[0] > 0; ++units, ++changes)
    {
        if (fabsf(num) < changes[0])
        {
            return my_sprintf(result, out_buffer_size, format, num, *units, suffix);
        }
        else
            num /= changes[0];
    }
    return my_sprintf(result, out_buffer_size, format, num, *units, suffix);
}

int hr_time(char* result, float num)
{
    static const char* units[] = { " ns",  " us",  " ms",  "sec",  "min",  "  h",  "day",  "yrs" };
    static const float changes[] = { 1e-9f,  1000,  1000,   1000,     60,     60,     24,   365, 0 };

    return print_hr(result, num, changes, units, "%4.3g%s%s");
}

int hr_time_P(char* result, float num)
{
    static const char* units[] = { "ns",  "us",  "ms",  "sec",  "min",  "h",  "day",  "yrs" };
    static const float changes[] = { 1e-9f,1000, 1000,   1000,     60,   60,     24,   365, 0 };

    return print_hr(result, num, changes, units, "%.3g%s%s");
}

int hr_giga(char* result, float num)
{
    static const char* units[] = { " ",  "k",  "M",  "G",  "T",  "P",  "E",  "Z",  "Y" };
    static const float changes[] = { 1, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 0 };

    return print_hr(result, num, changes, units, "%5.4g%s%s");
}

int hr_giga_P(char* result, float num)
{
    static const char* units[] = { "",  "k",  "M",  "G",  "T",  "P",  "E",  "Z",  "Y" };
    static const float changes[] = { 1, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 0 };

    return print_hr(result, num, changes, units, "%.4g%s%s");
}

int hr_gibi(char* result, float num)
{
    static const char* units[] = { "  ",  "ki",  "Mi",  "Gi",  "Ti",  "Pi",  "Ei",  "Zi",  "Yi" };
    static const float changes[] = {  1,  1024,  1024,  1024,  1024,  1024,  1024,  1024,  1024, 0 };

    return print_hr(result, num, changes, units, "%5.4g%s%s");
}

int hr_gibi_P(char* result, float num)
{
    static const char* units[] = { "",  "ki",  "Mi",  "Gi",  "Ti",  "Pi",  "Ei",  "Zi",  "Yi" };
    static const float changes[] = { 1,  1024,  1024,  1024,  1024,  1024,  1024,  1024,  1024, 0 };

    return print_hr(result, num, changes, units, "%.4g%s%s");
}

int main(int argc, const char* argv[])
{
    int s, line_length;
    const char* loc = "C";
    float x;
    int padding = 0;
    char white_space_format[20];
    char word_format[20];

    int(*print_functions[])(char*, float) = { hr_giga, hr_giga_P, hr_gibi, hr_gibi_P, hr_time, hr_time_P};
    int(**print_function)(char*, float) = print_functions;
    const char* program_name = argv[0];
    char *oldLocale = setlocale(LC_NUMERIC, NULL);

    for (++argv; *argv; ++argv)
    {
        if (strcmp(*argv, "-t") == 0 || strcmp(*argv, "--time") == 0)
            print_function = print_functions + 4;
        else if (strcmp(*argv, "-gi") == 0 || strcmp(*argv, "--gibi") == 0)
            print_function = print_functions + 2;
        else if (strcmp(*argv, "-g") == 0 || strcmp(*argv, "--giga") == 0)
            print_function = print_functions;
        else if (strcmp(*argv, "-P") == 0 || strcmp(*argv, "--unpadded") == 0)
            padding = 1;
        else if (strcmp(*argv, "-u") == 0 || strcmp(*argv, "--unbuffer") == 0)
        {
            if (setvbuf(stdout, NULL, _IONBF, 0) != 0)
                fprintf(stderr, "Unable to set unbuffered IO!\n");
        }
        else if (strcmp(*argv, "-s") == 0 || strcmp(*argv, "--suffix") == 0)
        {
            if (*++argv)
                suffix = *argv;
            else
                fprintf(stderr, "Argument \"-s\" or \"--suffix\" requires a value!\n");
        }
        else if (strcmp(*argv, "-h") == 0 || strcmp(*argv, "--help") == 0)
        {
            printf("HUMAN READABLE -- borbely@math.bme.hu --\nA command line tool for nicely printing data\nUSAGE: %s [options]\n", program_name);
            printf("\t-s --suffix <str>\tstr will be appended after every number\n");
            printf("\t-t --time\tevery number will be interpreted as time in seconds\n");
            printf("\t-gi --gibi\tuses binary prefixes, like kibi, mebi, gibi\n");
            printf("\t-g --giga\tuses SI or metric prefixes, like kilo, mega, giga (this is the default)\n");
            printf("\t-u --unbuffer\tunbuffered output\n");
            printf("\t-P --unpadded\tdoes not pad to fixed width\n");
            printf("\t-l --locale\tsets locale, default is \"%s\",\n\t\tuse empty string to inherit locale\n", loc);
            return 0;
        }
        else if (strcmp(*argv, "-l") == 0 || strcmp(*argv, "--locale") == 0)
        {
            if (*++argv)
                loc = *argv;
            else
                fprintf(stderr, "Argument \"-l\" or \"--locale\" requires a value!\n");
        }
        else
        {
            fprintf(stderr, "Unrecognized argument \"%s\" !\n", *argv);
        }

    }

    my_sprintf(white_space_format, 20, "%%%zd[ \t\n\r\f\v]", input_buffer_size);
    my_sprintf(word_format, 20, "%%%zd[^ \t\n\r\f\v]%%n", input_buffer_size);

    in_buffer = malloc(input_buffer_size + 1);
    if (in_buffer == NULL)
    {
        fprintf(stderr, "Unable to allocate %zd Bytes for input buffer!\n", input_buffer_size + 1);
        return 1;
    }

    out_buffer_size = strlen(suffix) + 20;
    out_buffer = malloc(out_buffer_size);
    if (out_buffer == NULL)
    {
        fprintf(stderr, "Unable to allocate %zd Bytes for output buffer!\n", out_buffer_size);
        return 1;
    }

    print_function += padding;

    if (setlocale(LC_NUMERIC, loc) == NULL)
        fprintf(stderr, "Unable to set LC_NUMERIC \"%s\"!\n", loc);

    while (!feof(stdin))
    {
        if (scanf(white_space_format, in_buffer))
            printf("%s", in_buffer);
        if (!feof(stdin) && scanf(word_format, in_buffer, &line_length) == 1)
        {
            if (sscanf(in_buffer, "%f%n", &x, &s) == 1 && s == line_length)
            {
                (*print_function)(out_buffer, x);
                printf("%s", out_buffer);
            }
            else
                printf("%s", in_buffer);
        }
    }

    setlocale(LC_NUMERIC, oldLocale);
    free(out_buffer);
    free(in_buffer);

    return 0;
}

