
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
#include "gfp.h"
#include "sint1.h"

// *****************************************************************

int TGFP::pretvori_vel_crke(void)
{
    char *stavek = text_buf_1;
    int  i;
    int  in;
    int  out;
    int  nr;
    char ch;
    char pr;
    char *stavek2 = text_buf_3;
    char bes[MAX_DOLZ_BESEDE + 64];
    bool koncno_locilo_levo;
    bool koncno_locilo_desno;


    for (in = 0, out = 0; stavek[in] != 0; ) {

        ch = stavek[in];             // trenutno obdelovani znak
        if (out > 0)
            pr = stavek2[out - 1];   // prejsnji znak
        else
            pr = ' ';

        // ----- ce je mala<-velika/mala ---> mala
        if (je_vcrka(ch) && je_mcrka(pr)) {
            stavek2[out] = stavek[in] = mcrka(ch);
            out++; in++;
            continue;
        }

        // ----- ce je presledek<-velika->mala  ---> mala
        if (je_vcrka(ch) && (pr == ' ') && je_mcrka(stavek[in + 1])) {
            stavek2[out] = stavek[in] = mcrka(ch);
            out++; in++;
            continue;
        }

        if (je_vcrka(ch) && (pr == ' ')) {

            // ----- ali je beseda v slovarju ???
            for (i = 0; je_vcrka(stavek[in + i]) && i < MAX_DOLZ_BESEDE; i++) bes[i] = stavek[in + i];
            if (i == MAX_DOLZ_BESEDE) {
                bes[i] = 0;
            }
            bes[i] = 0;
            if (strlen(bes) == 1) { //ce je beseda enocrkovni predlog, je niti ne poskusaj najti v slovarju
            }
            else {
                if (isci_slovar(bes) != NULL) {
                    // beseda je v slovarju ---> velike crke
                    for (; je_vcrka(stavek[in]); in++, out++) {
                        stavek2[out] = stavek[in];
                    }
                    continue;
                }
            }

            int left_lenght;
            int right_lenght;
            bool in_left;

            // ----- ali je levo oz. desno beseda z velikimi crkami?
            // dolzina besede napisane z velikimi crkami na levi

            if (in == 0) koncno_locilo_levo = true; else koncno_locilo_levo = false;
            for (i = 1, left_lenght = 0, in_left = false; ((in - i) >= 0) && !je_mcrka(stavek[in - i]); i++) {
                if (left_lenght == 0 && je_vcrka(stavek[in - i])) {
                    if (!in_left) in_left = true;
                    left_lenght++;
                }
                else if (in_left && je_vcrka(stavek[in - i])) {
                    left_lenght++;
                }
                else {
                    if (je_vcrka(stavek[in - i]) && left_lenght > 0) {
                        break;
                    }
                    if (in_left) in_left = false;
                    if (je_kloci(stavek[in - i]) && left_lenght == 0) {
                        koncno_locilo_levo = true;
                        break;
                    }
                }
            }

            for (i = 1, nr = 0, right_lenght = 0, koncno_locilo_desno = false; (stavek[in + i] != 0) && !je_mcrka(stavek[in + i]); i++) {
                if ((stavek[in + i] == ' ') && je_crka(stavek[in + i - 1])) nr++;
                if ((stavek[in + i] == ' ') && je_kloci(stavek[in + i - 1])) koncno_locilo_desno = true;
                if (nr > 0 && je_vcrka(stavek[in + i])) right_lenght++;
                if (nr > 1) break;
            }
            if (stavek[in + 1] == 0) nr = -2;
            if (koncno_locilo_desno) right_lenght = 0;


            if (((right_lenght > 1) || (left_lenght > 1))) {
                // besede ni v slov. ---> male crke
                for (; je_crka(stavek[in]); in++, out++) stavek2[out] = mcrka(stavek[in]);
                continue;
            }
            else {
                // ----- ni naslov
                for (i = 0; je_vcrka(stavek[in + i]); i++);
                if (i > 3) {
                    // ----- daljsa od 3 znakov
                    for (; je_crka(stavek[in]); in++, out++) stavek2[out] = mcrka(stavek[in]);
                    if (stavek[in] != ' ') {
                        stavek2[out] = ' ';
                        out++;
                    }
                    continue;
                }
                else {
                    // ----- krajsa ali dolga 3 znake
                    //Ce je beseda eneocrkovni predlog in je napisana z veliko in je pred njo koncno locilo, jo pretvori v malo crko.
                    // ----- ali je levo oz. desno beseda z velikimi crkami?
                    //nastavi vsiljene presledke pred in za crkovano besedico
                    if (!koncno_locilo_levo || strlen(bes) != 1) {
                        stavek2[out++] = ' ';
                        if (Uporabi_vsiljeni_presledek) {
                            stavek2[out++] = Vsiljeni_presledek;
                            stavek2[out++] = ' ';
                        }
                    }
                    for (; je_vcrka(stavek[in]); in++, out++) {
                        if (!koncno_locilo_levo || strlen(bes) != 1) {
                            stavek2[out] = stavek[in];
                            out++;
                            if (Uporabi_vsiljeni_presledek) {
                                stavek2[out] = ' ';
                                out++;
                                stavek2[out] = Vsiljeni_presledek;
                                out++;
                            }
                        }
                        else {
                            stavek2[out] = mcrka(stavek[in]);
                            out++;
                        }
                        stavek2[out] = ' ';
                    }
                    if (stavek[in] == ' ') in++;
                    continue;
                }

            }
        }
        stavek2[out] = stavek[in];
        out++; in++;
        if (out > (MAX_DOLZ_STAVKA - MAX_DOLZ_BESEDE - 10)) {
            break;
        }
    }
    stavek2[out] = 0;
    strcpy(stavek, stavek2);
    return(0);
}


