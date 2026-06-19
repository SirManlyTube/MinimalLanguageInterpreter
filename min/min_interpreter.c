#include "min_interpreter.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <memory.h>
#include <assert.h>
#include <string.h>

#include "string.h"
#include "vector.h"
#include "stack.h"
#include "precision_delay.h"

typedef struct VariableSymbol
{
	char name[32];
	int memoryIndex;
} VariableSymbol;

typedef struct SubroutineSymbol
{
	char name[32];
	int lineNumber;
} SubroutineSymbol;

VariableSymbol symbolTable[256];
int variableCount = 0;

SubroutineSymbol subroutineTable[256];
int subroutineCount = 0;

Stack subroutineStack;

Vector lines = { 0 };

int GetOrLocateSubroutine(const char* subroutineName)
{
	for (int i = 0; i < subroutineCount; ++i)
	{
		if (strcmp(subroutineTable[i].name, subroutineName) == 0)
		{
			return subroutineTable[i].lineNumber;
		}
	}

	for (size_t i = 0; i < lines.size; ++i)
	{
		String currentLine = ((String*)lines.data)[i];

		Vector tokens = String_Split(&currentLine, ' ');

		if (tokens.size > 1)
		{
			String* s_tokens = (String*)tokens.data;

			if (strcmp(s_tokens[0].data, "SUBROUTINE") == 0)
			{
				const char* name = s_tokens[1].data;
				if (strcmp(name, subroutineName) == 0)
				{
					SubroutineSymbol symbol = { 0 };
					strcpy(symbol.name, name);
					symbol.lineNumber = i + 1;
					subroutineTable[subroutineCount++] = symbol;
					return symbol.lineNumber;
				}
			}
		}
	}

	return -1;
}

int GetOrAllocateAddress(const char* tokenName)
{
	if (tokenName[0] == '0' && tokenName[1] == 'x')
		return (int)strtol(tokenName, NULL, 16);

	for (int i = 0; i < variableCount; ++i)
	{
		if (strcmp(symbolTable[i].name, tokenName) == 0)
		{
			return symbolTable[i].memoryIndex;
		}
	}

	int assignedSlot = variableCount;
	strcpy(symbolTable[variableCount].name, tokenName);
	symbolTable[variableCount].memoryIndex = assignedSlot;
	++variableCount;

	return assignedSlot;
}

void TrimString(char* str)
{
	int len = strlen(str);
	while (len > 0 && (str[len - 1] == ' ' || str[len - 1] == '\t' || str[len - 1] == '\r' || str[len - 1] == '\n'))
	{
		str[len - 1] = '\0';
		--len;
	}
}

