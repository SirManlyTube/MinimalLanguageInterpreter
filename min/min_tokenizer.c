#include "min_tokenizer.h"

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

Vector Tokenize(const char* source)
{
	Vector tokens = VECTOR_CREATE(Token, 1);

	char tokenBuffer[64];

	bool commentMode = false;
	int currentLine = 1;

	while (*source != '\0')
	{
		if (commentMode)
		{
			continue;
		}

		if (isspace(*source))
		{
			if (*source == '\n')
			{
				commentMode = false;
				++currentLine;
			}

			++source;
			continue;
		}

		if (*source == '"')
		{
			++source;
			int i = 0;
			while (*source != '"' && *source != '\0')
			{
				tokenBuffer[i++] = *source++;
			}
			tokenBuffer[i] = '\0';
			++source;

			Token token = { 0 };
			token.type = TOKEN_STRING;
			strcpy_s(token.lexeme, 63, tokenBuffer);
			token.line = currentLine;

			Vector_PushBack(&tokens, &token);

			continue;
		}

		if (isalpha(*source))
		{
			int i = 0;
			while (isalnum(*source) || *source == '_')
			{
				tokenBuffer[i++] = *source++;
			}
			tokenBuffer[i] = '\0';

			TokenType type = TOKEN_IDENTIFIER;

			for (int j = 0; j < sizeof(g_tokenDefinitions) / sizeof(g_tokenDefinitions[0]); ++j)
			{
				if (strcmp(tokenBuffer, g_tokenDefinitions[j].lexeme) == 0)
				{
					type = g_tokenDefinitions[j].type;
					break;
				}
			}

			Token token = { 0 };
			token.type = type;
			strcpy_s(token.lexeme, 63, tokenBuffer);
			token.line = currentLine;

			Vector_PushBack(&tokens, &token);

			continue;
		}

		if (isdigit(*source) || *source == '.')
		{
			TokenType numType = TOKEN_INT;

			bool hasDecimal = false;

			int i = 0;
			while (isdigit(*source) || *source == '.' || *source == '\'')
			{
				if (*source == '.')
				{
					if (hasDecimal)
					{
						fprintf(stderr, "Syntax Error: Number cannot have two decimals!\n");
						exit(-1);
					}
					hasDecimal = true;
					numType = TOKEN_DOUBLE;
				}
				tokenBuffer[i++] = *source++;
			}
			tokenBuffer[i] = '\0';

			if (*source == 'f' || *source == 'F')
			{
				numType = TOKEN_FLOAT;
				++source;
			}

			Token token = { 0 };
			token.type = numType;
			strcpy_s(token.lexeme, 63, tokenBuffer);
			token.line = currentLine;

			Vector_PushBack(&tokens, &token);

			continue;
		}

		int validDefinitions;
		TokenDefinition definition;

		int i = 0;
		int lastValidIteration = 0;

		do
		{
			validDefinitions = 0;
			tokenBuffer[i++] = *source++;
			tokenBuffer[i] = '\0';
			for (int j = 0; j < sizeof(g_tokenDefinitions) / sizeof(g_tokenDefinitions[0]); ++j)
			{
				if (strcmp(tokenBuffer, g_tokenDefinitions[j].lexeme) == 0)
				{
					definition = g_tokenDefinitions[j];
					++validDefinitions;
				}
			}
			if (validDefinitions > 0) lastValidIteration = i;
		} while (validDefinitions > 1);

		tokenBuffer[i] = '\0';

		if (validDefinitions == 1)
		{
			if (definition.type == TOKEN_COMMENT)
			{
				commentMode = true;
			}
			else
			{
				Token token = { 0 };
				token.type = definition.type;
				strcpy_s(token.lexeme, 63, tokenBuffer);
				token.line = currentLine;

				Vector_PushBack(&tokens, &token);
			}
		}
		else if (validDefinitions == 0)
		{
			if (i > 1 && lastValidIteration != i - 1)
			{
				fprintf(stderr, "Unknown token found: %s\n", tokenBuffer);
				exit(-1);
			}
			else
			{
				if (definition.type == TOKEN_COMMENT)
				{
					tokenBuffer[0] = '\0';

					--source;

					commentMode = true;
				}
				else
				{
					tokenBuffer[i - 1] = '\0';

					--source;

					Token token = { 0 };
					token.type = definition.type;
					strcpy_s(token.lexeme, 63, tokenBuffer);
					token.line = currentLine;

					Vector_PushBack(&tokens, &token);
				}
			}
		}
	}

	return tokens;
}
