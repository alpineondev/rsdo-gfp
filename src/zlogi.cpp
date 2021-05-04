#ifdef _VISUALC_CE_PH_
#include "stdafx.h"
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <assert.h>

#ifdef _BORLANDC_
#include <conio.h>
#endif
#include "aparsablestring.h"
#include "profile5.h"
#include "util_stuff.h"
#include "gfp.h"
#include "tstring.h"

#ifdef _VISUALC_CE_
#include "wce_stdlib.h"
#endif

//------------------------------------------------------------------------------

int PrimerjajRAZCL_BESEDA(const void* a, const void*b) {
    return strcmp((char*)((RAZCL_BESEDA*)a)->Text, (char*)((RAZCL_BESEDA*)b)->Text);
}
//---------------------------------------------------------------------------

int PrimerjajRAZCL_BESEDA_EinO(const void* a, const void*b) {
    return strcmp((char*)((RAZCL_BESEDA_EinO*)a)->Text, (char*)((RAZCL_BESEDA_EinO*)b)->Text);
}

//---------------------------------------------------------------------------

int TGFP::pripravi_zlogovanje(class Profile *profile) {
    int sLen;

    istevg = profile->prf_get_str("male_crke", grafemi);
    if (istevg <= 0) {
        return -1;
    }

    sLen = profile->prf_get_str("Razvrstitev_grafemov", srazvg);
    if (sLen < 0) {
        return -1;
    }
    sLen = profile->prf_get_str("samoglasniki", samoglasniki);
    if (sLen < 0) {
        return -1;
    }

    char tmp[2];
    sLen = profile->prf_get_str("zlog_break", tmp);
    if (sLen < 0) {
        return -1;
    }
    zlog_break = tmp[0];

    int polje[MAXSTZLOGOV];
    SizeNajvNaglZlog = profile->prf_get_inta("najverjetnejsi_zlogi", '\t', polje);
    if (SizeNajvNaglZlog < 0) {
        return -1;
    }
    NajverjetnejeNaglasenZlog = (int*)malloc(SizeNajvNaglZlog * sizeof(int));
    for (int n = 0; n < SizeNajvNaglZlog; n++) {
        NajverjetnejeNaglasenZlog[n] = polje[n];
    }

    if ((istevf = profile->prf_get_stra("Nabor_fonemov", '\t', fonemi)) == -1) {
        return -1;
    }
    sLen = profile->prf_get_str("Razvrstitev_fonemov", srazvf);
    if (sLen < 0) {
        return -1;
    }
    else if (sLen != istevf) {
        //Napaka: 'Razvrstitev_fonemov' ni enake dolzine kot Nabor_fonemov
        return -1;
    }

    ZlogBreak.sprintf("%c", zlog_break);

    InitArrayGrafemov();   // ustvari enostavno preslikovalno tabelo Asc(Znak) -> index v arrayu *grafemi
    InitArrayFonemov();    // isto v arrayu *fonemi

    return(0);
}
//---------------------------------------------------------------------------
// le zaradi poenotenega izracuna Indexa
int TGFP::GetIndex(char* Niz, short nBytes) {
    int result;
    try {
        switch (nBytes) {
        case 1:
            if (strlen(Niz) < 1) return 0;
            result = (unsigned short)Niz[0];
            return result;
        case 2:
            if (strlen(Niz) < 2 || Niz[1] == 0) return 0;
            result = ((unsigned short)Niz[1]) * 256 + (unsigned short)Niz[0];
            return result;
        default:
            return 0;
        }
    }
    catch (...) {
        return 0;
    }
}
//---------------------------------------------------------------------------

// predpostavka: oznaka fonema je lahko dvoznakovna
void TGFP::InitArrayFonemov(void) {
    ArrayFonemov = (char*)malloc(ARRAY_FONEMOV_LENGTH);
    memset(ArrayFonemov, 0, ARRAY_FONEMOV_LENGTH);
    short Index;

    for (int n = 0; n < istevf; n++) {
        Index = GetIndex(fonemi[n], (short)strlen(fonemi[n]));
        ArrayFonemov[Index] = n + 1;
    }
}

//---------------------------------------------------------------------------