void RunInterpreter(const char* code)
{
	subroutineStack = Stack_Create(sizeof(int), 1);

	int sourceLength = 0;

	lines = VECTOR_CREATE(String, 1);
	{
		String newString = String_Empty();
		Vector_PushBack(&lines, &newString);
	}

	int currentLineNum = 0;

	while (code[sourceLength] != 0)
	{
		if (code[sourceLength] == '\n' || code[sourceLength] == '\r')
		{
			if (code[sourceLength] == '\r' && code[sourceLength + 1] == '\n')
			{
				++sourceLength;
			}
			String newString = String_Empty();
			Vector_PushBack(&lines, &newString);
			++currentLineNum;
		}
		else
		{
			String_ConcatChar(&((String*)lines.data)[currentLineNum], code[sourceLength]);
		}
		++sourceLength;
	}

	int programCounter = 1;

	int memory[256] = { 0 };

	while (programCounter < lines.size)
	{
		String currentLine = ((String*)lines.data)[programCounter - 1];

		Vector tokens = String_Split(&currentLine, ' ');
		String* s_tokens = (String*)tokens.data;

		int cursor = 0;
		bool skipLine = true;
		while (currentLine.data[cursor] != 0)
		{
			if (currentLine.data[cursor] != ' ' &&
				currentLine.data[cursor] != '\t' &&
				currentLine.data[cursor] != '\n' &&
				currentLine.data[cursor] != '\r' &&
				currentLine.data[cursor] != '\v')
			{
				skipLine = false;
				break;
			}
			++cursor;
		}
		if (!skipLine)
		{
			const char* cmd = s_tokens[0].data;

			TrimString((char*)cmd);

			if (strcmp(s_tokens[0].data, "END") == 0)
			{
				break;
			}

			if (strcmp(cmd, "SET") == 0)
			{
				int idx = GetOrAllocateAddress(s_tokens[1].data);
				int value = (int)strtol(s_tokens[2].data, NULL, 10);
				memory[idx] = value;
				programCounter += 1;
			}
			else if (strcmp(cmd, "ADD") == 0)
			{
				int idx = GetOrAllocateAddress(s_tokens[1].data);
				int rhs = (int)strtol(s_tokens[2].data, NULL, 10);
				memory[idx] += rhs;
				programCounter += 1;
			}
			else if (strcmp(cmd, "SUB") == 0)
			{
				int idx = GetOrAllocateAddress(s_tokens[1].data);
				int rhs = (int)strtol(s_tokens[2].data, NULL, 10);
				memory[idx] -= rhs;
				programCounter += 1;
			}
			else if (strcmp(cmd, "MUL") == 0)
			{
				int idx = GetOrAllocateAddress(s_tokens[1].data);
				int rhs = (int)strtol(s_tokens[2].data, NULL, 10);
				memory[idx] *= rhs;
				programCounter += 1;
			}
			else if (strcmp(cmd, "DIV") == 0)
			{
				int idx = GetOrAllocateAddress(s_tokens[1].data);
				int rhs = (int)strtol(s_tokens[2].data, NULL, 10);
				memory[idx] /= rhs;
				programCounter += 1;
			}
			else if (strcmp(cmd, "XOR") == 0)
			{
				int idx = GetOrAllocateAddress(s_tokens[1].data);
				int rhs = (int)strtol(s_tokens[2].data, NULL, 10);
				memory[idx] ^= rhs;
				programCounter += 1;
			}
			else if (strcmp(cmd, "AND") == 0)
			{
				int idx = GetOrAllocateAddress(s_tokens[1].data);
				int rhs = (int)strtol(s_tokens[2].data, NULL, 10);
				memory[idx] &= rhs;
				programCounter += 1;
			}
			else if (strcmp(cmd, "OR") == 0)
			{
				int idx = GetOrAllocateAddress(s_tokens[1].data);
				int rhs = (int)strtol(s_tokens[2].data, NULL, 10);
				memory[idx] |= rhs;
				programCounter += 1;
			}
			else if (strcmp(cmd, "NOT") == 0)
			{
				int idx = GetOrAllocateAddress(s_tokens[1].data);
				memory[idx] = !memory[idx];
				programCounter += 1;
			}
			else if (strcmp(cmd, "BITNOT") == 0)
			{
				int idx = GetOrAllocateAddress(s_tokens[1].data);
				memory[idx] = ~memory[idx];
				programCounter += 1;
			}
			else if (strcmp(cmd, "IF") == 0)
			{
				if (strcmp(s_tokens[1].data, "NOT") == 0)
				{
					if (strcmp(s_tokens[3].data, "JUMP") == 0)
					{
						int idx = GetOrAllocateAddress(s_tokens[2].data);
						int targetLine = (int)strtol(s_tokens[4].data, NULL, 10);
						if (memory[idx] == 0)
						{
							programCounter += targetLine;
						}
						else
						{
							programCounter += 1;
						}
					}
					else if (strcmp(s_tokens[3].data, "GOSUB") == 0)
					{
						int idx = GetOrAllocateAddress(s_tokens[2].data);
						int targetLine = GetOrLocateSubroutine(s_tokens[4].data);
						assert(targetLine != -1);
						if (memory[idx] == 0)
						{
							Stack_PushInt(&subroutineStack, programCounter);
							programCounter += targetLine;
						}
						else
						{
							programCounter += 1;
						}
					}
				}
				else if (strcmp(s_tokens[2].data, "JUMP") == 0)
				{
					int idx = GetOrAllocateAddress(s_tokens[1].data);
					int targetLine = (int)strtol(s_tokens[3].data, NULL, 10);
					if (memory[idx] != 0)
					{
						programCounter += targetLine;
					}
					else
					{
						programCounter += 1;
					}
				}
				else if (strcmp(s_tokens[2].data, "GOSUB") == 0)
				{
					int idx = GetOrAllocateAddress(s_tokens[1].data);
					int targetLine = GetOrLocateSubroutine(s_tokens[3].data);
					assert(targetLine != -1);
					if (memory[idx] == 0)
					{
						Stack_PushInt(&subroutineStack, programCounter);
						programCounter = targetLine + 1;
					}
					else
					{
						programCounter += 1;
					}
				}
			}
			else if (strcmp(cmd, "JUMP") == 0)
			{
				int targetLine = (int)strtol(s_tokens[1].data, NULL, 10);
				programCounter += targetLine;
			}
			else if (strcmp(cmd, "PRINT") == 0)
			{
				int idx = GetOrAllocateAddress(s_tokens[1].data);
				printf("%d\n", memory[idx]);
				programCounter += 1;
			}
			else if (strcmp(cmd, "GOSUB") == 0)
			{
				int lineNumber = GetOrLocateSubroutine(s_tokens[1].data);
				assert(lineNumber != -1);
				Stack_PushInt(&subroutineStack, programCounter);
				programCounter = lineNumber + 1;
			}
			else if (strcmp(cmd, "RETURN") == 0)
			{
				if (subroutineStack.size > 0)
				{
					int lineNumber = Stack_PopInt(&subroutineStack);
					programCounter = lineNumber + 1;
				}
			}
			else if (strcmp(cmd, "SUBROUTINE") == 0)
			{
				int skipCounter = programCounter + 1;
				bool foundReturn = false;

				while (skipCounter < lines.size)
				{
					String scanLine = ((String*)lines.data)[skipCounter - 1];
					Vector scanTokens = String_Split(&scanLine, ' ');

					if (scanTokens.size > 0)
					{
						String* s_scanTokens = (String*)scanTokens.data;
						if (strcmp(s_scanTokens[0].data, "RETURN") == 0)
						{
							programCounter = skipCounter + 1;
							foundReturn = true;
						}
					}

					for (size_t i = 0; i < scanTokens.size; ++i)
					{
						String_Destroy(&((String*)scanTokens.data)[i]);
					}
					Vector_Destroy(&scanTokens);

					if (foundReturn)
					{
						break;
					}

					++skipCounter;
				}

				if (!foundReturn)
				{
					programCounter += 1;
				}
			}
			else if (strcmp(cmd, "DELAY") == 0)
			{
				int msToWait = (int)strtol(s_tokens[1].data, NULL, 10);

				PrecisionDelay((double)msToWait);
				programCounter += 1;
			}
			else
			{
				programCounter += 1;
			}
		}
		else
		{
			programCounter += 1;
		}

		for (size_t i = 0; i < tokens.size; ++i)
		{
			String_Destroy(&((String*)tokens.data)[i]);
		}

		Vector_Destroy(&tokens);
	}

	for (size_t i = 0; i < lines.size; ++i)
	{
		String_Destroy(&((String*)lines.data)[i]);
	}

	Vector_Destroy(&lines);
	Stack_Destroy(&subroutineStack);
}
