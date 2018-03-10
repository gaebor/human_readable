#include <stdlib.h>
#include <string.h>
#include <locale.h>

#include "hr.h"

const char* suffix = "";
char* out_buffer;
char* in_buffer;
size_t out_buffer_size;
size_t input_buffer_size = 1024;

int main(int argc, const char* argv[])
{
    int s, line_length;
    const char* loc = "C";
    float x;
    int padding = 0;
    char white_space_format[20];
    char word_format[20];
    int function_id = 0;

    hr_print_function_t print_function;
    const char* program_name = argv[0];
    char *oldLocale = setlocale(LC_NUMERIC, NULL);
    
    for (++argv; *argv; ++argv)
    {
        if (strcmp(*argv, "-t") == 0 || strcmp(*argv, "--time") == 0)
            function_id = 4;
        else if (strcmp(*argv, "-gi") == 0 || strcmp(*argv, "--gibi") == 0)
            function_id = 2;
        else if (strcmp(*argv, "-g") == 0 || strcmp(*argv, "--giga") == 0)
            function_id  = 0;
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

    snprintf(white_space_format, 20, "%%%zd[ \t\n\r\f\v]", input_buffer_size);
    snprintf(word_format, 20, "%%%zd[^ \t\n\r\f\v]%%n", input_buffer_size);

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

    print_function = hr_get_function(function_id + padding);

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
                print_function(out_buffer, out_buffer_size, x, suffix);
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

