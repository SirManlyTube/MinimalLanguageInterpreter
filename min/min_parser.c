#include "min_parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "stack.h"

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
static int g_currentIndex = 0;

static VariableSymbol symbolTable[256];
static int variableCount = 0;

static SubroutineSymbol subroutineTable[256];
static int subroutineCount = 0;

static Stack subroutineStack;

static const Token* Peek(void)
{
	if (g_currentIndex >= g_tokenCount) return NULL;
	return &g_tokens[g_currentIndex];
}

static const Token* Advance(void)
{
	if (g_currentIndex >= g_tokenCount) return NULL;
	return &g_tokens[g_currentIndex++];
}

static void EmitPrintInstruction(const char* value, TokenType tokenType)
{
	printf("%s\n", value);
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
	if (!value || value->type != TOKEN_NUMBER && value->type != TOKEN_IDENTIFIER)
	{
		fprintf(stderr, "Syntax Error: Expected a number or variable value.\n");
		exit(-1);
	}
}

static void ParserPrintKeyword(void)
{
	Advance();

	const Token* value = Advance();
	if (!value || value->type != TOKEN_NUMBER && value->type != TOKEN_STRING && value->type != TOKEN_IDENTIFIER)
	{
		fprintf(stderr, "Syntax Error: Invalid print value type.\n");
		exit(-1);
	}

	EmitPrintInstruction(value->lexeme, value->type);
}

void Parse(const Vector* tokens)
{
	g_tokens = tokens->data;
	g_tokenCount = tokens->size;

	while (g_currentIndex < g_tokenCount)
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
			default:
				Advance();
				break;
		}
	}
}
