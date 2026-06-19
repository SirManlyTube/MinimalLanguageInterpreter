#include "file_reader.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

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
