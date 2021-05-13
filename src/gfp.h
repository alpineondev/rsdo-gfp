//---------------------------------------------------------------------------

#ifndef gfpH
#define gfpH

#include "tstring.h"
#include "sint1.h"
#include "profile5.h"

#define     MAXDOLZNAKA         3
#define     MAXSTRINGLEN        512
#define     MAXIZJEM            100

#define     MAX_PATHLEN         256

#define     NVARS               80
#define     NLOCIL              15
#define     NTA                 8

#define     MAX_DOLZ_VHODA      1024*30
#define     MAX_DOLZ_STAVKA     4700

#define     MAX_STEV_BESED      MAX_DOLZ_STAVKA/2
#define     MAX_DOLZ_BESEDE     64
#define     MAXSTFONBES         MAX_DOLZ_BESEDE

#define     MAXSTFONEMOV        64
#define     MAXSTEVFONEMOV      MAXSTFONEMOV
#define     MAXSTZLOGOV         512

#define ARRAY_FONEMOV_LENGTH    65536

#define MAX_KAT                 18
#define MAX_SIM                 256

#define GLAS_SK                 0
#define SAMOGL                  1
#define SOGLAS                  2
#define SOGLAS_PR               3
#define ZVOCNIKI                4
#define NEZV_SOGL               5
#define ZVEN_SOGL               6
#define ZVEN_NEZV               7

#define MCRKA                   9
#define VCRKA                   10
#define CIFRA                   11
#define KLOCILO                 12
#define NLOCILO                 13
#define DLOCILO                 14
#define POSZNAK                 15
#define DODZNAK                 16
#define SAMPA_LOC               17

#define MAX_DCT_FILES           40

#define DICT_FILE               "*.dct"  //slovarji

#define INI_FILE                ".ini"   //inicializacijske datoteke

typedef char _sampa_znak[MAXDOLZNAKA + 1];

struct ZLOG {
    int    zacetek;
    int    osnova;
    int    konec;
    int    naglas;
};

struct BES_LOC {
    char   bl;                       // 'b' - beseda ; 'l' - locilo
    char   cbeseda[MAXSTFONBES + 1];
    int    ibeseda[MAXSTFONBES + 1];
    int    stfon;
    char   zvocnost[MAXSTFONBES + 1];  // 'v' - samoglasnik ; 'n' - nezvocnik ; 'z' - zvocnik
    int    stzlog;
    struct ZLOG zlog[MAXSTZLOGOV + 1];
    char   tip;                      // 'b' - baryton ; 'o' - oxyton
    char   vrstanag;                 // 'a' - akut ; 'c' - cirkumfleks
    char   polozaj;                  // 'Z' 'z' 's' 'k' 'K'
    int    dolfon[MAXSTFONBES + 1];
    int    visfon[MAXSTFONBES + 1];
};

struct BESEDA {
    char   Text[MAX_DOLZ_BESEDE + 64];                          // grafemska transkripcija besede
    char   BesedaVZN[MAX_DOLZ_BESEDE + 64];                     // 'v' - samoglasnik ; 'n' - nezvocnik ; 'z' - zvocnik
    char   BesedaVC[MAX_DOLZ_BESEDE + 64];                      // 'v' - samoglasnik 'c' - soglasnik
    char   BesedaVZN_Zlogi[MAX_DOLZ_BESEDE + MAXSTZLOGOV + 64]; // oznaceni zlogi
    char   BesedaVC_Zlogi[MAX_DOLZ_BESEDE + MAXSTZLOGOV + 64];  // oznaceni zlogi
    int    StZlogov;
    short  NaglaseniZlog;                                       // prvi zlog = 1, drugi 2,...
    struct ZLOG Zlogi[MAXSTZLOGOV + 1];
    char   ZlogBreak;
    int    BesedaLength;
};

//gfp
struct GFP_parameters {
    char vh_stavek[MAX_DOLZ_STAVKA + 64];
    char izh_stavek[MAX_DOLZ_STAVKA + 64];
};

//naslonke
enum NASLONKA {
    NASL_NONE = 0,
    NASL_PROKLITIKA = 1,
    NASL_ENKLITIKA = 2
};

//zlog
struct RAZCL_BESEDA {
    char  Text[MAX_DOLZ_BESEDE + MAXSTZLOGOV + 64];
    short NajverjetZlog;
    short DrugiNajvZlog;
};

