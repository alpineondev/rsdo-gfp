//---------------------------------------------------------------------------
#ifdef _VISUALC_CE_PH_
#include "stdafx.h"
#endif
#include "gfp.h"
#include "util_stuff.h"
#include "paths.h"
#include "tstring.h"
#include <ctype.h>
#include "linux.h"
//---------------------------------------------------------------------------

// konvertira vse czs-je v cp ASCII 7 in sfiltrira znake < 32 in >126
void TGFP::PrviVhodniFilter(void) {
    unsigned char *vhod = (unsigned char*)text_buf_1;
    char *izhod = text_buf_2;
    int dolzina = MAX_DOLZ_VHODA;
    int Znak;
    int j = 0;

    unsigned int DolzinaVhoda = (unsigned int)strlen(text_buf_1);
    for (unsigned int i = 0; i < DolzinaVhoda; i++) {

        if ((vhod[i] >= 65 && vhod[i] <= 90) ||
            (vhod[i] >= 97 && vhod[i] <= 122) ||
            strchr(".,;:!?-\r\n\t ", vhod[i]) != NULL ||
            IsInCPc(vhod[i], CPlatin2))
        {
            Znak = ConvertCPc(vhod[i], CPlatin2);
        }
        else continue;

        if (Znak == 127) {
            izhod[j++] = 68;   // D
            izhod[j++] = 96;   // ž
        }
        else if (Znak == 125) {
            izhod[j++] = 100;  // d
            izhod[j++] = 96;   // ž
        }
        else if ((Znak >= 32 && Znak <= 126) || Znak == '\n' || Znak == '\r' || Znak == 0xEC)
            izhod[j++] = Znak;

        if (j + 3 > dolzina) break;
    }
    izhod[j] = 0;
    strcpy(text_buf_1, text_buf_2);
}
//------------------------------------------------------------------------------
int TGFP::init_gfp_profile(class Profile *prf) {
    if (!prf->IsOk()) {
        //Napaka: init_gfp_profile
        return(-1);
    }

    if (pripravi_simbole(prf) != 0) return -1;

    if (pripravi_naglase(prf) != 0) return -1;
    if (pripravi_pravila(prf) != 0) return -1;

    if (pripravi_zlogovanje(prf) != 0) return -1;

    return 0;
}

//------------------------------------------------------------------------------
int TGFP::init_gfp_dictionary(class Profile *prf) {
    if (!prf->IsOk()) {
        //Napaka: init_gfp_dictionary
        return(-1);
    }
    if (pripravi_slovar(prf, Dct_directory) != 0) {
        //Sintetizator zagnan brez slovarjev!
    }


    return 0;
}
//------------------------------------------------------------------------------

TGFP::TGFP(char *NameOfIniFile) {
    GFP_res = NULL;
    //pozabi_slovarje
    slovar = NULL;
    pslovar = NULL;

    //pozabi_naglase
    prvi_zlog = NULL;
    predpone = NULL;
    koncppz = NULL;
    koncpz = NULL;
    konczad = NULL;
    //pozabi_pravila
    pra_slo = NULL;
    pravila = NULL;

    //pozabi_zlogovanje
    ArrayGrafemov = NULL;
    ArrayFonemov = NULL;
    NajverjetnejeNaglasenZlog = NULL;



    if (!ReinitializeAll(NameOfIniFile)) {
        OK = false;
        throw - 1;
    }
}

//------------------------------------------------------------------------------

bool TGFP::ReinitializeAll(char *NameOfIniFile) {
    OK = false;
    if (GFP_res != NULL) delete GFP_res;
    pozabi_slovar();

    pozabi_naglase();
    pozabi_pravila();

    pozabi_zlogovanje();



    GFP_res = new Profile(NameOfIniFile);
    if (GFP_res == NULL) {
        return OK;
    }
    if (!GFP_res->IsOk()) {
        return OK;
    }
    if (init_gfp_profile(GFP_res) == -1) {
        return OK;
    }
    if (init_gfp_dictionary(GFP_res) == -1) {
        return OK;
    }
    OK = true;
    return OK;
}

//------------------------------------------------------------------------------
bool TGFP::ReinitializeDictionary(void) {

    pozabi_slovar();


    OK = false;

    if (GFP_res == NULL) {
        return OK;
    }
    GFP_res->prf_open();
    if (!GFP_res->IsOk()) {
        return OK;
    }

    if (init_gfp_dictionary(GFP_res) == -1) {
        return OK;
    }
    OK = true;
    return OK;
}

//------------------------------------------------------------------------------

bool TGFP::Reinitialize(void) {


    pozabi_naglase();
    pozabi_pravila();

    pozabi_zlogovanje();


    OK = false;

    if (GFP_res == NULL) {
        return OK;
    }
    GFP_res->prf_open();
    if (!GFP_res->IsOk()) {
        return OK;
    }

    if (init_gfp_profile(GFP_res) == -1) {
        return OK;
    }
    OK = true;
    return OK;
}

//------------------------------------------------------------------------------

int TGFP::Go(char *vhod, bool uporabi_slovarje) {

    if (vhod == NULL || strlen(vhod) == 0) {
        strcpy(GFP_param.izh_stavek, "");
        return 0;
    }

    if (strlen(vhod) >= MAX_DOLZ_VHODA) vhod[MAX_DOLZ_VHODA] = 0;

    strcpy(GFP_param.vh_stavek, "");
    strcpy(text_buf_1, vhod);
    PrviVhodniFilter();


    TvoriPovedi();
    pretvori_samostojne_crke();

    pretvori_vel_crke();
    tekst_v_sampa(uporabi_slovarje);

    strcpy(GFP_param.izh_stavek, text_buf_1);
    return (int)strlen(text_buf_1);
}


int TGFP::GoCopyGFP(const char *vhod) {

    if (vhod == NULL || strlen(vhod) == 0) {
        strcpy(GFP_param.izh_stavek, "");
        return 0;
    }

    strcpy(GFP_param.izh_stavek, vhod);
    return (int)strlen(GFP_param.izh_stavek);
}

const char *TGFP::IzhodniStavek_int(void) {
    return GFP_param.izh_stavek;
}
//------------------------------------------------------------------------------

TGFP::~TGFP() {
    pozabi_slovar();

    pozabi_naglase();
    pozabi_pravila();

    pozabi_zlogovanje();



    delete GFP_res;
}

//------------------------------------------------------------------------------