void  TGFP::pozabi_zlogovanje(void) {
    if (NajverjetnejeNaglasenZlog != NULL) { free(NajverjetnejeNaglasenZlog); NajverjetnejeNaglasenZlog = NULL; }
}

//---------------------------------------------------------------------------

// pretvori zaporedje SAMPA simbolov v zaporedje indexov teh SAMPA simbolov
// cBeseda naj bo c-string
// vrne stevilo SAMPA simbolov
//---------------------------------------------------------------------------
int TGFP::CbesedaToIbeseda(char *cBeseda, int *iBeseda) {
    int Index;
    int i = 0;    // vhod
    int j = 0;    // izhod

    assert(ArrayFonemov != NULL);
    while (cBeseda[i] != 0) {
        Index = GetIndex(&cBeseda[i], 2);
        if ((Index = ArrayFonemov[Index]) != 0) // najprej dva znaka
            i += 2;
        else {
            Index = GetIndex(&cBeseda[i], 1);
            if ((Index = ArrayFonemov[Index]) != 0) // ce ni dvoznakovni, je morda eno
                i++;
        }
        // Index predstavlja (1 based) pozicijo fonema v arrayu fonemi[]
        if (Index == 0)   //  v tabeli nismo nasli nicesar
            i++;    // malo se vseeno premaknem, da ne obstanem na mestu
        else
            iBeseda[j++] = Index - 1; // kot receno, -1 zato ker imamo 1 based

    }
    return j;
}

//---------------------------------------------------------------------------

bool TGFP::DolociZvocnost(int *ibeseda, int stfon, char *zvocnost)
{
    int i, n, v;
    n = 0;
    bool Success = true;

    for (i = 0; i < stfon; ++i)
    {
        v = ibeseda[i];
        switch (srazvf[v])
        {
        case '1':zvocnost[n] = 'v'; break;
        case '2':zvocnost[n] = 'z'; break;
        case '3':
        case '4':zvocnost[n] = 'n'; break;
        default:
        {
            //Napaka: DolociZvocnost() na poziciji i
            zvocnost[n] = 'n';  // privzeto
            Success = false;
        }
        }
        n++;
    }
    zvocnost[n] = 0;

    return Success;
}
//------------------------------------------------------------------------------

bool TGFP::MyStrStr(const char* SearchFor, char* Niz, int j) {
    char* Niz1 = &Niz[j];
    return (strstr(Niz1, SearchFor) != NULL);
}

//------------------------------------------------------------------------------

