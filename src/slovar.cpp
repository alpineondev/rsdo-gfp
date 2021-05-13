
#ifdef _VISUALC_CE_PH_
#include "stdafx.h"
#endif
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#ifdef _BORLANDC_
#include <io.h>
#include <dir.h>
#include <conio.h>
#include <dos.h>
#endif
#if defined (_VISUALC_)
#include <io.h>
#elif defined (_VISUALC_CE_)
#include <wce_io.h>
#include <wce_stdlib.h>
#endif
#include <string.h>
#include "gfp.h"
#include "util_stuff.h"
#include "sint1.h"
#include "profile5.h"
#include "ffind.h"
#include "linux.h"

#ifdef LINUX
#define DIR_SEPARATOR "/"
#else
#define DIR_SEPARATOR "\\"
#endif

//----------------------------------------------------------

int compare_slo(void const *a, void const *b)
{
    return strcmp(((PSLOVAR*)a)->Geslo, ((PSLOVAR*)b)->Geslo);
}

//----------------------------------------------------------

int TGFP::JePrioritetenSlovar(char* FileName, char** ImenaPSlovarjev, int n_prior_slov) {
    for (int n = 0; n < n_prior_slov; n++)
        if (m_strcmpi(FileName, ImenaPSlovarjev[n]) == 0)
            return n;

    return -1;
}
//----------------------------------------------------------

