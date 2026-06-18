#include "stack.h"

#include <assert.h>
#include <stdlib.h>
#include <memory.h>

Stack Stack_Create(size_t elementSize, size_t initialCapacity)
{
	Stack stack = { 0 };
	stack.size = 0;
	stack.capacity = initialCapacity == 0 ? elementSize : (initialCapacity * elementSize);
	stack.elementSize = elementSize;
	stack.data = malloc(stack.capacity);
	assert(stack.data != NULL);
	return stack;
}

void Stack_PushInt(Stack* stack, int element)
{
	if (stack->size * stack->elementSize >= stack->capacity)
	{
		do
		{
			stack->capacity <<= 1;
		} while (stack->size * stack->elementSize >= stack->capacity);

		void* data = malloc(stack->capacity);
		assert(data != NULL);
		memcpy(data, stack->data, stack->size * stack->elementSize);
		free(stack->data);
		stack->data = data;
	}

	((int*)stack->data)[stack->size++] = element;
}

int Stack_PopInt(Stack* stack)
{
	return ((int*)stack->data)[--stack->size];
}

void Stack_Destroy(Stack* stack)
{
	free(stack->data);
}
