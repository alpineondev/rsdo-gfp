//---------------------------------------------------------------------------

#ifndef MyStringsH
#define MyStringsH
#include "tstring.h"

long ChangeChar(char *Niz, long size, char SearchFor, char ReplaceWith);
long ChangeNCharsTo1(char *Niz, long size, char *Niz1, Tstring SearchFor, Tstring ReplaceWith);
long Change1CharToN(char *Niz, long size, char *Niz1, Tstring SearchFor, Tstring ReplaceWith);
long ElimChar(char* Niz, long size, char CharToElim);
int PosOffset(int Offset, Tstring &Text, Tstring SearchFor);
int CountChars(char *pBuf, int size, char CharToCount);

void StrRTrim(char* Niz);

long MyStrCat(long* Offset, char* dest, char* src);
long MyStrNCat(long* Offset, char* dest, char* src, long MaxLen);

//---------------------------------------------------------------------------
#endif