int TGFP::pripravi_slovar(class Profile *prf, char *slo_dir)
{
    char    FindStr[MAX_PATHLEN], OpenDat[MAX_PATHLEN]; //niza namenjena iskanju in odpiranju datoteke
    FILE    *fd[MAX_DCT_FILES];                         //polje najdenih datotek slovarjev .dct, .dst
    long    slo_len[MAX_DCT_FILES];                     //dolzine posameznih slovarjev
    int     n_files;                                    //stevilo najdenih slovarjev
    int     file;                                       //stevec v zanki
    long    all_len;                                    //celotna dolzina
    long    n;
    long    x;
    char*   ptr;
    int     fs;
#if defined(LINUX) || defined(_BORLANDC_)
    struct  ffblk ffblk;                            //podatki o datoteki (finfirst,findnext)
#elif defined (_VISUALC_)|| defined(_VISUALC_CE_)
    struct _finddata_t ffblk;
    intptr_t filehandle;
#else
    #unknown os
#endif
    char    slo_prior[MAX_DCT_FILES];               // prioriteta slovarja 0 ce ga ni na prioritetni listi in 1-255 ce pac je
    //bool    cipher_slo[MAX_DCT_FILES];              //je slovar sifriran ali ne

    Tstring file_name = "";

    // info o prioritetah slovarjev je napisan v dveh pomoznih poljih (ImenaPSlovarjev in PrioritetaPSlovarjev)
    // ko se odpirajo datoteke pa se napolni polje slo_prior, ki nosi info o prioriteti za vsak slovar
    int     n_prior_slov;
    char**  ImenaPSlovarjev;
    int*    PrioritetaPSlovarjev;
    if ((n_prior_slov = prf->prf_make_stra("prioriteta_slovarjev", '\t', &ImenaPSlovarjev)) > 0) {
        PrioritetaPSlovarjev = (int*)malloc(n_prior_slov * sizeof(int));
        for (int i = 0; i < n_prior_slov; i++) {
            PrioritetaPSlovarjev[i] = abs(i - n_prior_slov);
        }
    }
    else {
        ImenaPSlovarjev = NULL;
        PrioritetaPSlovarjev = NULL;
    }

    //  odpiranje datotek

    strcpy(FindStr, slo_dir);
    strcat(FindStr, "*.*");

    n_files = 0;
    all_len = 0;
#if defined(LINUX) || defined(_BORLANDC_)
    fs = findfirst(FindStr, &ffblk, 0);
#elif defined (_VISUALC_)
    filehandle = _findfirst(FindStr, &ffblk);
    if (filehandle != -1) fs = 0; else fs = -1;
#elif defined(_VISUALC_CE_)
    filehandle = wceex_findfirst(FindStr, &ffblk);
    if (filehandle != -1) fs = 0; else fs = -1;
#endif
    int k;
    if (fs == -1) {
        //Ni slovarja!
        return(-1);
    }

    while (fs == 0) {

#ifdef _BORLANDC_
        file_name = ffblk.ff_name;
#elif defined(LINUX)
        file_name = ffblk.name;
#else
        file_name = ffblk.name;
#endif

        if (AreExtensionsEqual((char*)file_name.c_str(), DICT_FILE)) {
            //cipher_slo[n_files] = false;
        }
        else {
#if defined(LINUX) || defined(_BORLANDC_)
            fs = findnext(&ffblk);
#elif defined(_VISUALC_)
            fs = _findnext(filehandle, &ffblk);
#elif defined(_VISUALC_CE_)
            fs = wceex_findnext(filehandle, &ffblk);
#endif
            continue;
        }

        strcpy(OpenDat, slo_dir);
        strcat(OpenDat, file_name.c_str());
        fd[n_files] = fopen(OpenDat, "rb");
        if (fd[n_files] == NULL) {
            //poskusimo odpreti naslednjo datoteko
#if defined(LINUX) || defined(_BORLANDC_)
            fs = findnext(&ffblk);
#elif defined(_VISUALC_)
            fs = _findnext(filehandle, &ffblk);
#elif defined(_VISUALC_CE_)
            fs = wceex_findnext(filehandle, &ffblk);
#endif
            continue;
        }

        k = JePrioritetenSlovar((char*)file_name.c_str(), ImenaPSlovarjev, n_prior_slov);
        if (k >= 0)
            slo_prior[n_files] = PrioritetaPSlovarjev[k];
        else
            slo_prior[n_files] = 0;

        slo_len[n_files] = filesize(fd[n_files]);

        all_len += 2;

        all_len += slo_len[n_files];
        n_files++;
        if (n_files > MAX_DCT_FILES) {
            //Prevec slovarskih datotek !
            break;
        }

#if defined(LINUX) || defined(_BORLANDC_)
        fs = findnext(&ffblk);
#elif defined(_VISUALC_)
        fs = _findnext(filehandle, &ffblk);
#elif defined(_VISUALC_CE_)
        fs = wceex_findnext(filehandle, &ffblk);
#endif
    }
#if defined(LINUX)
    findclose(&ffblk);
#elif defined(_VISUALC_)
    _findclose(filehandle);
#elif defined(_VISUALC_CE_)
    wceex_findclose(filehandle);
#endif

    if (n_files == 0) {
        //Ni slovarja!
        return(-1);
    }


    if (ImenaPSlovarjev != NULL) { free(ImenaPSlovarjev); ImenaPSlovarjev = NULL; }
    if (PrioritetaPSlovarjev != NULL) { free(PrioritetaPSlovarjev); PrioritetaPSlovarjev = NULL; }

    // ----- alokacija pomnilnika za slovar

    slovar = (char*)malloc(all_len + 1);
    if (slovar == NULL) {
        for (x = 0; x < n_files; x++) fclose(fd[x]);
        return(-1);
    }

    // ----- branje datotek v buffer
    long* DCTPosition = (long*)malloc((n_files + 1) * sizeof(long));
    if (DCTPosition == NULL) { //ce ne moremo alocirati zadosti pomnilnika, sploh ne uporabljamo solvarjev
        free(slovar);
        slovar = NULL;
        for (x = 0; x < n_files; x++) fclose(fd[x]);
        return -1;
    }

    ptr = slovar;
    for (file = 0; file < n_files; file++) {
        DCTPosition[file] = (long)(ptr - slovar);          // zacetek datoteke v skupnem buferju
        x = Bread(ptr, slo_len[file], fd[file]);


        if (slo_len[file] != x) {
            free(slovar);   //sprosti se do sedaj zasezeni pomnilnik
            slovar = NULL;
            free(DCTPosition);
            DCTPosition = NULL;
            for (x = 0; x < n_files; x++) fclose(fd[x]);
            return(-1);
        }
        ptr += x;

        // dodaj clrf na konec posameznega slovarja, ce je ta manjka
        // ali ima slovar na koncu clrf ali ne se preveri sele tukaj

        char *testing_clr = NULL;
        int off = (int)(ptr - slovar);
        if (off > 2) testing_clr = ptr - 2;
        if (testing_clr[0] != '\r' || testing_clr[1] != '\n') {
            *ptr = '\r'; ptr++;
            *ptr = '\n'; ptr++;
        }
    }
    *ptr = 0;
    DCTPosition[file] = (long)(ptr - slovar);
    all_len = (int)(ptr - slovar);


    for (x = 0; x < n_files; x++) fclose(fd[x]);

    // ----- stetje stevila besed v slovarju

    ptr = slovar;
    for (n = 0, bes_v_slo = 0; n < all_len; n++, ptr++) {
        if (*ptr == 13) bes_v_slo++;
    }

    // ----- alokacija pomnilnika za kazalce

    pslovar = (PSLOVAR*)malloc(bes_v_slo * sizeof(PSLOVAR));
    if (pslovar == NULL) {
        free(slovar);   //sprosti se do sedaj zasezeni pomnilnik
        slovar = NULL;    //ne uporabljamo slovarjev
        free(DCTPosition);
        DCTPosition = NULL;
        for (x = 0; x < n_files; x++) fclose(fd[x]);
        return(-1);
    }

    // ----- postavitev kazalcev na besede

    long i = 0;
    int CurrentDCT = 0;     // 0..(n_files-1)
    pslovar[i].Geslo = slovar;
    pslovar[i].Prioriteta = slo_prior[CurrentDCT];
    i++;
    for (n = 0; n < all_len - 2; n++) {
        if (n + 2 >= DCTPosition[CurrentDCT + 1]) CurrentDCT++;

        if (slovar[n] == '\r') {
            if (slovar[n + 2] != 0) {
                pslovar[i].Geslo = &slovar[n + 2];                //  beseda po CRLF
                pslovar[i].Prioriteta = slo_prior[CurrentDCT];
                i++;
            }
            slovar[n] = 0;
            slovar[n + 1] = 0;
        }
        else if (slovar[n] == '\t')
            slovar[n] = 0; // zato, da bo strcmp v searchu delal prav (da bo primerjal le geslo in ne se izgovorjavo)
    }
    slovar[all_len - 1] = 0;
    slovar[all_len - 2] = 0;
    free(DCTPosition); DCTPosition = NULL;


    // ----- sortiranje slovarja

    if (pslovar != NULL)
        qsort(pslovar, bes_v_slo, sizeof(PSLOVAR), &compare_slo);

    return 0;
}


