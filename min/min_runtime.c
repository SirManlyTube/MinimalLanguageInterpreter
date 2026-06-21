#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "file_reader.h"
#include "min_interpreter.h"

#include "min_interpreter_v2.h"

int main(int argc, char* argv[])
{
	char* inputFilename = "";
	for (int i = 1; i < argc; ++i)
	{
		if (strncmp(argv[i], "-file=", 6) == 0)
		{
			char* value = strchr(argv[i], '=') + 1;
			printf("Found argument -file with value: '%s'\n", value);
			inputFilename = value;
		}
		else
		{
			printf("Unknown argument: %s\n", argv[i]);
		}
	}
	if (strlen(inputFilename) == 0)
	{
		fprintf(stderr, "Please input a valid filename by passing the argument '-file=\"example_file.min\"' when running the interpreter in the command-line.");
		exit(-1);
	}
	char* code = ReadFromFile(inputFilename);
	RunInterpreterV2(code);
	free(code);
	return 0;
}
