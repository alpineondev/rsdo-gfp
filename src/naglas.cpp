
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
#include <string.h>
#include "profile5.h"
#include "util_stuff.h"
#include "gfp.h"

// ------------------------------------------------------------

int TGFP::pripravi_naglase_main(Profile *profile)
{
    if ((n_prvi = profile->prf_make_stra("prvi_zlog", '\t', &prvi_zlog)) < 0) return(-1);
    if ((n_pred = profile->prf_make_stra("predpone", '\t', &predpone)) <= 0) return(-1);
    if ((n_kppz = profile->prf_make_stra("koncppz", '\t', &koncppz)) < 0) return(-1);
    if ((n_kpz = profile->prf_make_stra("koncpz", '\t', &koncpz)) < 0) return(-1);
    if ((n_kzad = profile->prf_make_stra("konczad", '\t', &konczad)) < 0) return(-1);
    return(0);
}

// ------------------------------------------------------------

int TGFP::pripravi_naglase(Profile *profile)
{
    if (pripravi_naglase_main(profile) != 0) {
        return(-1);
    }
    else return 0;
}

// ------------------------------------------------------------

void TGFP::pozabi_naglase(void) {
    if (prvi_zlog != NULL) { free(prvi_zlog); prvi_zlog = NULL; }
    if (predpone != NULL) { free(predpone); predpone = NULL; }
    if (koncppz != NULL) { free(koncppz); koncppz = NULL; }
    if (konczad != NULL) { free(konczad); konczad = NULL; }
    if (koncpz != NULL) { free(koncpz); koncpz = NULL; }
}

// ------------------------------------------------------------

int TGFP::je_koncnica(char *bes, char **ptr, int n)
{
    int i;
    int index;

    for (i = 0; i < n; i++) {
        index = (int)(strlen(bes) - strlen(ptr[i]));
        if (index < 0) continue;
        if (strcmp(&bes[index], ptr[i]) == 0) return(1);
    }
    return(0);
}

// ------------------------------------------------------------

int TGFP::je_zacetek(char *bes, char **ptr, int n)
{
    int i;

    for (i = 0; i < n; i++)
        if (strncmp(bes, ptr[i], strlen(ptr[i])) == 0)
            return(1);

    return(0);
}

// ------------------------------------------------------------

int TGFP::JeSpredajSamogl(char* bes, int position) {
    if (position > 0)
        return je_samog(*(bes - 1));

    return (0);
}

//----------------------------------------------------------------

int TGFP::JeNosilecZloga(char* bes, int position) {
    if (je_samog(*bes))
        return (1);
    else if ((*bes == 'r')) {  // r je nosilec zloga le, ce nima za soseda samoglasnik
        if (!je_samog(*(bes + 1)) && !JeSpredajSamogl(bes, position))
            return (1);
    }
    return (0);
}

//----------------------------------------------------------------

int TGFP::stev_samog(char *bes)
{
    int i = 0, n = 0;

    for (; (*bes != ' ') && (*bes != 0); bes++, n++) {
        if (JeNosilecZloga(bes, n)) i++;
    }

    return (i);
}

// ------------------------------------------------------------

int TGFP::naglasi_besedo(char *bes, char *izh, int naglas)
{
    int i = 0, n = 0;

    izh[0] = ' ';
    izh++;
    for (; (*bes != ' ') && (*bes != 0); izh++, bes++, n++) {
        if (JeNosilecZloga(bes, n)) i++;

        if (i == naglas) {
            *izh = '/';
            izh++;
            i++;
        }
        *izh = *bes;
    }
    *izh = ' ';
    izh++;
    *izh = 0;

    return (0);
}

// ------------------------------------------------------------

int TGFP::doloci_naglas(char *bes, char *izh)
{
    int  st_zlog;
    int  naglas;

    st_zlog = stev_samog(bes);
    if (je_zacetek(bes, prvi_zlog, n_prvi) || (st_zlog <= 1))
        naglas = 1;
    else if (je_koncnica(bes, konczad, n_kzad))
        naglas = st_zlog;
    else if (je_koncnica(bes, koncpz, n_kpz) && (st_zlog >= 2)) {
        if ((st_zlog == 2) && je_zacetek(bes, predpone, n_pred))
            naglas = st_zlog;
        else
            naglas = st_zlog - 1;
    }
    else if (je_koncnica(bes, koncppz, n_kppz) && (st_zlog >= 3)) {
        if ((st_zlog == 3) && je_zacetek(bes, predpone, n_pred))
            naglas = st_zlog - 1;
        else
            naglas = st_zlog - 2;
    }
    else
        naglas = 0;

    if (naglas > 0)
        naglasi_besedo(bes, izh, naglas);


    return naglas;
}

// ------------------------------------------------------------