void TGFP::pozabi_slovar(void) {
    if (pslovar != NULL) { free(pslovar); pslovar = NULL; }
    if (slovar != NULL) { free(slovar); slovar = NULL; }
}


char* TGFP::isci_slovar(char* bes)
{
    PSLOVAR Key;
    Key.Geslo = bes;

#ifndef _VISUALC_CE_
    PSLOVAR* ptr = (PSLOVAR*)bsearch(&Key, pslovar, bes_v_slo, sizeof(PSLOVAR), &compare_slo);
#else
    PSLOVAR* ptr = (PSLOVAR*)wceex_bsearch(&Key, pslovar, bes_v_slo, sizeof(PSLOVAR), &compare_slo);
#endif
    if (ptr == NULL) return (NULL);

    // preveri geslo glede na prioriteto slovarja

    char Prioriteta = 0;
    char*  cptr = ptr->Geslo;
    while (ptr > pslovar) {
        ptr--;
        if (strcmp(ptr->Geslo, bes) != 0) {
            ptr++;
            break;
        }
    }

    while (strcmp(ptr->Geslo, bes) == 0) {
        if (ptr->Prioriteta > Prioriteta) {
            Prioriteta = ptr->Prioriteta;
            cptr = ptr->Geslo;
        }
        if (ptr < &pslovar[bes_v_slo - 1])
            ptr++;
        else
            break;
    }

    while (*cptr != 0) cptr++;
    cptr++;
    return(cptr);
}
