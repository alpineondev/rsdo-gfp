
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
#include "sint1.h"
#include "util_stuff.h"
#include "mystrings.h"

// pravila

int TGFP::pripravi_pravila(class Profile *profile)
{
    int i, j;

    if ((n_prsl = profile->prf_make_stra("pravila_slovar", '\t', &pra_slo)) <= 0) {
        return(-1);
    }

    if ((n_prav = profile->prf_make_stra("pravila", '\t', &pravila)) <= 0) {
        return(-1);
    }


    if ((n_prsl - 4 * (n_prsl / 4)) != 0) {
        //Napacno stevilo pravil (pravila_slovar)
        return(-1);
    }

    if ((n_prav - 4 * (n_prav / 4)) != 0) {
        //Napacno stevilo pravil
        return(-1);
    }

    for (i = 0; i < n_prsl; i++)
        for (j = 0; pra_slo[i][j] != 0; j++) {
            if (pra_slo[i][j] == '=') pra_slo[i][j] = 0;
            if (pra_slo[i][j] == '_') pra_slo[i][j] = ' ';
        }
    for (i = 0; i < n_prav; i++)
        for (j = 0; pravila[i][j] != 0; j++) {
            if (pravila[i][j] == '=') pravila[i][j] = 0;
            if (pravila[i][j] == '_') pravila[i][j] = ' ';
        }
    return(0);
}

// ------------------------------------------------------------


void TGFP::pozabi_pravila(void) {
    if (pra_slo != NULL) { free(pra_slo); pra_slo = NULL; }
    if (pravila != NULL) { free(pravila); pravila = NULL; }
}

//------------------------------------------------------------------------------


char* TGFP::pravilo(char **ptr, char **pravila, int n)
{
    int   i;
    int   j;
    char  cmp[16];
    char* inp;

    for (i = 0; i < n; i += 4) {

        inp = *ptr - strlen(pravila[i]);
        cmp[0] = 0;
        strcat(cmp, pravila[i]);
        strcat(cmp, pravila[i + 1]);
        strcat(cmp, pravila[i + 2]);

        // ----- pregled -----
        for (j = 0; cmp[j] != 0; j++) {
            if (je_gskup(cmp[j])) {
                if (!je_vskup(inp[j], je_gskup(cmp[j]))) break;
            }
            else if (cmp[j] != inp[j]) break;
        }
        if (cmp[j] != 0) continue;

        // ----- OK -----
        *ptr += strlen(pravila[i + 1]);
        return(pravila[i + 3]);
    }
    return(NULL);
}

//------------------------------------------------------------------------------

void TGFP::FilterLocilaInCrke(char* bes) {
    int j = 0;

    for (unsigned int i = 0; i < strlen(bes); i++) {
        if (je_loci(bes[i]) || je_crka(bes[i]))
            bes[j++] = bes[i];
    }
    bes[j] = 0;
}

//------------------------------------------------------------------------------

