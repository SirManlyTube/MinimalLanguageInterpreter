#pragma once

typedef enum TokenType
{
	TOKEN_KEYWORD_IF,
	TOKEN_KEYWORD_NOT,
	TOKEN_KEYWORD_JUMP,
	TOKEN_KEYWORD_PRINT,
	TOKEN_KEYWORD_DELAY,
	TOKEN_KEYWORD_SET,
	TOKEN_KEYWORD_GOSUB,
	TOKEN_KEYWORD_SUBROUTINE,
	TOKEN_KEYWORD_RETURN,
	TOKEN_KEYWORD_ADD,
	TOKEN_KEYWORD_SUB,
	TOKEN_KEYWORD_MUL,
	TOKEN_KEYWORD_DIV,
	TOKEN_KEYWORD_END,
	TOKEN_IDENTIFIER,
	TOKEN_INT,
	TOKEN_FLOAT,
	TOKEN_DOUBLE,
	TOKEN_STRING,
	TOKEN_MINUS,
	TOKEN_MINUS_MINUS,
	TOKEN_COMMENT,
} TokenType;

typedef struct TokenDefinition
{
	TokenType type;
	char lexeme[64];
} TokenDefinition;

typedef struct Token
{
	TokenType type;
	char lexeme[64];
	int line;
} Token;

typedef struct LineIndex
{
	int lineNumber;
	int tokenIndex;
} LineIndex;

static TokenDefinition g_tokenDefinitions[] = {
	{.type = TOKEN_MINUS, .lexeme = "-"},
	{.type = TOKEN_KEYWORD_IF, .lexeme = "IF"},
	{.type = TOKEN_KEYWORD_NOT, .lexeme = "NOT"},
	{.type = TOKEN_KEYWORD_JUMP, .lexeme = "JUMP"},
	{.type = TOKEN_KEYWORD_PRINT, .lexeme = "PRINT"},
	{.type = TOKEN_KEYWORD_DELAY, .lexeme = "DELAY"},
	{.type = TOKEN_KEYWORD_SET, .lexeme = "SET"},
	{.type = TOKEN_KEYWORD_GOSUB, .lexeme = "GOSUB"},
	{.type = TOKEN_KEYWORD_SUBROUTINE, .lexeme = "SUBROUTINE"},
	{.type = TOKEN_KEYWORD_RETURN, .lexeme = "RETURN"},
	{.type = TOKEN_KEYWORD_ADD, .lexeme = "ADD"},
	{.type = TOKEN_KEYWORD_SUB, .lexeme = "SUB"},
	{.type = TOKEN_KEYWORD_MUL, .lexeme = "MUL"},
	{.type = TOKEN_KEYWORD_DIV, .lexeme = "DIV"},
	{.type = TOKEN_KEYWORD_END, .lexeme = "END"},
	{.type = TOKEN_COMMENT, .lexeme = "#"},
};
