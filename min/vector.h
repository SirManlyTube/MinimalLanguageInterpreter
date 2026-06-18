#pragma once

#include <stdint.h>

typedef struct Vector
{
	size_t size;
	size_t capacity;
	size_t elementSize;
	void* data;
} Vector;

extern Vector Vector_Create(size_t elementSize, size_t initialCapacity);
extern void Vector_PushBack(Vector* vector, void* element);
extern void Vector_Destroy(Vector* vector);

#define VECTOR_CREATE(_Ty, initialCapacity) Vector_Create(sizeof(_Ty), initialCapacity)
