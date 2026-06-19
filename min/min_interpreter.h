#pragma once

extern int GetOrLocateSubroutine(const char* subroutineName);
extern int GetOrAllocateAddress(const char* tokenName);
extern void TrimString(char* str);
extern void RunInterpreter(const char* code);
