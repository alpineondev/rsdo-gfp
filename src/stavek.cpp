
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
#include "util_stuff.h"

// *****************************************************************
// iz vhodnega besedila tvori povedi
// ugotovi, katere pike so locila in katre ne (okrajsave, stevniki,...)...
// Vsako poved zakljuci z ustreznim koncnim locilom.
// Tudi za zadnjim koncnim locilom je SPACE.

int TGFP::TvoriPovedi(void)
{

    char *vhod_buf = text_buf_1;
    char *izhod = text_buf_2;
    int MaxIzhod = MAX_DOLZ_STAVKA;
    int dolzina_vhod = (int)strlen(vhod_buf);
    int ptr_vhod = 0;
    *izhod = 0;

    int   ch;
    int   len;
    int   out;
    int   pika;
    int   kli_vpr;
    char  *buff = text_buf_3;
    char  *buff2 = text_buf_4;
    char* stavek;
    char* stavek2 = buff2;

    do {
        stavek = buff;
        buff[0] = buff[1] = buff[2] = buff[3] = buff[4] = 0;
        len = 4;
        pika = 0;
        kli_vpr = 0;

        // postavimo se na 1. crko (spustimo zacetna locila in presledke)
        for (ch = ' '; (ch != EOF) && (isspace(ch) || je_loci(ch)); ) {
            if (ptr_vhod < dolzina_vhod)
                ch = vhod_buf[ptr_vhod++];
            else
                ch = EOF;
        }

        while (1) {
            if (ch == EOF) {
                if (kli_vpr || pika) {
                    if ((pika > kli_vpr) && (kli_vpr > 0)) pika = kli_vpr;
                    if (!pika) pika = kli_vpr;
                    stavek[pika + 1] = 0;
                    break;
                }
                if (len > 4) {
                    stavek[len] = '.';
                    stavek[len + 1] = 0;
                    break;
                }
                else break;
            }
            if (ch == '\n') {
                if (stavek[len - 1] == ' ') len--;
                stavek[len] = '\n';
                stavek[len + 1] = 0;

                if (stavek[len - 1] == '\n') {
                    if (kli_vpr || pika) {
                        if ((pika > kli_vpr) && (kli_vpr > 0)) pika = kli_vpr;
                        if (!pika) pika = kli_vpr;
                        stavek[pika + 1] = '}';
                        stavek[pika + 2] = 0;
                        break;
                    }
                    stavek[len - 1] = '.';
                    stavek[len] = '}';
                    stavek[len + 2] = 0;
                    break;
                }
                len++;
                ch = 0;
            }

            if (isspace(ch)) {
                if (!isspace(stavek[len - 1])) {
                    stavek[len] = ' ';
                    stavek[len + 1] = 0;
                    len++;
                }
                ch = 0;
            }

            if (je_vhsim(ch)) {
                stavek[len] = ch;
                stavek[len + 1] = 0;

                if (kli_vpr > 0) {
                    stavek[kli_vpr + 1] = 0;
                    ptr_vhod--;
                    break;
                }
                if ((ch == '!') || (ch == '?')) kli_vpr = len;

                if (ch == '.') {
                    if (!pika) {
                        pika = len;
                    }
                    else if (strcmp(&stavek[len - 2], "...") == 0) {
                        stavek[len - 2] = '.';
                        stavek[len - 1] = ']';
                        stavek[len] = 0;
                        break;
                    }
                }

                if ((ch != '.') && (pika > 0)) {
                    if ((je_cifra(stavek[pika - 1]) && je_cifra(ch)) || (je_cifra(stavek[pika - 1]) && je_mcrka(ch))) pika = 0;
                    else {
                        stavek[pika + 1] = 0;
                        ptr_vhod--;
                        break;
                    }
                }
                len++;
            }

            if (ptr_vhod < dolzina_vhod)
                ch = vhod_buf[ptr_vhod++];
            else
                ch = EOF;

            if (len >= (MAX_DOLZ_STAVKA / 2))
                if (isspace(ch)) {
                    stavek[len] = '.';
                    stavek[len + 1] = 0;
                    // *** Stavek je predolg. Razdeljen bo na 2 stavka.
                    break;
                }
        }

        for (len = 4, out = 0; stavek[len] != 0; len++) {
            if ((stavek[len] == '-') && (stavek[len + 1] == '\n')) {
                len++;
                continue;
            }
            if ((stavek[len] == '-') && (je_cifra(stavek[len + 1]))) {
                stavek[len] = '\\';
            }
            if ((stavek[len] == ',') && (je_cifra(stavek[len - 1])) && (je_cifra(stavek[len + 1]))) {
                stavek[len] = '\'';
            }
            if (stavek[len] == '.') {
                if ((je_cifra(stavek[len - 1])) && (isspace(stavek[len + 1])))
                    stavek[len] = '|';
                else if ((stavek[len + 1] != 0) && (stavek[len + 1] != ']') && (stavek[len + 1] != '}'))
                    stavek[len] = '\"';
            }

            if (stavek[len] == '\n') {
                stavek[len] = ' ';
            }
            if (isspace(stavek[len]) && isspace(stavek[len - 1])) {
                continue;
            }
            if ((je_pznak(stavek[len]) || je_loci(stavek[len]) || je_dznak(stavek[len]))) {
                if (out) if (stavek2[out - 1] != ' ') {
                    if (out < MaxIzhod) {
                        stavek2[out] = ' ';
                        out++;
                    }
                }
                if (out < MaxIzhod) { stavek2[out] = stavek[len]; out++; }
                if (out < MaxIzhod) { stavek2[out] = ' ';         out++; }
                continue;
            }
            if (out) if ((je_cifra(stavek[len]) && je_crka(stavek2[out - 1])) ||
                (je_crka(stavek[len]) && je_cifra(stavek2[out - 1]))) {
                if (out < MaxIzhod) { stavek2[out] = ' ';         out++; }
                if (out < MaxIzhod) { stavek2[out] = stavek[len]; out++; }
                continue;
            }
            if (out) if ((stavek[len] == ' ') && (stavek2[out - 1] == ' ')) continue;
            if (out < MaxIzhod) {
                stavek2[out] = stavek[len];
                out++;
            }
        }

        if (out) if (out < MaxIzhod && stavek2[out - 1] != ' ') {
            stavek2[out] = ' ';
            out++;
        }
        stavek2[out] = 0;

        strncat(izhod, stavek2, MaxIzhod - strlen(izhod));

    } while (out > 0);

    strcpy(text_buf_1, text_buf_2);
    return (out);
}