struct RAZCL_BESEDA_EinO {
    char  Text[MAX_DOLZ_BESEDE + MAXSTZLOGOV + 64];
    char  Vokal;
};

//okrajsave
struct ZAMENJAVA {
    int   ZacetekOkrajsave;   // pointer v vhodnem stringu
    int   ZapStOkrajsave;     // dolocena s pozicijo v GFP.INI
    bool  DodajSPACE;         // naj se vrine space (npr. univ.dipl.)
};

//pretvori
enum SKP_ZNAKOV {
    SZ_CRKA = 1,
    SZ_LOCILO = 2,
    SZ_PRESLEDEK = 4
};

//slovar
struct PSLOVAR {
    char *Geslo;
    char Prioriteta;  // dopuscamo 255 prioritet
};


class TGFP {

private:
    char text_buf_1[2 * MAX_DOLZ_VHODA + 64];
    char text_buf_2[2 * MAX_DOLZ_VHODA + 64];
    char text_buf_3[2 * MAX_DOLZ_VHODA + 64];
    char text_buf_4[2 * MAX_DOLZ_VHODA + 64];

    //naglas
    char   **prvi_zlog;
    char   **predpone;
    char   **koncppz;
    char   **koncpz;
    char   **konczad;
    int    n_prvi;
    int    n_pred;
    int    n_kppz;
    int    n_kpz;
    int    n_kzad;

    //zlog
    char   grafemi[MAXSTFONEMOV];
    char   srazvg[MAXSTFONEMOV];
    int    istevg;

    char   *fonemi[MAXSTFONEMOV];
    char   srazvf[MAXSTFONEMOV];
    int    istevf;

    char   zlog_break;
    char   samoglasniki[64];

    int    *NajverjetnejeNaglasenZlog;
    int    SizeNajvNaglZlog;

    Tstring ZlogBreak;

    char  *ArrayGrafemov;
    int   ArrayGrafemov_usage_counter;
    char  *ArrayFonemov;
    int   ArrayFonemov_usage_counter;

    //naslonke
    char    **proklitike;
    char    **pro_ali_enklitike;
    int     n_prokl;
    int     n_pro_enkl;
    bool    lepi_le_enkrat;

    //okrajsave
    char    **okrajsave;
    int     n_okrajsav;
    int     *OkrajsavaSize;
    int     *IzgovorjavaSize;

    //pretvori
    char    **pra_slo;
    char    **pravila;
    int     n_prsl;
    int     n_prav;

    //simboli
    short    simbol_tab[MAX_SIM][MAX_KAT]; // tabela simbolov
    unsigned char    vcrke[MAX_SIM];              // seznam velikih crk
    unsigned char    mcrke[MAX_SIM];              // seznam malih crk
    unsigned char    Vsiljeni_presledek;
    bool Uporabi_vsiljeni_presledek;

    //slovar
    char    *slovar;
    PSLOVAR *pslovar;
    long    bes_v_slo;



    bool je_tab(unsigned char znak);
    bool je_nezanimivo_locilo(unsigned char znak);
    int  is_ura_ob(char *niz, int index);
    bool is_ura(char *niz, int &oburi, int &ure, int &minute, int &sekunde);
    void nastavi_govori_nic_minut_ali_sekund(bool vrednost);
    int  ura_len(int oburi, int ure);
    void pripopaj_minute_in_sekunde(char *out_buf, int minute, int sekunde);
    void pripopaj_minute(char *out_buf, int minute);
    void pripopaj_zadek(char *out_buf);
    bool compose_ura(int oburi, int ure, int minute, int sekunde,
        bool govori_nic_minut_ali_sekund, char *out_buf, int out_buf_size);
public:
    //naglas
    int    pripravi_naglase_main(Profile *profile);
    int    pripravi_naglase(Profile *profile);
    void   pozabi_naglase(void);
    int    je_koncnica(char *bes, char **ptr, int n);
    int    je_zacetek(char *bes, char **ptr, int n);
    int    JeSpredajSamogl(char* bes, int position);
    int    JeNosilecZloga(char* bes, int position);
    int    stev_samog(char *bes);
    int    naglasi_besedo(char *bes, char *izh, int naglas);
    int    doloci_naglas(char *bes, char *izh);

