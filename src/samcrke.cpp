
#ifdef _VISUALC_CE_PH_
#include "stdafx.h"
#endif
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include "profile5.h"
#include "gfp.h"


int TGFP::pretvori_samostojne_crke(void)
{
    int cc, ic, oc;
    //int wd_last_len = 0;

    char *stavek = text_buf_1;
    stavek[MAX_DOLZ_VHODA] = 0;
    char *stavek2 = text_buf_3;

    char ch_prev;
    char ch_curr = ' ';
    char ch_next;

    //char lt_prev = 0;
    //char lt_last = 0;
    //char lt_next = 0;

    //char* wd_prev = NULL;
    //char* wd_last = NULL;

    for (ic = oc = 0; stavek[ic] != 0; ic++)
    {
        ch_prev = ch_curr;
        ch_curr = stavek[ic];
        ch_next = stavek[ic + 1];

        if (ch_curr == ' ')
        {
            //Ce je trenutni znak presledek, ne naredimo nic posebnega.
            stavek2[oc++] = stavek[ic];
            continue;
        }

        {
            //Uredimo podatke o prejsnji, zadnji in naslednji crki in
            //o prejsnji in zadnji besedi.
            //lt_prev = lt_last;
            //lt_last = ch_curr;

            for (cc = ic + 1; stavek[cc] != 0 && stavek[cc] == ' '; cc++);
            //lt_next = stavek[cc];


            if (ch_prev == ' ')
            {
                //wd_prev = wd_last;
                //wd_last = stavek + ic;
                int zz;
                for (zz = ic + 1; stavek[zz] != 0 && (isalpha(stavek[zz]) || isdigit(stavek[zz])); zz++);
                //wd_last_len = zz - ic;
            }
            else {
                //wd_last_len = 0;
            }
        }



        if (ch_prev != ' ' || ch_next != ' ')
        {
            //Ce nimamo osamljene crke, ne naredimo nic posebnega.
            stavek2[oc++] = stavek[ic];
            continue;
        }


        stavek2[oc++] = stavek[ic];
    }

    stavek2[oc] = 0;

    strcpy(stavek, stavek2);

    return 0;
}
