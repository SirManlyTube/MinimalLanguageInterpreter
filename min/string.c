#include "string.h"

#include <stdlib.h>
#include <memory.h>
#include <assert.h>

String String_Create(int length, const char* c_str)
{
	String string = { 0 };
	string.size = length;
	string.capacity = length * sizeof(char);
	if (string.capacity <= 0)
	{
		string.capacity = 1;
	}
	string.data = malloc(string.capacity);
	assert(string.data != NULL);
	if (length == 0)
	{
		string.data[0] = 0;
	}
	else
	{
		memcpy(string.data, c_str, length);
	}

	return string;
}

String String_Empty(void)
{
	String string = { 0 };
	string.size = 0;
	string.capacity = sizeof(char);
	string.data = malloc(string.capacity);
	assert(string.data != NULL);
	string.data[0] = 0;

	return string;
}

void String_Concat(String* string, int length, const char* c_str)
{
	if (length <= 0) return;
	if ((string->size + length) * sizeof(char) >= string->capacity)
	{
		do
		{
			string->capacity <<= 1;
		} while ((string->size + length) * sizeof(char) >= string->capacity);

		char* buffer = malloc(string->capacity);
		assert(buffer != NULL);
		memcpy(buffer, string->data, string->size);
		free(string->data);
		string->data = buffer;
	}

	memcpy(string->data + string->size, c_str, length * sizeof(char));
	string->size += length;
}

void String_ConcatChar(String* string, const char c)
{
	if ((string->size + 1) * sizeof(char) >= string->capacity)
	{
		do
		{
			string->capacity <<= 1;
		} while (string->size * sizeof(char) >= string->capacity);

		char* buffer = malloc(string->capacity);
		assert(buffer != NULL);
		memcpy(buffer, string->data, string->size);
		free(string->data);
		string->data = buffer;
	}

	*(string->data + string->size++) = c;
	*(string->data + string->size) = 0;
}

void String_Destroy(String* string)
{
	free(string->data);
}

Vector String_Split(const String* string, const char delimiter)
{
	Vector split = VECTOR_CREATE(String, 1);
	String currentString = String_Empty();
	size_t cursor = 0;
	while (cursor < string->capacity)
	{
		if (string->data[cursor] == 0)
		{
			if (currentString.size > 0)
			{
				Vector_PushBack(&split, &currentString);
			}
			break;
		}
		if (string->data[cursor] == delimiter)
		{
			if (currentString.size > 0)
			{
				Vector_PushBack(&split, &currentString);
				currentString = String_Empty();
			}
		}
		else
		{
			String_ConcatChar(&currentString, string->data[cursor]);
		}
		++cursor;
	}
	if (currentString.size == 0)
	{
		String_Destroy(&currentString);
	}
	return split;
}
