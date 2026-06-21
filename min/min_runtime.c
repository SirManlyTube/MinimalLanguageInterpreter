#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "file_reader.h"
#include "min_interpreter.h"

#include "min_interpreter_v2.h"

int main(int argc, char* argv[])
{
	int versionRequested = 1;
	char* inputFilename = "";
	for (int i = 1; i < argc; ++i)
	{
		if (strcmp(argv[i], "-file") == 0)
		{
			if (i + 2 > argc)
			{
				fprintf(stderr, "Could not find value for argument '%s'.\n", argv[i]);
				return -1;
			}
			const char* value = argv[++i];
			inputFilename = value;
		}
		else if (strcmp(argv[i], "-version") == 0)
		{
			if (i + 2 > argc)
			{
				fprintf(stderr, "Could not find value for argument '%s'\n", argv[i]);
				return -1;
			}
			int value = (int)strtol(argv[++i], NULL, 10);
			versionRequested = value;
		}
		else
		{
			fprintf(stderr, "Unknown argument: %s\n", argv[i]);
		}
	}
	if (strlen(inputFilename) == 0)
	{
		fprintf(stderr, "Please input a valid filename by passing the argument '-file \"example_file.min\"' when running the interpreter in the command-line.");
		return -1;
	}
	char* code = ReadFromFile(inputFilename);
	switch (versionRequested)
	{
		case 1:
			RunInterpreter(code);
			break;
		case 2:
			RunInterpreterV2(code);
			break;
		default:
			fprintf(stderr, "Invalid version requested: %d. Valid versions: [1, 2]\n", versionRequested);
			free(code);
			return -1;
	}
	free(code);
	return 0;
}
