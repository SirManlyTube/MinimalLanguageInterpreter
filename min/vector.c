#include "vector.h"

#include <assert.h>
#include <stdlib.h>
#include <memory.h>

Vector Vector_Create(size_t elementSize, size_t initialCapacity)
{
	if (initialCapacity == 0) initialCapacity = 1;

	Vector vector = { 0 };
	vector.size = 0;
	vector.capacity = initialCapacity * elementSize;
	vector.elementSize = elementSize;
	vector.data = malloc(vector.capacity);
	assert(vector.data != NULL);

	return vector;
}

void Vector_PushBack(Vector* vector, void* element)
{
	assert(vector != NULL);
	assert(element != NULL);

	if (vector->size * vector->elementSize >= vector->capacity)
	{
		do
		{
			vector->capacity <<= 1;
		} while (vector->size * vector->elementSize >= vector->capacity);

		void* data = malloc(vector->capacity);
		assert(data != NULL);
		memcpy(data, vector->data, vector->size * vector->elementSize);
		free(vector->data);
		vector->data = data;
	}

	memmove((char*)vector->data + (vector->size * vector->elementSize), element, vector->elementSize);
	++vector->size;
}

void Vector_Destroy(Vector* vector)
{
	assert(vector != NULL);
	assert(vector->data != NULL);
	free(vector->data);
	vector->data = NULL;
	vector->size = 0;
	vector->capacity = 0;
	vector->elementSize = 0;
}
