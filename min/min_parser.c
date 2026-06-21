#include "min_parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

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

static const Token* g_tokens;
static size_t g_tokenCount;
static int g_programCounter = 0;

static VariableSymbol symbolTable[256];
static int variableCount = 0;

static SubroutineSymbol subroutineTable[256];
static int subroutineCount = 0;

static Stack subroutineStack;

static LineIndex* registeredLines;
static size_t registeredLineCount = 0;

static const Token* Peek(void)
{
	if (g_programCounter >= g_tokenCount) return NULL;
	return &g_tokens[g_programCounter];
}

static const Token* Advance(void)
{
	if (g_programCounter >= g_tokenCount) return NULL;
	return &g_tokens[g_programCounter++];
}

static void EmitSetInstruction(const Token* var, const Token* value)
{
	printf("Setting var %s to %s\n", var->lexeme, value->lexeme);
}

static void EmitPrintInstruction(const Token* value)
{
	printf("%s\n", value->lexeme);
}

static void EmitDelayInstruction(const Token* value)
{
	double durationMS = 0;
	switch (value->type)
	{
		case TOKEN_INT:
			durationMS = (double)((int)strtol(value->lexeme, NULL, 10) * 1000);
			break;
		case TOKEN_FLOAT:
			durationMS = (double)(strtof(value->lexeme, NULL) * 1000.0f);
			break;
		case TOKEN_DOUBLE:
			durationMS = strtod(value->lexeme, NULL) * 1000.0;
			break;
	}
	PrecisionDelay(durationMS);
}

static void EmitJumpInstruction(int targetLine)
{
	LineIndex index = registeredLines[targetLine - 1];

	g_programCounter = index.tokenIndex;
}

static void ParserSetKeyword(void)
{
	Advance();

	const Token* var = Advance();
	if (!var || var->type != TOKEN_IDENTIFIER)
	{
		fprintf(stderr, "Syntax Error: Expected variable name after SET.\n");
		exit(-1);
	}

	const Token* value = Advance();
	if (!value || value->type != TOKEN_INT && value->type != TOKEN_IDENTIFIER)
	{
		fprintf(stderr, "Syntax Error: Expected a number or variable value.\n");
		exit(-1);
	}

	EmitSetInstruction(var, value);
}

static void ParserPrintKeyword(void)
{
	Advance();

	const Token* value = Advance();
	if (!value || value->type != TOKEN_INT && value->type != TOKEN_FLOAT && value->type != TOKEN_DOUBLE && value->type != TOKEN_STRING && value->type != TOKEN_IDENTIFIER)
	{
		if (value) fprintf(stderr, "[%d] Syntax Error: Invalid PRINT value type.\n", value->line);
		else fprintf(stderr, "Syntax Error: Missing value after PRINT, reached end of file instead.\n");
		exit(-1);
	}

	EmitPrintInstruction(value);
}

static void ParserDelayKeyword(void)
{
	Advance();

	const Token* duration = Advance();
	if (!duration || duration->type != TOKEN_INT && duration->type != TOKEN_FLOAT && duration->type != TOKEN_DOUBLE)
	{
		if (duration)
		{
			if (duration->type == TOKEN_MINUS) fprintf(stderr, "[%d] Syntax Error: DELAY duration cannot be negative.\n", duration->line);
			else fprintf(stderr, "[%d] Syntax Error: Invalid DELAY value type. Value type must be either an int, a float, or a double.\n", duration->line);
		}
		else if (duration) fprintf(stderr, "Syntax Error: Missing duration after DELAY, reached end of file instead.\n");
		exit(-1);
	}

	EmitDelayInstruction(duration);
}

static void ParserJumpKeyword(void)
{
	Advance();

	const Token* first = Advance();
	if (!first || first->type != TOKEN_INT && first->type != TOKEN_MINUS)
	{
		if (first) fprintf(stderr, "[%d] Syntax Error: Invalid JUMP line offset value type. Value type must be an int.\n", first->line);
		else fprintf(stderr, "Syntax Error: Missing offset after JUMP, reached end of file instead.\n", first->line);
		exit(-1);
	}

	const Token* lineOffset;
	int offset = 0;

	if (first->type == TOKEN_MINUS)
	{
		lineOffset = Advance();
		if (!lineOffset || lineOffset->type != TOKEN_INT)
		{
			if (lineOffset) fprintf(stderr, "[%d] Syntax Error: Missing int on right side of negative sign.\n", first->line);
			else fprintf(stderr, "Syntax Error: Missing int on right side of negative sign, reached end of file instead.\n", first->line);
			exit(-1);
		}
		offset = -(int)strtol(lineOffset->lexeme, NULL, 10);
	}
	else
	{
		lineOffset = first;
		offset = (int)strtol(lineOffset->lexeme, NULL, 10);
	}

	int currentLine = lineOffset->line;
	int targetLine = currentLine + offset;

	if (targetLine >= registeredLineCount)
	{
		fprintf(stderr, "Error: Target line number %d out of range.\n", targetLine);
		exit(-1);
	}

	EmitJumpInstruction(targetLine);
}

void Parse(const Vector* tokens)
{
	Vector lineRegistry = VECTOR_CREATE(LineIndex, 1024);

	g_tokens = tokens->data;
	g_tokenCount = tokens->size;

	int lastLine = -1;
	for (size_t i = 0; i < g_tokenCount; ++i)
	{
		if (g_tokens[i].line != lastLine)
		{
			LineIndex newLine = { 0 };
			newLine.lineNumber = g_tokens[i].line;
			newLine.tokenIndex = i;

			Vector_PushBack(&lineRegistry, &newLine);

			lastLine = g_tokens[i].line;
		}
	}

	registeredLines = lineRegistry.data;
	registeredLineCount = lineRegistry.size;

	while (g_programCounter < g_tokenCount)
	{
		const Token* token = Peek();

		switch (token->type)
		{
			case TOKEN_KEYWORD_SET:
				ParserSetKeyword();
				break;
			case TOKEN_KEYWORD_PRINT:
				ParserPrintKeyword();
				break;
			case TOKEN_KEYWORD_DELAY:
				ParserDelayKeyword();
				break;
			case TOKEN_KEYWORD_JUMP:
				ParserJumpKeyword();
				break;
			default:
				Advance();
				break;
		}
	}

	Vector_Destroy(&lineRegistry);
}