void TGFP::DeljenjeNaZloge(char *zvocnost, struct ZLOG *zlog, int* StZlogov)
{
    int i, j, d, stsam, meja;
    int zapsam[MAXSTZLOGOV + 1];
    Tstring ErrMsg;
    bool Success = true;

    // presteje samoglasnike => st. zlogov
    stsam = 0;
    for (i = 0; i < (int)strlen(zvocnost); ++i) {
        if (zvocnost[i] == 'v') {
            zapsam[stsam] = i;
            stsam++;
        }
        if (stsam == MAXSTZLOGOV) {
            //prisel do MAX st. zlogov
            continue;
        }
    }

    if (stsam == 0)
    {
        zlog[0].zacetek = 0;
        zlog[0].osnova = 0;
        zlog[0].konec = (int)strlen(zvocnost) - 1;
        meja = 1;
    }
    if (stsam == 1)
    {
        zlog[0].zacetek = 0;
        zlog[0].osnova = zapsam[0];
        zlog[0].konec = (int)strlen(zvocnost) - 1;
        meja = 1;
    }
    if (stsam > 1)
    {
        zlog[0].zacetek = 0;
        zlog[0].osnova = zapsam[0];
        for (i = 1; i < stsam; i++) {
            d = zapsam[i] - zapsam[i - 1];
            j = zapsam[i - 1] + 1;
            meja = 0;
            while (!meja && (j <= zapsam[i]))
            {
                if (d == 1)
                    meja = PostaviMejo(i, zlog, j, zapsam[i]);
                if (d == 2)
                    meja = PostaviMejo(i, zlog, j, zapsam[i]);
                if (d > 2)
                    if (MyStrStr("nnv", zvocnost, j))
                        meja = PostaviMejo(i, zlog, j + 1, zapsam[i]);
                if (d > 2)
                    if (MyStrStr("zzv", zvocnost, j))
                        meja = PostaviMejo(i, zlog, j + 1, zapsam[i]);
                if (d > 2)
                    if (MyStrStr("znv", zvocnost, j))
                    {
                        if (zvocnost[j - 1] == 'v')
                            meja = PostaviMejo(i, zlog, j + 1, zapsam[i]);
                        else
                            meja = PostaviMejo(i, zlog, j, zapsam[i]);
                    };
                if (d > 2)
                    if (MyStrStr("vzn", zvocnost, j - 1))
                        meja = PostaviMejo(i, zlog, j + 1, zapsam[i]);
                if (d > 2)
                    if (MyStrStr("vnz", zvocnost, j - 1))
                    {
                        if (zvocnost[j + 2] == 'v')
                            meja = PostaviMejo(i, zlog, j + 1, zapsam[i]);
                        else
                            meja = PostaviMejo(i, zlog, j, zapsam[i]);
                    };
                if (d > 3)
                    if (MyStrStr("nnzv", zvocnost, j))
                        meja = PostaviMejo(i, zlog, j + 1, zapsam[i]);
                if (d == 4)
                    if (MyStrStr("vzznv", zvocnost, j - 1))
                        meja = PostaviMejo(i, zlog, j + 2, zapsam[i]);
                if (d == 4)
                    if (MyStrStr("vzzzv", zvocnost, j - 1))
                        meja = PostaviMejo(i, zlog, j + 1, zapsam[i]);
                if (d == 5) {
                    if (MyStrStr("vzznnv", zvocnost, j - 1))
                        meja = PostaviMejo(i, zlog, j + 2, zapsam[i]);
                    else if (MyStrStr("vzznzv", zvocnost, j - 1))
                        meja = PostaviMejo(i, zlog, j + 2, zapsam[i]);
                }

                if (d > 4)
                    if (MyStrStr("znnzv", zvocnost, j))
                        meja = PostaviMejo(i, zlog, j + 3, zapsam[i]);

                // ce mi v tem zlogu ni uspelo postaviti meje (nobeno pravilo ni strezalo)
                // postavi mejo nekje vmes
                if (meja == 0) {
                    PostaviMejo(i, zlog, j + (d - 1) / 2, zapsam[i]);
                    meja = 1;
                }
            } // end while
        } // end for
        zlog[stsam - 1].konec = (int)strlen(zvocnost) - 1;
    }   // end if

    if (stsam == 0) stsam = 1;     // zagotovo vsaj en zlog

    if (!Success) {
        // Neuspel poskus postavitve meje. Mejo nastavimo na slepo. Lahko še kaj spremenimo...
    }

    *StZlogov = stsam;   // vrnemo stevilo zlogov
}
//---------------------------------------------------------------------------

int TGFP::PostaviMejo(int i, struct ZLOG *zlog, int j, int osnova)
{
    zlog[i - 1].konec = j - 1;
    zlog[i].zacetek = j;
    zlog[i].osnova = osnova;
    return(1);
}

//---------------------------------------------------------------------------

void TGFP::MoveAccentBehindLJ(char* Niz, long /*size*/, Tstring IsciZnak) {
    Tstring IsciNiz = IsciZnak + ZlogBreak + "j";
    Tstring ZamenjajZ = IsciZnak + "j" + ZlogBreak;
    char* pIsciNiz = (char*)IsciNiz.c_str();
    char* pZamenjajZ = (char*)ZamenjajZ.c_str();
    char* pBuf = Niz;

    while ((pBuf = strstr(pBuf, pIsciNiz)) != NULL) {
        memcpy(pBuf, pZamenjajZ, 3);
    }
}
//---------------------------------------------------------------------------

//prestavi oznake za meje med zlogi in sicer:
//  l"j -> lj"
//  n"j -> nj"
//  v"I -> vI"
//  v"U -> vU"

void TGFP::ZlogovanjePostProcess(char* pBuf) {
    long size = (long)strlen(pBuf);

    MoveAccentBehindLJ(pBuf, size, "l");
    MoveAccentBehindLJ(pBuf, size, "n");
}

