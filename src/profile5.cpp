/******************************************************************************
Funkcije za delo s profile datotekami
*******************************************************************************

1) format datoteke:

----------------------------------------------------
ime_spremenljivke1 = vrednost_spr1
ime_spremenljivke2 = vrednost_spr2
ime_polja1         = vrednost_11, vrednost_12, vrednost_13,
                     rednost_14, vrednost_15, vrednost_16

ime_polja2     = vrednost_21    vrednost_22
                 vrednost_23    vrednost_24

# to je komentar
----------------------------------------------------

2) funkcije:

   prf_open
        - Odpre profile datoteko, alocira pomnilnik za celotno vsebino
          datoteke, prebere vsebino v pomninik in zapre datoteko. Vrne
          -1 ob napaki, 0 sicer.

        * int prf_open(char* ime_datoteke);

   prf_close
        - Dealocira pomnilnik, ki ga zaseda prebrana vsebina profile
          datoteke. Vedno vrne 0.

        * int prf_close();

   prf_get_int
   prf_get_long
   prf_get_double
        - Prebere vrednost izbrane spremenljivke tipa integer, long,
          ali double. Vrne -1 ob napaki, 0 sicer.

        * int prf_get_int(char* ime_spr,
                  int*  kaz_na_int);

        * int prf_get_long(char* ime_spr,
                   long* kaz_na_long);

        * int prf_get_double(char*   ime_spr,
                     double* kaz_na_double);

   prf_get_str
        - Prebere (prepise) string z izbranim imenom. Vrne dolzino
          prebranega stringa ali -1, ce string ne obstaja.

        * int prf_get_str(char* ime_spr,
                  char* kaz_na_char);

   prf_get_strl
        - Vrne dolzino string a z izbranim imenom oz. -1 ce string
          ne obstaja.

        * int prf_get_strl(char* ime_spr);

   prf_get_count
        - Vrne stevilo elementov v polju z izbranim imenom, ki so
          loceni s podanim separatorjem oz. vrne -1 ce polje s podanim
          imenom ne obstaja.

        * int prf_get_count(char* ime_spr,
                    char  separator);

   prf_get_stra
        - Postavi kazalce na zacetke posameznih stringov v polju
          stringov z izbranim imenom. Posamezni elementi polja v
          profile datoteki morajo biti loceni s separatorjem ali
          morajo biti v locenih vrsticah. Funkcija vrne stevilo
          elementov polja ali -1, ce polje z navedenim imenom ne
          obstaja.

        * int prf_get_stra(char*  ime_spr,
                   char   separator,
                   char** kaz_na_polje_kaz_na_char);

   prf_get_inta
        - Prebere vec integer vrednosti iz izbranega polja v
          profile datoteki v podano polje integer-jev. Posamezni
          elementi polja v profile datoteki morajo biti loceni s
          separatorjem ali morajo biti v locenih vrsticah. Funkcija
          vrne stevilo elementov polja ali -1, ce polje s navedenim
          imenom ne obstaja.

        * int prf_get_inta(char* ime_spr,
                   char  separator,
                   int*  kaz_na_polje_int);

   prf_get_array
        - Prebere vec vrednosti dolocenega tipa iz izbranega polja v
          profile datoteki v podano polje spremenljivk istega tipa.
          Posamezni elementi polja v profile datoteki morajo biti
          loceni s separatorjem ali morajo biti v locenih vrsticah.
          Tip elementov polja je lahko int, long, float ali double,
          dolocimo pa ga z predefiniranimi konstantami

            PRF_INT, PRF_LONG, PRF_FLOAT in PRF_DOUBLE

          Funkcija vrne stevilo elementov polja ali -1, ce polje s
          navedenim imenom ne obstaja.

        * int prf_get_array(char* ime_spremenljivke,
                    char  separator,
                    void* kaz_na_polje,
                    int   tip_elementov_polja);

   prf_make_stra
        - Alocira prostor za zadostno polje kazalcev na stringe in
          postavi kazalce na zacetke posameznih stringov v polju
          stringov z izbranim imenom. Posamezni elementi polja v
          profile datoteki morajo biti loceni s separatorjem ali
          morajo biti v locenih vrsticah. Funkcija vrne stevilo
          elementov polja ali -1, ce polje s navedenim imenom ne
          obstaja, ali ce ni bilo mozno alocirati pomnilnik za
          kazalce.

        * int prf_get_stra(char*   ime_spr,
                   char    separator,
                   char*** kazalec_na_kaz_na_kazalce_na_char);

*******************************************************************************/

#ifdef _VISUALC_CE_PH_
#include "stdafx.h"
#endif
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "profile5.h"
#include "util_stuff.h"
#include "ffind.h"

Profile::Profile(char *name) {
    SetIniName(name);
    prf_data = NULL;
    prf_index = NULL;
    prf_open();
}

Profile::~Profile() { prf_close(); }
int Profile::IsOk() { return OK; }
//----------------------------------------------------------