int TGFP::tekst_v_sampa(bool uporabi_slovarje) {

    char *vh_stavek = text_buf_1;
    char *izh_stavek = text_buf_2;
    int   n = 0;
    int   j;
    int   i;
    int   pos;
    char* str;
    char* slo;
    char* ptr;
    char* bes2;
    char  bes[MAX_DOLZ_BESEDE + 64];
    char  buff[MAX_DOLZ_BESEDE + 64];
    char  besedaFT[MAX_DOLZ_BESEDE + 64];
    char  kontekst[MAX_DOLZ_BESEDE + 64];
    bool  PrejsnjaBesJeIzSlov = false;
    bool  TrenutnaBesJeIzSlov;
    //bool  PrvaBesedaVStavku = true;
    char* KonecPrejBes = NULL;
    char* KonecTrenutBes = NULL;
    int st_besed_iz_slovarja = 0;

    memset(text_buf_2, 0, MAX_DOLZ_VHODA + 64);
    BESEDA beseda;
    short NaglasenZlog;

    izh_stavek[0] = 0;

    bes2 = buff + 3;
    while (*vh_stavek != 0) {
        // ----- ali je beseda (ali vec besed) v slovarju ?
        for (j = 0, slo = NULL, pos = 0; ; ) {
            while ((vh_stavek[j] != ' ') && (vh_stavek[j] != 0)) {
                bes[j] = vh_stavek[j];
                j++;
                if (j == MAX_DOLZ_BESEDE)
                    break;
            }
            bes[j] = 0;
            str = isci_slovar(bes);
            if (!uporabi_slovarje) str = NULL; //ce nikakor nocemo uporabljati slovarjev
            if (str != NULL)
            {
                slo = str; pos = j; st_besed_iz_slovarja += 1;
            }
            else {
                break;
            }

            bes[j] = vh_stavek[j];
            if (vh_stavek[j] != 0)
                j++;
            else
                break;    // ven iz for
        }


        if (slo != NULL) {
            // ----- beseda (ali vec besed) je v slovarju
            vh_stavek += pos;
            TrenutnaBesJeIzSlov = true;

            bes[pos] = 0;


            strcat(izh_stavek, slo);
            KonecTrenutBes = &izh_stavek[strlen(izh_stavek) - 1]; // zadnja "crka" besede
        }
        else {
            // ----- besede ni v slovarju
            TrenutnaBesJeIzSlov = false;
            vh_stavek += j;

            // lahko se zgodi, da beseda vsebuje nedovoljene znake ($, %, /, \, itd)
            // (ti znaki se na vhodu ne filtrirajo, saj bi lahko bili pretvorjeni s
            // pomocjo slovarja...)
            FilterLocilaInCrke(bes);

            // dolocanje naglasnega mesta
            memcpy(beseda.Text, bes, strlen(bes) + 1);
            AnalizaBesede(&beseda);   // razclenimo besedo v zvocnost VZN in VC, jo zlogujemo

            if ((NaglasenZlog = doloci_naglas(bes, bes2)) == 0) {  // pravila
                DolociNaglasIzSplosneStat(&beseda, bes2);
            }
            else
                beseda.NaglaseniZlog = NaglasenZlog;
            // ------------------------------------------------------------

            if (je_crka(bes[0])) n++;

            // ----- naglas je dolocen, pred in za besedo je presl.
            // ----- dodamo se stiri zanke
            j = (int)strlen(bes2);
            for (i = 0; i < 4; i++) bes2[j + i] = vh_stavek[i];
            bes2[j + 4] = 0;
            ptr = bes2 + 1;

            strcpy(besedaFT, "");
            // GFP pravila
            for (i = 0; (*ptr != ' ') && (*ptr != 0); ) {
                str = pravilo(&ptr, pravila, n_prav);
                if (str != NULL) {
                    if (i < MAX_DOLZ_BESEDE) {
                        strcat(besedaFT, str);
                        i += (int)strlen(str);
                    }
                    else {
                        break;
                    }
                }
                else {
                    besedaFT[strlen(besedaFT) + 1] = 0;
                    besedaFT[strlen(besedaFT)] = *ptr;
                    ptr++;
                }
            }



            strcat(izh_stavek, besedaFT);

        }
        // obdelujemo prejsnjo besedo, ker imamo sedaj še njen desni kontekst
        if (PrejsnjaBesJeIzSlov) {
            char* Tmp = KonecPrejBes;
            strcpy(kontekst, " ");
            strcat(kontekst, Tmp);
            strcat(kontekst, " ");
            Tmp = kontekst + 2;
            if ((str = pravilo(&Tmp, pra_slo, n_prsl)) != NULL) {
                if (strlen(str) > 0)
                    *KonecPrejBes = str[0];
            }
        }
        PrejsnjaBesJeIzSlov = TrenutnaBesJeIzSlov;
        KonecPrejBes = KonecTrenutBes;


        strcat(izh_stavek, " ");//@@@@@ dodano
        //PrvaBesedaVStavku = false;

        while (*vh_stavek == ' ') vh_stavek++;

        if (strlen(izh_stavek) > (MAX_DOLZ_STAVKA - MAX_DOLZ_BESEDE - 10)) {
            //pojdimo do prvega presledka
            int izh_len = (int)strlen(izh_stavek) - 1;
            while (izh_len >= 0) {
                if (izh_stavek[izh_len] != ' ') izh_stavek[izh_len--] = 0;
                else break;
            }
            break;
        }
    }


    bes_iz_slovarja = st_besed_iz_slovarja;
    strcpy(text_buf_1, text_buf_2);
    return(n);
}
// ------------------------------------------------------------


//------------------------------------------------------------------------------
// nekatere znake obravnamo kot crke. Npr. znak za odstavek $, ki se lahko  pojavi
// tudi za koncnim locilom.
bool TGFP::je_sampa_crka(char znak) {
    return (je_crka(znak) > 0 || znak == ':' || znak == zlog_break) || znak == '$';
}

//------------------------------------------------------------------------------
void TGFP::ZadnjiIzhodniFilter() {
}
//------------------------------------------------------------------------------

