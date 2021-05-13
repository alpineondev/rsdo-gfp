#ifndef _UTILSTUFFH_
#define _UTILSTUFFH_

#ifdef DISABLE_WARNING_4996
    #pragma warning(disable : 4996)
#endif

#include <stdio.h>
#include "tstring.h"

#define  CPascii   0
#define  CPlatin2  1
#define  CP1250    2
#define  CP852     3

//------------------------------------------------------------------------------

long Bread(void *kam,long koliko,FILE *od);
long Bwrite(void *kam,long koliko,FILE *od);
size_t Bread2(void *base,size_t size, FILE *fp);
int toCstring(char *stavek);
int Exists(char znak, const char *string);
int SpacesOut(char *niz);
char *Lower(char *niz);
int ConvertCPc(int s, const int cp_in);
void ConvertCPcStr(char* Niz, const int  cp_in);
bool IsInCPc(int s, const int cp_in);
char *GetExtension(char *filename);
bool AreExtensionsEqual(const char *a, const char *b);

#endif