    //zlog
    int    pripravi_zlogovanje(class Profile *profile);
    int    GetIndex(char* Niz, short nBytes);
    void   InitArrayFonemov(void);
    void   pozabi_zlogovanje(void);
    int    CbesedaToIbeseda(char *cBeseda, int *iBeseda);
    bool   DolociZvocnost(int *ibeseda, int stfon, char *zvocnost);
    bool   MyStrStr(const char* SearchFor, char* Niz, int j);
    void   DeljenjeNaZloge(char *zvocnost, struct ZLOG *zlog, int* StZlogov);
    int    PostaviMejo(int i, struct ZLOG *zlog, int j, int osnova);
    void   MoveAccentBehindLJ(char* Niz, long size, Tstring IsciZnak);
    void   ZlogovanjePostProcess(char* pBuf);
    bool   DolociZvocnostZaZnakGT(char Znak, int n, int size, char *zvocnost, char *srazvg);
    void   DolociVC(char *cbeseda, int size, char *BesedaVC);
    bool   DolociZvocnostGT(char *cbeseda, int size, char *zvocnost, char *srazvg);
    void   NajdiZlogeBesedeGT(BESEDA* beseda);
    bool   OznaciZlogeGT(char* cBesedilo, Tstring &Result, Tstring &ErrMsg);
    void   AnalizaBesede(BESEDA *beseda);
    bool   JeVokalGT(char Znak);
    bool   JeSpredajVokalGT(char* Niz, int i);
    bool   JeZadajVokalGT(char* Niz, int i, int size);
    void   InitArrayGrafemov(void);
    void   DolociNaglasIzSplosneStat(struct BESEDA *beseda, char* bes2);
    int    GetIndexInFonemi(char* fonem);
    int    GetMestoNaglasa(char* cBeseda);
    char   *GetFonemAtIndex(int Index);





    //pretvori
    int    pripravi_pravila(class Profile *profile);
    void   pozabi_pravila(void);
    char   *pravilo(char **ptr, char **pravila, int n);
    void   FilterLocilaInCrke(char* bes);
    int    pretvori_samostojne_crke(void);
    int    tekst_v_sampa(bool uporabi_slovarje = true);
    bool   je_sampa_crka(char znak);
    void   ZadnjiIzhodniFilter(void);

    //simboli
    int    pripravi_simbole(class Profile *profile);
    int    beri_sim(const char *ime, int index, class Profile *profile);
    int    beri_sim(const char *ime, int index, const char *data);

    int    je_mcrka(unsigned char);
    int    je_vcrka(unsigned char);
    int    je_cifra(unsigned char);
    int    je_kloci(unsigned char);
    int    je_nloci(unsigned char);
    int    je_dloci(unsigned char);
    int    je_pznak(unsigned char);
    int    je_dznak(unsigned char);

    int    je_samog(unsigned char);
    int    je_sogl(unsigned char);
    int    je_soglp(unsigned char);
    int    je_zvoc(unsigned char);
    int    je_nsogl(unsigned char);
    int    je_zsogl(unsigned char);
    int    je_zvnzv(unsigned char);
    int    je_gskup(unsigned char);

    int    je_nagl(unsigned char);
    int    je_presl(unsigned char);
    int    je_nic(unsigned char);

    int    je_crka(unsigned char);
    int    je_loci(unsigned char);
    int    je_vhsim(unsigned char);

    int    je_vskup(unsigned char, int sk);
    int    je_sampa_loci(unsigned char);

    int    mcrka(unsigned char);
    int    vcrka(unsigned char);

    //slovar
    int    JePrioritetenSlovar(char* FileName, char** ImenaPSlovarjev, int n_prior_slov);
    int    pripravi_slovar(class Profile *prf, char *slo_dir);
    void   pozabi_slovar(void);
    char   *isci_slovar(char* bes);

    //stavek
    int    TvoriPovedi(void);



    //velcrke
    int    pretvori_vel_crke(void);



    int    init_gfp_profile(class Profile *prf);
    int    init_gfp_dictionary(class Profile *prf);

    void   PrviVhodniFilter(void);
    //---------------------------------------------------------------------------

    bool                OK;
    Profile             *GFP_res;
    GFP_parameters      GFP_param;

    bool IsOk(void) {
        return OK;
    }

    TGFP(char *NameOfIniFile);
    ~TGFP();

    int bes_iz_slovarja;
    bool ReinitializeAll(char *NameOfIniFile);
    bool ReinitializeDictionary(void);
    bool Reinitialize(void);
    int  Go(char *vhod, bool uporabi_slovarje = true);
    int  GoCopyGFP(const char *vhod);
    const char *IzhodniStavek_int(void);
};
//---------------------------------------------------------------------------
#endif
