#pragma once

#include <stdint.h>

typedef struct Stack
{
	size_t size;
	size_t capacity;
	size_t elementSize;
	void* data;
} Stack;

extern Stack Stack_Create(size_t elementSize, size_t initialCapacity);
extern void Stack_PushInt(Stack* stack, int element);
extern int Stack_PopInt(Stack* stack);
extern void Stack_Destroy(Stack* stack);