bool Profile::SetIniName(char *name) {
    if (strlen(name) > MAX_INI_NAME) return false;
    strncpy(ini_name, name, MAX_INI_NAME);
    return true;
}

const char *Profile::GetIniName(void) {
    return ini_name;
}

//----------------------------------------------------------

int Profile::prf_open(void) {
    FILE    *fd;
    int line;
    long    len;
    long    n;
    long    x;
    long    end;
    char*   ptr;
    unsigned char *uptr;

    if (prf_data != NULL) {
        free(prf_data);
        prf_data = NULL;
    }
    if (prf_index != NULL) {
        free(prf_index);
        prf_index = NULL;
    }
    OK = false;

    // ----- odpiranje datoteke

    fd = fopen(ini_name, "rb");
    if (fd == NULL) {
        return -1;
    }

    len = filesize(fd);

    fseek(fd, 0, SEEK_SET);

    // ----- alokacija pomnilnika

    prf_data = (char*)malloc(len + 1);
    if (prf_data == NULL) {
        return(-1);
    }
    // ----- branje datoteke v buffer

    ptr = prf_data;
    x = Bread(ptr, len, fd);
    if (len != x) {
        return(-1);
    }

    fclose(fd);
    prf_data[x] = 0;

    // ----- stetje stevila spremenljivk

    ptr = prf_data;
    uptr = (unsigned char*)prf_data;
    for (x = 0, prf_items = 0; x < len; x++, uptr++, ptr++) {
        if ((isalnum((*uptr))) && ((x == 0) || (*(uptr - 1) == 13) || (*(uptr - 1) == 10))) prf_items++;
    }
    // ----- alokacija pomnilnika za kazalce

    prf_index = (char**)malloc(2 * prf_items * sizeof(char*));
    if (prf_index == NULL) {
        //Napaka pri alokaciji pomnilnika (profile-kazalci)
        return(-1);
    }

    // ----- postavitev kazalcev na besede

    for (n = 0, x = 0, line = 1, ptr = prf_data, uptr = (unsigned char*)prf_data; x < len; uptr++, ptr++, x++) {
        if ((isalnum(*uptr)) && ((x == 0) || (*(uptr - 1) == 13) || (*(uptr - 1) == 10))) {
            if (x > 0) *(uptr - 1) = 0;
            prf_index[2 * n] = ptr;
            while ((x < len) && (isalnum(*uptr) || (*uptr == '_'))) { x++; uptr++; ptr++; }
            end = x;
            while ((x < len) && ((*uptr == '\t') || (*uptr == ' '))) { x++; uptr++; ptr++; }
            if (*uptr != '=') {
                //Napaka v profile datoteki - vrstica line
                return(-1);
            }
            prf_data[end] = 0;
            prf_index[2 * n + 1] = ptr + 1;
            n++;
        }
        if ((*uptr == '#') && ((x == 0) || (*(uptr - 1) == 13) || (*(uptr - 1) == 10))) *ptr = 0;
        if (*uptr == 13) line++;
    }
    OK = true;
    return 0;
}

// *****************************************************************

int Profile::prf_close(void)
{
    if (prf_data != NULL) {
        free(prf_data);
        prf_data = NULL;
    }
    if (prf_index != NULL) {
        free(prf_index);
        prf_index = NULL;
    }
    OK = false;
    return 0;
}

// *****************************************************************

int Profile::prf_get_strl(const char *name)
{
    unsigned char* data;
    int   x, l;
    int   count;
    int   add;
    unsigned char  prev;

    data = (unsigned char*)prf_get_data(name);
    if (data == NULL) return(-1);

    for (x = 0, l = 1, count = 0, add = 0, prev = '.'; data[x] != 0; x++) {
        if ((data[x] == 10) || (data[x] == 13)) {
            if ((l == 0) && (count > 0) && (!isspace(prev))) { prev = ' '; count++; add = 1; }
            l = 1;
        }
        else if (isspace(data[x]) && (l == 1)) l = 1;
        else { l = 0; prev = data[x]; count++; add = 0; }
    }

    if (add == 1) count--;
    return (count);
}

// *****************************************************************

int Profile::prf_get_count(const char *name, char sep)
{
    const unsigned char* data;
    int   x, s, l;
    int   count;

    data = (const unsigned char*)prf_get_data(name);
    if (data == NULL) return(-1);

    for (x = 0, s = 1, l = 1, count = 0; data[x] != 0; x++) {
        if (data[x] == sep) { s = 1; l = 0; }
        else if ((data[x] == 10) || (data[x] == 13)) { s = 1; l = 1; }
        else if (isspace(data[x]) && (l == 1)) { s = 1; l = 1; }
        else if (s == 1) { s = 0; l = 0; count++; }
    }
    return (count);
}

// *****************************************************************

const char* Profile::prf_get_data(const char *name)
{
    int i;
    for (i = 0; i < prf_items; i++) {
        if (strcmp(name, prf_index[2 * i]) == 0) return (prf_index[2 * i + 1]);
    }
    return (NULL);
}

