#include <stdio.h>
#include <string.h>
#include <math.h>

#ifdef _MSC_VER
# define my_sprintf sprintf_s
#elif defined __GNUC__
# define my_sprintf snprintf
#endif

int print_humanreadable_time(char* result, int bufferSize, float num, const char* suffix /*= ""*/)
{
	if (num < 1e-6f)
		return my_sprintf(result, bufferSize, "%4.3g%s%s", num*1e9, " ns", suffix);
	else if (num < 1e-3f)
		return my_sprintf(result, bufferSize, "%4.3g%s%s", num*1e6, " us", suffix);
	else if (num < 1)
		return my_sprintf(result, bufferSize, "%4.3g%s%s", num*1e3, " ms", suffix);
	else if (num < 60)
		return my_sprintf(result, bufferSize, "%4.3g%s%s", num, "sec", suffix);
	else if (num < 3600)
		return my_sprintf(result, bufferSize, "%4.3g%s%s", num / 60, "min", suffix);
	else if (num < 3600 * 24)
		return my_sprintf(result, bufferSize, "%4.3g%s%s", num / 3600, "  h", suffix);
	else if (num < 3600 * 24 * 365)
		return my_sprintf(result, bufferSize, "%4.3g%s%s", num / (3600 * 24), "day", suffix);
	else 
		return my_sprintf(result, bufferSize, "%.3g%s%s", num / (3600 * 24 * 365), "yrs", suffix);
}

int print_humanreadable_giga(char* result, int bufferSize, float num, const char* suffix /*= ""*/)
{
	static const char* units[] = { " ", "k", "M", "G", "T", "P", "E", "Z", NULL };
	const char** unit;

	for (unit = units; *unit != NULL; ++unit)
	{
		if (fabsf(num) < 1000.0f)
		{
			return my_sprintf(result, bufferSize, "%4.3g%s%s", num, *unit, suffix);
		}
		else
			num /= 1000;
	}
	return my_sprintf(result, bufferSize, "%.3g%s%s", num, "Y", suffix);
}

int print_humanreadable_gibi(char* result, int bufferSize, float num, const char* suffix /*= ""*/)
{
	static const char* units[] = { "  ", "ki", "Mi", "Gi", "Ti", "Pi", "Ei", "Zi", NULL };
	const char** unit;

	for (unit = units; *unit != NULL; ++unit)
	{
		if (fabsf(num) < 1024.0f)
		{
			return my_sprintf(result, bufferSize, "%5.4g%s%s", num, *unit, suffix);
		}
		else
			num /= 1024;
	}
	return my_sprintf(result, bufferSize, "%g%s%s", num, "Yi", suffix);
}

int main(int argc, char* argv[])
{
	char line[1001], number[20];
	float x;
	char suffix[1001];
	int (*print_function)(char*, int , float , const char* ) = print_humanreadable_giga;
	const char* program_name = argv[0];

	strcpy(suffix, "");
	strcpy(line, "");

	for (++argv; *argv; ++argv)
	{
		if (strcmp(*argv, "-t") == 0 || strcmp(*argv, "--time") == 0)
			print_function = print_humanreadable_time;
		else if (strcmp(*argv, "-gi") == 0 || strcmp(*argv, "--gibi") == 0)
			print_function = print_humanreadable_gibi;
		else if (strcmp(*argv, "-g") == 0 || strcmp(*argv, "--giga") == 0)
			print_function = print_humanreadable_giga;
		else if (strcmp(*argv, "-s") == 0 || strcmp(*argv, "--suffix") == 0)
		{
			if (*++argv)
				strcpy(suffix, *argv);
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
			return 0;
		}

	}

	while (!feof(stdin))
	{
		if(scanf("%1000[ \t\n\r\f]", line))
			printf("%s", line);
		if (!feof(stdin) && scanf("%1000[^ \t\n\r\f]", line) == 1)
		{
			if (sscanf(line, "%f", &x) == 1)
			{
				print_function(number, 20, x, suffix);
				printf("%s", number);
			}
			else
				printf("%s", line);
		}
	}

	return 0;
}

