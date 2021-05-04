
#ifdef _VISUALC_CE_PH_
#include "stdafx.h"
#endif
#include "aparsablestring.h"

void AParsableString::SetTextInt(Tstring t){
    T = t;   // notranja spremnljivka
    x = 0;
    x1 = 0;   // zadnji delimiter
    DCount = -1; // neznano št. delimiterjev
}
//------------------------------------------------------------------------
int AParsableString::GetDCount(void){
    int x;
    // ce se ne vem koliko jih imam, jih prestejem
    if (DCount == -1){
        DCount = 0;
        x = 0;
        while ((x = PosOffset(x + 1, T, D)) > 0) DCount++;
    }
    return DCount;
}
//------------------------------------------------------------------------

int AParsableString::PosOffset(int Offset, Tstring &Text, Tstring &SearchFor){
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
//------------------------------------------------------------------------

Tstring AParsableString::GetNextSubString(void){
    x = x1;
    x1 = PosOffset(x + 1, T, D);
    if (x1 == 0){
        x1 = T.len() + 1;
        return T.substr(x + 1, T.len() - x);
        }
     else
        return T.substr(x + 1, x1 - x - 1);
}
//------------------------------------------------------------------------

Tstring AParsableString::GetSubString(int Position){
    x = 0;
    x1 = 0;
    for (int n = 0; n<Position; n++){
        x1 = PosOffset(x + 1, T, D);
        if (x1 == 0)
            return "";
        x = x1;
    }
    return GetNextSubString();
}
//------------------------------------------------------------------------

void AParsableString::ElimDubleDelim(void){
    char* Orig = (char*)T.c_str();
    char* Copy = (char*)malloc(T.len() + 1);
    int i, j = 0;

    char d = *D.c_str();  // delimiter v char obliki
    char LastChar = d;

    for (i = 0; i < T.len(); i++){
        if (Orig[i] == d) {
            if (LastChar != d){
                Copy[j] = Orig[i];
                j++;
            }
         }else {
            Copy[j] = Orig[i];
            j++;
        }
        LastChar = Orig[i];
    }
    Copy[j] = 0;
    T = Copy;
    D = d;
    free(Copy); Copy=NULL;
}

//------------------------------------------------------------------------

int AParsableString::GetCurBytePos (void){
    return x;
}
//------------------------------------------------------------------------

