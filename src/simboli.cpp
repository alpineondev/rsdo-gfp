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
#include "gfp.h"
#include "util_stuff.h"


// *****************************************************************

int TGFP::pripravi_simbole(class Profile *profile)
{
    if (beri_sim("cifre", CIFRA, profile) != 0) return(-1);
    if (beri_sim("male_crke", MCRKA, profile) != 0) return(-1);
    if (beri_sim("velike_crke", VCRKA, profile) != 0) return(-1);
    if (beri_sim("koncna_locila", KLOCILO, profile) != 0) return(-1);
    if (beri_sim("nekoncna_locila", NLOCILO, profile) != 0) return(-1);
    if (beri_sim("dodatna_locila", DLOCILO, profile) != 0) return(-1);
    if (beri_sim("posebni_znaki", POSZNAK, "$%&/()+=<>*-") != 0) return(-1);
    if (beri_sim("dodatni_znaki", DODZNAK, "\"\\'|") != 0) return(-1);
    if (beri_sim("samoglasniki", SAMOGL, profile) != 0) return(-1);
    if (beri_sim("soglasniki", SOGLAS, profile) != 0) return(-1);
    if (beri_sim("sogl_pr", SOGLAS_PR, profile) != 0) return(-1);
    if (beri_sim("zvocniki", ZVOCNIKI, profile) != 0) return(-1);
    if (beri_sim("nezv_sogl", NEZV_SOGL, profile) != 0) return(-1);
    if (beri_sim("zven_sogl", ZVEN_SOGL, profile) != 0) return(-1);
    if (beri_sim("zven_nezvoc", ZVEN_NEZV, profile) != 0) return(-1);
    if (beri_sim("glasovne_skupine", GLAS_SK, profile) != 0) return(-1);
    if (beri_sim("sampa_locila", SAMPA_LOC, profile) != 0) return(-1);

    char buf[10];
    int  buf_full;

    if (profile->IsOk()) {
        buf_full = profile->prf_get_str("Vsiljeni_presledek", buf);
        if (buf_full > 0) {
            Vsiljeni_presledek = buf[0];
            Uporabi_vsiljeni_presledek = true;
        }
        else Uporabi_vsiljeni_presledek = false;
    }

    return(0);
}

// -------------------------------------------------------------------------

int TGFP::beri_sim(const char *ime, int index, class Profile *profile)
{
    int  i;
    int  n;
    unsigned char str[MAX_SIM];

    for (i = 0; i < MAX_SIM; i++) simbol_tab[i][index] = 0;

    n = profile->prf_get_strl(ime);
    if ((n > MAX_SIM) || (n <= 0)) {
        return (-1);
    }
    profile->prf_get_str(ime, (char*)str);

    if (strcmp(ime, "velike_crke") == 0) strcpy((char*)vcrke, (char *)str);
    if (strcmp(ime, "male_crke") == 0) strcpy((char*)mcrke, (char *)str);


    for (i = 0; i < n; i++)
        if (str[i] > 0) simbol_tab[str[i]][index] = i + 1;
        else {
            return (-1);
        }
    return 0;
}

// -------------------------------------------------------------------------

int TGFP::beri_sim(const char *ime, int index, const char *data)
{
    int  i;
    int  n;
    unsigned char str[MAX_SIM];

    for (i = 0; i < MAX_SIM; i++) simbol_tab[i][index] = 0;

    n = (int)strlen(data);
    if ((n > MAX_SIM) || (n <= 0)) {
        return (-1);
    }
    strcpy((char*)str, data);

    if (strcmp(ime, "velike_crke") == 0) strcpy((char*)vcrke, (char *)str);
    if (strcmp(ime, "male_crke") == 0) strcpy((char*)mcrke, (char *)str);


    for (i = 0; i < n; i++)
        if (str[i] > 0) simbol_tab[str[i]][index] = i + 1;
        else {
            return (-1);
        }
    return 0;
}

// -------------------------------------------------------------------------

int TGFP::je_mcrka(unsigned char x) {
    return(simbol_tab[x][MCRKA]);
}

int TGFP::je_vcrka(unsigned char x) {
    return(simbol_tab[x][VCRKA]);
}

int TGFP::je_cifra(unsigned char x) {
    return(simbol_tab[x][CIFRA]);
}

int TGFP::je_kloci(unsigned char x) {
    return(simbol_tab[x][KLOCILO]);
}

int TGFP::je_nloci(unsigned char x) {
    return(simbol_tab[x][NLOCILO]);
}

int TGFP::je_dloci(unsigned char x) {
    return(simbol_tab[x][DLOCILO]);
}

int TGFP::je_pznak(unsigned char x) {
    return(simbol_tab[x][POSZNAK]);
}

int TGFP::je_dznak(unsigned char x) {
    return(simbol_tab[x][DODZNAK]);
}

int TGFP::je_samog(unsigned char x) {
    return(simbol_tab[x][SAMOGL]);
}

int TGFP::je_sogl(unsigned char x) {
    return(simbol_tab[x][SOGLAS]);
}

int TGFP::je_soglp(unsigned char x) {
    return(simbol_tab[x][SOGLAS_PR]);
}

int TGFP::je_zvoc(unsigned char x) {
    return(simbol_tab[x][ZVOCNIKI]);
}

int TGFP::je_nsogl(unsigned char x) {
    return(simbol_tab[x][NEZV_SOGL]);
}

int TGFP::je_zsogl(unsigned char x) {
    return(simbol_tab[x][ZVEN_SOGL]);
}

int TGFP::je_zvnzv(unsigned char x) {
    return(simbol_tab[x][ZVEN_NEZV]);
}

int TGFP::je_gskup(unsigned char x) {
    return(simbol_tab[x][GLAS_SK]);
}

int TGFP::je_nagl(unsigned char x) {
    return(x == '/');
}

int TGFP::je_presl(unsigned char x) {
    return(x == ' ');
}

int TGFP::je_nic(unsigned char x) {
    return(x == '=');
}

int TGFP::je_crka(unsigned char x) {
    return(simbol_tab[x][VCRKA] | simbol_tab[x][MCRKA]);
}
int TGFP::je_loci(unsigned char x) {
    return(simbol_tab[x][KLOCILO] | simbol_tab[x][NLOCILO] | simbol_tab[x][DLOCILO]);
}

int TGFP::je_vhsim(unsigned char x) {
    return(simbol_tab[x][KLOCILO] | simbol_tab[x][NLOCILO] | simbol_tab[x][VCRKA] | simbol_tab[x][MCRKA] | simbol_tab[x][CIFRA] | simbol_tab[x][POSZNAK]);
}

int TGFP::je_vskup(unsigned char x, int sk) {
    return(simbol_tab[x][sk]);
}

int TGFP::je_sampa_loci(unsigned char x) {
    return(simbol_tab[x][SAMPA_LOC] && x != '$');
}

int TGFP::mcrka(unsigned char x) {
    if (je_vcrka(x))
        return(mcrke[simbol_tab[x][VCRKA] - 1]);
    else return x;
}
int TGFP::vcrka(unsigned char x) {
    if (je_mcrka(x))
        return(vcrke[simbol_tab[x][MCRKA] - 1]);
    else return x;
}

// -------------------------------------------------------------------------