//---------------------------------------------------------------------------
// funkcije, ki se ticejo zlogovanja besed v grafemskem zapisu
// pretvori zaporedje znakov v zaporedje indexov v arrayu *grafemi

bool TGFP::DolociZvocnostZaZnakGT(char Znak, int n, int /*size*/, char *zvocnost, char *srazvg) {
    int i;
    unsigned char uZnak = (unsigned char)Znak;
    if ((Uporabi_vsiljeni_presledek && uZnak != Vsiljeni_presledek) || !Uporabi_vsiljeni_presledek)
        i = ArrayGrafemov[uZnak];
    else i = 0;
    bool Success = true;

    switch (srazvg[i]) {
    case '1':
        zvocnost[n] = 'v';
        break;
    case '2':
        zvocnost[n] = 'z';
        break;
    case '3':
    case '4':
        zvocnost[n] = 'n';
        break;
    default:
        zvocnost[n] = 'n';
        Success = false;
    }

    return Success;
}
//---------------------------------------------------------------------------

void TGFP::DolociVC(char *cbeseda, int size, char *BesedaVC) {
    int n;

    for (n = 0; n < size; n++) {
        if (JeVokalGT(cbeseda[n]))
            BesedaVC[n] = 'v';
        else
            BesedaVC[n] = 'c';
    }
    BesedaVC[n] = 0;
}
//---------------------------------------------------------------------------

bool TGFP::DolociZvocnostGT(char *cbeseda, int size, char *zvocnost, char *srazvg) {
    int n;
    bool Success = true;

    for (n = 0; n < size; n++) {
        if (cbeseda[n] == 'r') {
            if (!JeSpredajVokalGT(cbeseda, n) && !JeZadajVokalGT(cbeseda, n, size))
                zvocnost[n] = 'v';
            else
                if (!DolociZvocnostZaZnakGT(cbeseda[n], n, size, zvocnost, srazvg)) {
                    Success = false;
                }
        }
        else {
            if (!DolociZvocnostZaZnakGT(cbeseda[n], n, size, zvocnost, srazvg)) {
                Success = false;
            }
        }
    }
    zvocnost[n] = 0;

    return Success;
}
//---------------------------------------------------------------------------

void TGFP::NajdiZlogeBesedeGT(BESEDA* beseda) {

    DolociZvocnostGT(beseda->Text, (int)strlen(beseda->Text), (char*)(beseda->BesedaVZN), srazvg);
    DeljenjeNaZloge(beseda->BesedaVZN, beseda->Zlogi, &(beseda->StZlogov));
}
//---------------------------------------------------------------------------

bool TGFP::OznaciZlogeGT(char* cBesedilo, Tstring &Result, Tstring &ErrMsg) {
    Tstring Ret;
    if (strchr(cBesedilo, zlog_break) != 0) {
        Result = cBesedilo;
        return true;
    }
    ErrMsg = "";
    bool Success = true;

    AParsableString Y;
    Y.SetText(Tstring(cBesedilo));
    Y.SetDelimiter(" ");
    Y.ElimDubleDelim();

    Tstring cBeseda;
    BESEDA beseda;
    Result = "";
    for (int b = 0; b <= Y.DelimCount(); b++) {
        cBeseda = Y.GetNextSubString();
        memcpy(beseda.Text, cBeseda.c_str(), cBeseda.len() + 1);

        NajdiZlogeBesedeGT(&beseda);

        Ret = "";
        for (int n = 0; n < beseda.StZlogov; n++) {
            for (int i = beseda.Zlogi[n].zacetek; i <= beseda.Zlogi[n].konec; i++) {
                Ret = Ret + beseda.Text[i];
            }
            Ret = Ret + zlog_break;
        }
        Result = Result + Ret.substr(1, Ret.len() - 1) + " ";
    }
    Result = Result.substr(1, Result.len() - 1);   // odrezem zadnji Space

    // se postprocesiranje celega stringa (l"j -> lj",...)
    ZlogovanjePostProcess((char*)Result.c_str());
    return Success;
}
//---------------------------------------------------------------------------

