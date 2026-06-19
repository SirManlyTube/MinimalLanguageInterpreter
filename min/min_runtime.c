#include <stdio.h>

#include "min_interpreter.h"

char* ReadFromFile(const char* filename)
{
	FILE* file = fopen(filename, "rb");

	if (file == NULL)
	{
		fprintf(stderr, "Failed to open file: '%s' does not exist.", filename);
		exit(-1);
	}

	fseek(file, 0, SEEK_END);
	size_t length = (size_t)(ftell(file));
	fseek(file, 0, SEEK_SET);

	char* content = malloc(length + 1);

	if (content == NULL)
	{
		fprintf(stderr, "Failed to open file: Could not allocate memory for buffer.\n");
		exit(-1);
	}

	fread(content, 1, length, file);

	content[length] = 0;

	fclose(file);

	return content;
}

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
	RunInterpreter(code);
	free(code);
	return 0;
}