// *****************************************************************

int Profile::prf_get_str(const char *name, char *str)
{
    unsigned char* data;
    unsigned char* ustr = (unsigned char*)str;
    long  x, l;
    long  count;
    int   add;

    data = (unsigned char*)prf_get_data(name);
    if (data == NULL) return(-1);

    for (x = 0, l = 1, count = 0, add = 0; data[x] != 0; x++) {
        if ((data[x] == 10) || (data[x] == 13)) {
            if ((l == 0) && (count > 0) && (!isspace(ustr[count - 1]))) { ustr[count] = ' '; count++; add = 1; }
            l = 1;
        }
        else if (isspace(data[x]) && (l == 1)) l = 1;
        else { l = 0; ustr[count] = data[x]; count++; add = 0; }
    }
    if (add == 1) count--;
    ustr[count] = 0;
    return (count);
}

// *****************************************************************

int Profile::prf_get_stra(const char *name, char sep, char **strp)
{
    unsigned char* data;
    unsigned char** ustrp = (unsigned char**)strp;
    long  x, s, l;
    long  count;

    data = (unsigned char*)prf_get_data(name);
    if (data == NULL) return(-1);

    for (x = 0, s = 1, l = 1, count = 0; data[x] != 0; x++) {
        if ((data[x] == 10) || (data[x] == 13)) { s = 1; l = 1; data[x] = 0; }
        else if (isspace(data[x]) && (l == 1)) { s = 1; l = 1; data[x] = 0; }
        else if (data[x] == sep) { s = 1; l = 0; data[x] = 0; }
        else if (s == 1) {
            s = 0;
            l = 0;
            *ustrp = &data[x];
            ustrp++;
            count++;
        }
    }
    return (count);
}

// *****************************************************************

int Profile::prf_get_int(const char *name, int *val)
{
    const char* data;
    char* ptr;

    data = prf_get_data(name);
    if (data == NULL) return(-1);
    *val = (int)strtol(data, &ptr, 0);
    return (0);
}

// *****************************************************************

int Profile::prf_get_long(const char *name, long *val)
{
    const char* data;
    char* ptr;

    data = prf_get_data(name);
    if (data == NULL) return(-1);
    *val = strtol(data, &ptr, 0);
    return (0);
}

// *****************************************************************

int Profile::prf_get_double(const char *name, double *val)
{
    const char* data;
    char* ptr;

    data = prf_get_data(name);
    if (data == NULL) return(-1);
    *val = (double)strtod(data, &ptr);
    return (0);
}

// *****************************************************************

int Profile::prf_get_inta(const char *name, char sep, int *intp)
{
    const char* data;
    long  x, s, l;
    long  count;

    data = prf_get_data(name);
    if (data == NULL) return(-1);

    for (x = 0, s = 1, l = 1, count = 0; data[x] != 0; x++) {
        if (data[x] == sep) { s = 1; l = 0; }
        else if ((data[x] == 10) || (data[x] == 13)) { s = 1; l = 1; }
        else if (isspace(data[x]) && (l == 1)) { s = 1; l = 1; }
        else if (s == 1) {
            s = 0;
            l = 0;
            *intp = atoi(&data[x]);
            count++;
            intp++;
        }
    }
    return (count);
}

// *****************************************************************

int Profile::prf_get_array(const char *name, char sep, void *ptr, int type)
{
    const char* data;
    long  x, s, l;
    long  count;
    char* xxx;

    data = prf_get_data(name);
    if (data == NULL) return(-1);

    for (x = 0, s = 1, l = 1, count = 0; data[x] != 0; x++) {
        if (data[x] == sep) { s = 1; l = 0; }
        else if ((data[x] == 10) || (data[x] == 13)) { s = 1; l = 1; }
        else if (isspace(data[x]) && (l == 1)) { s = 1; l = 1; }
        else if (s == 1) {
            s = 0;
            l = 0;
            switch (type) {
            case PRF_DOUBLE: *(double*)ptr = strtod(&data[x], &xxx);
                ptr = ((double*)ptr) + 1;
                break;
            case PRF_FLOAT:  *(float*)ptr = (float)strtod(&data[x], &xxx);
                ptr = ((float*)ptr) + 1;
                break;
            case PRF_INT:    *(int*)ptr = (int)strtol(&data[x], &xxx, 0);
                ptr = ((int*)ptr) + 1;
                break;
            case PRF_LONG:   *(long*)ptr = strtol(&data[x], &xxx, 0);
                ptr = ((long*)ptr) + 1;
                break;
            }

            count++;
        }
    }
    return (count);
}

// *****************************************************************

int Profile::prf_make_stra(const char *name, char sep, char ***ptr)
{
    int n;

    n = prf_get_count(name, sep);

    if (n <= 0) {
        return(n);
    }
    *ptr = (char**)malloc(n * sizeof(char*));
    if (*ptr == NULL) {
        return (-1);
    }
    return(prf_get_stra(name, sep, *ptr));
}



