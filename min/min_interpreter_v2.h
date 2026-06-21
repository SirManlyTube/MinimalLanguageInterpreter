#pragma once

#include <stdio.h>
#include <stdint.h>

#include "min_tokenizer.h"
#include "min_parser.h"

void RunInterpreterV2(const char* source)
{
	Vector tokens = Tokenize(source);

	Parse(&tokens);

	Vector_Destroy(&tokens);
}
