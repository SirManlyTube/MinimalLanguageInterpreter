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

	while (*source != '\0')
	{
		if (commentMode)
		{
			if (*source++ == '\n')
			{
				commentMode = false;
			}
			continue;
		}

		if (isspace(*source))
		{
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

			Vector_PushBack(&tokens, &token);

			continue;
		}

		if (isdigit(*source))
		{
			int i = 0;
			while (isdigit(*source) || *source == '\'')
			{
				tokenBuffer[i++] = *source++;
			}
			tokenBuffer[i] = '\0';

			Token token = { 0 };
			token.type = TOKEN_NUMBER;
			strcpy_s(token.lexeme, 63, tokenBuffer);

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

					Vector_PushBack(&tokens, &token);
				}
			}
		}
	}

	return tokens;
}
