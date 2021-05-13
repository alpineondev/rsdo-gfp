#ifdef _VISUALC_CE_PH_
#include "stdafx.h"
#endif
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "util_stuff.h"

#ifdef _BORLANDC_
#include <mem.h>
#elif defined(LINUX)
#include <memory.h>
#elif defined(_VISUALC_) || defined(_VISUALC_CE_)
  //
#else
#unknown OS
#endif

//---------------------------------------------------------------------------
#define BLOCK_BUFFER 16384
long Bread(void *kam, long koliko, FILE *od)
{// preberi v pomnilnik preko buf po kosckih
    long prebrano, lokacija = 0;
    int konec = 0;
    char buf[BLOCK_BUFFER];

    if (BLOCK_BUFFER <= koliko) prebrano = BLOCK_BUFFER; else prebrano = koliko;
    while (konec == 0) {
        prebrano = (long)fread(&buf, 1, prebrano, od);
        if (prebrano != BLOCK_BUFFER) konec = 1;
        memcpy((char *)kam + lokacija, &buf, prebrano);
        lokacija += prebrano;
        if ((koliko - lokacija) < BLOCK_BUFFER) prebrano = koliko - lokacija;
    }
    return lokacija;
}

//---------------------------------------------------------------------------

long Bwrite(void *kam, long koliko, FILE *od)
{// shrani po kosckih
    long prebrano, lokacija = 0;
    int konec = 0;
    char buf[BLOCK_BUFFER];

    if (BLOCK_BUFFER <= koliko) prebrano = BLOCK_BUFFER; else prebrano = koliko;
    while (konec == 0) {
        memcpy(&buf, (char *)kam + lokacija, prebrano);
        prebrano = (long)fwrite(&buf, 1, prebrano, od);
        if (prebrano != BLOCK_BUFFER) konec = 1;
        lokacija += prebrano;
        if ((koliko - lokacija) < BLOCK_BUFFER) prebrano = koliko - lokacija;
    }
    return lokacija;
}

//---------------------------------------------------------------------------
//druga vezija preberi po kosckih
size_t Bread2(void *base, size_t size, FILE *fp) {
    char *ptr = (char *)base;

    while (size > 0) {
        int n = (int)fread(ptr, 1, (size > BLOCK_BUFFER) ? BLOCK_BUFFER : size, fp);
        if (n <= 0) break;
        ptr += n;
        size -= n;
    }
    return ptr - (char *)base;
}

//---------------------------------------------------------------------------
//ali niz vsebuje naslednji znak
int Exists(char znak, const char *string) {
    const char* ptr;
    if (znak == 0) return 0;
    if ((ptr = strchr(string, znak)) != NULL)
        return ((long)(ptr - string)) + 1;
    return 0;
}


////////////////////////////////////////////////////////////////////////
// preprosta obdelava nizov
// niz se mora zaceti z " in koncati z "
// interpretira ubezne sekvence \a \n \t \v \b \r \f in \0
// ostale se interpretirajo kot navadni znaki ( \g -> g ...)

int toCstring(char *stavek)
{
    const char *atext = "antvbrf0";
    const char *ctext = "\a\n\t\v\b\r\f\0";

    int now = 0, next = 0;
    int dolz = (int)strlen(stavek);
    int index;

    while (stavek[now] != '\"') {
        now++;
        if (now >= dolz) return -1;
    }
    now++;
    for (;;) {
        if (stavek[now] == '\\') {
            now++;
            if (now >= dolz) return -1;
            if ((index = Exists(stavek[now], atext)) != 0) stavek[next] = ctext[index - 1];
            else stavek[next] = stavek[now];
            now++;
            next++;
        }
        else if (stavek[now] == '\"') {
            stavek[next] = 0;
            break;
        }
        else {
            stavek[next] = stavek[now];
            now++;
            next++;
        }
        if (now >= dolz) return -1;
    }
    return 0;
}

//---------------------------------------------------------------------------
//pretvarja med kodnimi stranmi

#define CP_COUNT 4
#define CP_LEN   CP_COUNT*10

//                ascii7 iso8859-2 cp1250 cp852
int OriginalCP[] = { 126,232,232,159,   //c^
                      94,200,200,172,   //C^
                      123,185,154,231,  //s^
                      91,169,138,230,   //S^
                      96,190,158,167,   //z^
                      64,174,142,166,   //Z^
                      126,230,230,134,  //c/
                      94,198,198,143,   //C/
                      125,240,240,208,  //d| -> }
                      127,208,208,209,  //D| -> ¦
                        0 };            //zakljuci niz

// Vedno prevede v interni ascii7 zapis
void ConvertCPcStr(char* Niz, const int cp_in) {
    int i = 0;
    if (cp_in == CPascii) return;
    while (Niz[i] != 0) {
        Niz[i] = ConvertCPc(Niz[i], cp_in);
        i++;
    }
}

//------------------------------------------------------------------------------

// Vedno prevede v interni ascii7 zapis
int ConvertCPc(int s, const int cp_in) {
    //CP1250   pretvori iz CP 1250
    //CPlatin2 pretvori iz ISO 8859-2 (ISO Latin 2)
    //CP852    pretvori iz CP 852
    int k;
    if (cp_in == CPascii) return s;
    for (k = 0; k < CP_LEN / CP_COUNT; k++) {
        if (s == OriginalCP[k*CP_COUNT + cp_in]) return OriginalCP[k*CP_COUNT];
    }
    return s;
}
//------------------------------------------------------------------------------

bool IsInCPc(int s, const int cp_in) {
    int k;
    for (k = 0; k < CP_LEN / CP_COUNT; k++) {
        if (s == OriginalCP[k*CP_COUNT + cp_in]) return true;
    }
    return false;
}

//------------------------------------------------------------------------------

//brise vse presledke pred in za nizom
//ce se v samem nizu pojavi kaksen presledek ga tam pusti
char Spaces[5] = { 0x09,0x0A,0x0D,32,'\0' };
int SpacesOut(char *niz)
{
    int i;
    while (Exists(niz[0], Spaces) != 0) for (i = 0; i < (int)strlen(niz); i++) niz[i] = niz[i + 1];
    i = (int)strlen(niz) - 1;
    if (i > 0) while (Exists(niz[i], Spaces) != 0) niz[i--] = '\0';
    return 0;
}
//------------------------------------------------------------------------------

char *Lower(char *niz) {
    for (unsigned int i = 0; i < strlen(niz); i++) niz[i] = tolower(niz[i]);
    return niz;
}

//------------------------------------------------------------------------------

char *GetExtension(const char *filename) {
    static char buf[128];
    const char *pika = filename;

    strcpy(buf, "");
    while ((pika = strchr(pika, '.')) != NULL) {
        strcpy(buf, pika + 1);
        pika++;
    }
    ::Lower(buf);
    return buf;
}

//------------------------------------------------------------------------------

bool AreExtensionsEqual(const char *a, const char *b) {
    char exta[1024], extb[1024];

    if (a == NULL || b == NULL) return false;
    strcpy(exta, GetExtension(a));
    strcpy(extb, GetExtension(b));

    return strcmp(exta, extb) == 0;
}

//------------------------------------------------------------------------------
