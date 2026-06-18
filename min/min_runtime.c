#include <stdio.h>

#include "min_interpreter.h"

char* ReadFromFile(const char* filename)
{
	FILE* file = fopen(filename, "rb");

	assert(file != NULL);

	fseek(file, 0, SEEK_END);
	size_t length = (size_t)(ftell(file));
	fseek(file, 0, SEEK_SET);

	char* content = malloc(length + 1);
	assert(content != NULL);

	fread(content, 1, length, file);

	content[length] = 0;

	fclose(file);

	return content;
}

int main(int argc, char* argv[])
{
	char* code = ReadFromFile("tests/subroutine.min");
	RunInterpreter(code);
	free(code);
	return 0;
}
