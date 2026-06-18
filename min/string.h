#pragma once

#include <stdint.h>
#include "vector.h"

typedef struct String
{
	size_t size;
	size_t capacity;
	char* data;
} String;

extern String String_Create(int length, const char* c_str);
extern String String_Empty(void);
extern void String_Concat(String* string, int length, const char* c_str);
extern void String_ConcatChar(String* string, const char c);
extern void String_Destroy(String* string);
extern Vector String_Split(const String* string, const char delimiter);