void TGFP::AnalizaBesede(BESEDA *beseda) {
    Tstring ErrMsg;

    beseda->BesedaLength = (int)strlen(beseda->Text);

    DolociZvocnostGT(beseda->Text, beseda->BesedaLength, (char*)(beseda->BesedaVZN), srazvg);
    DolociVC(beseda->Text, beseda->BesedaLength, (char*)(beseda->BesedaVC));

    NajdiZlogeBesedeGT(beseda);

    // dodamo oznake zlogov
    int j = 0;

    for (int n = 0; n < beseda->StZlogov; n++) {
        for (int i = beseda->Zlogi[n].zacetek; i <= beseda->Zlogi[n].konec; i++) {
            beseda->BesedaVZN_Zlogi[j] = beseda->BesedaVZN[i];
            beseda->BesedaVC_Zlogi[j++] = beseda->BesedaVC[i];
        }
        if (n < beseda->StZlogov - 1) {
            beseda->BesedaVZN_Zlogi[j] = zlog_break;
            beseda->BesedaVC_Zlogi[j++] = zlog_break;
        }
    }
    beseda->BesedaVZN_Zlogi[j] = 0;
    beseda->BesedaVC_Zlogi[j] = 0;
}
//---------------------------------------------------------------------------

bool TGFP::JeVokalGT(char Znak) {
    return (strchr(samoglasniki, Znak) != NULL);
}
//---------------------------------------------------------------------------

bool TGFP::JeSpredajVokalGT(char* Niz, int i) {
    if (i > 0)
        return JeVokalGT(Niz[i - 1]);

    return false;
}
//---------------------------------------------------------------------------

bool TGFP::JeZadajVokalGT(char* Niz, int i, int size) {
    if (i == size) {
        return false;
    }
    else {
        return JeVokalGT(Niz[i + 1]);
    }
}

//---------------------------------------------------------------------------
// ustvari enostavno preslikovalno tabelo Asc(Znak) -> index v arrayu *grafemi

void TGFP::InitArrayGrafemov(void) {
    ArrayGrafemov = (char*)malloc(256);
    memset(ArrayGrafemov, 0, 256);

    for (int n = 0; n < istevg; n++) {
        ArrayGrafemov[(unsigned char)grafemi[n]] = n;
    }
}


//---------------------------------------------------------------------------
// vedno odloci o naglasnem mestu
void TGFP::DolociNaglasIzSplosneStat(struct BESEDA *beseda, char* bes2) {
    int naglas;
    int StZlogov = beseda->StZlogov;

    // Èe št. zlogov presega najveèje število zlogov, za katere imamo podatke,
    // uporabimo podatek za najveèje razpoložljivo št. zlogov
    if (StZlogov > SizeNajvNaglZlog)
        naglas = NajverjetnejeNaglasenZlog[SizeNajvNaglZlog - 1];
    else
        naglas = NajverjetnejeNaglasenZlog[StZlogov - 1];   //-1 zato, ker je 1. element je na nicti poziciji

    beseda->NaglaseniZlog = naglas;

    naglasi_besedo(beseda->Text, bes2, naglas);
}

//---------------------------------------------------------------------------

// vrne index fonema v arrayu fonemi[] oz. -1, ce ga ni
int TGFP::GetIndexInFonemi(char* fonem) {
    return ArrayFonemov[GetIndex(fonem, (short)strlen(fonem))] - 1;
}
//---------------------------------------------------------------------------

// vrne pozicijo naglasenega vokala
int TGFP::GetMestoNaglasa(char* cBeseda) {
    int* iBeseda = (int*)malloc(strlen(cBeseda) * sizeof(int));

    int StFonemov = CbesedaToIbeseda(cBeseda, iBeseda);
    for (int i = 0; i < StFonemov; i++) {
        if (fonemi[iBeseda[i]][1] == ':') {
            free(iBeseda); iBeseda = NULL;
            return i;
        }
    }

    free(iBeseda); iBeseda = NULL;

    return -1;
}
//---------------------------------------------------------------------------

char* TGFP::GetFonemAtIndex(int Index) {
    return fonemi[Index];
}
//---------------------------------------------------------------------------

