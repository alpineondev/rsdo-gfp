//---------------------------------------------------------------------------

#ifdef _VISUALC_CE_PH_
#include "stdafx.h"
#endif
#include "mystrings.h"
#include "tstring.h"

#define min(a, b)  (((a) < (b)) ? (a) : (b))

//---------------------------------------------------------------------------
// 1 v 1
long ChangeChar(char *Niz, long size, char SearchFor, char ReplaceWith){
    long i = 0; // pozicija v stringu
    long j = 0; // stevilo zamenjav

    while (i < size){
        if (Niz[i] == SearchFor){
            Niz[i] = ReplaceWith;
            j++;
        }
        i++;
    }

    return j;
}

//---------------------------------------------------------------------------
// zamenjava le n znakov v enega (n->1)
// vhod: Niz, izhod: Niz1
long ChangeNCharsTo1(char *Niz, long size, char *Niz1, Tstring SearchFor, Tstring ReplaceWith){
    long i = 0, j = 0;
    char PrviZnak = *SearchFor.c_str();
    int n = SearchFor.len();
    char ZamenjajZ = *ReplaceWith.c_str();

    while (i < size){
        if (Niz[i] == PrviZnak){
            if (memcmp(&Niz[i], SearchFor.c_str(), n) == 0){
                Niz1[j] = ZamenjajZ;
                i+=(n-1);
            } else
                Niz1[j] = Niz[i];
        } else
            Niz1[j] = Niz[i];


        i++;
        j++;
    }
    Niz1[j] = 0;
    return j;
}

//---------------------------------------------------------------------------

// 1 znak v n (1->n)
// vhod: Niz, izhod: Niz1
long Change1CharToN(char *Niz, long size, char *Niz1, Tstring SearchFor, Tstring ReplaceWith){
    long i = 0, j = 0;
    char IsciZnak = *SearchFor.c_str();
    int n = ReplaceWith.len();
    char* pBufRepl = (char*)ReplaceWith.c_str();

    while (i < size){
        if (Niz[i] == IsciZnak){
            memcpy(&Niz1[j], pBufRepl, n);
            j+=(n-1);
        } else
            Niz1[j] = Niz[i];
        i++;
        j++;
    }
    Niz1[j] = 0;
    return j;  // nova dolžina
}

//---------------------------------------------------------------------------

int PosOffset(int Offset, Tstring &Text, Tstring SearchFor){
    if (Offset <= 0) return 0;
    if (Offset > Text.len()) return 0;

    char *pBufStart = (char*)Text.c_str();
    char *pBufOffset = &pBufStart[Offset - 1];
    char c = *SearchFor.c_str();
    char *pOccurence = strchr(pBufOffset, c);

    if (pOccurence == NULL)
        return 0;
     else
        return Offset + (long)(pOccurence - pBufOffset);

}
//---------------------------------------------------------------------------

int CountChars(char *pBuf, int size, char CharToCount){
    int Cnt = 0;

    for (int n = 0; n < size; n++){
        if (pBuf[n] == CharToCount) Cnt++;
    }
    return Cnt;
}
//---------------------------------------------------------------------------

long ElimChar(char* Niz, long size, char CharToElim){
    char *Niz1 = (char*)malloc(size + 1); // novi niz bo krajsi ali enak izvornemu
    long i = 0, j = 0;

    while (i < size){
        if (Niz[i] != CharToElim){
            Niz1[j] = Niz[i];
            j++;
        }
        i++;
    }
    Niz1[j] = 0;
    memcpy(Niz, Niz1, j + 1);
    free(Niz1); Niz1=NULL;

    return j;
}
//---------------------------------------------------------------------------

void StrRTrim(char* Niz){
    if (Niz[0] == 0) return;

    char* ptr = &Niz[strlen(Niz) - 1];
    while (ptr >= Niz && (*ptr == ' ' || *ptr == '\r' || *ptr == '\n' || *ptr == '\t'))
        ptr--;

    *(++ptr) = 0;
}
//---------------------------------------------------------------------------
// strcat + parameter Offset = povemo kje v dest naj prilepimo src
long MyStrCat(long* Offset, char* dest, char* src){
    int n;

    for (n = 0; n < (int)strlen(src); n++){
        dest[*Offset] = src[n];
        (*Offset)++;
    }
    dest[*Offset] = 0;
    return n;
}
//---------------------------------------------------------------------------
// strncat + parameter Offset = povemo kje v dest naj prilepimo src
long MyStrNCat(long* Offset, char* dest, char* src, long MaxLen){
    int n;
    long _MaxLen = min(MaxLen, (long)strlen(src));

    for (n = 0; n < _MaxLen; n++){
        dest[*Offset] = src[n];
        (*Offset)++;
    }
    dest[*Offset] = 0;
    return n;
}
//---------------------------------------------------------------------------

