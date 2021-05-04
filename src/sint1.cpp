//---------------------------------------------------------------------------
#ifdef _VISUALC_CE_PH_
#include "stdafx.h"
#endif
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "profile5.h"
#include "util_stuff.h"
#include "sint1.h"
#include "mystrings.h"
#include "switches.h"

#ifdef _VISUALC_CE_
#include "wce_direct.h"
#endif

#ifdef _VISUALC_
#include <direct.h>
#endif

#ifdef _BORLANDC_
#include <conio.h>
#include <dir.h>
#endif

#ifdef LINUX
#include <unistd.h>
#endif

bool IniOK = false;
bool SintOK = false;

#include "tstring.h"
#include <time.h>
#include "paths.h"
#include "gfp.h"

TGFP              *GFP;
static Profile    *Sint_locat = NULL;       //branje ini datotek

//=================================================================

struct t_tabela {
    unsigned char crka;
    const char *sampa;
};

//-----------------------

t_tabela pretvorna_tabela[] = {
  {'a',"a"},
  {'b',"b@"},
  {'c',"ts@"},
  {0xE8,"tS@"},
  {0xE6,"mE:hki tS@"},
  {'d',"d@"},
  {0xF0,"dZ@"},
  {'e',"E:"},
  {'f',"f@"},
  {'g',"g@"},
  {'h',"h@"},
  {'i',"i"},
  {'j',"j@"},
  {'k',"k@"},
  {'l',"l@"},
  {'m',"m@"},
  {'n',"n@"},
  {'o',"O:"},
  {'p',"p@"},
  {'q',"ku"},
  {'r',"r@"},
  {'s',"s@"},
  {0x9A,"S@"},
  {'t',"t@"},
  {'u',"u"},
  {'v',"v@"},
  {'w',"dvo:Ini v@"},
  {'x',"iks"},
  {'y',"ipsi:lOn"},
  {'z',"z@"},
  {0x9E,"Z@"},
  {'A',"a"},
  {'B',"b@"},
  {'C',"ts@"},
  {0xC8,"tS@"},
  {0xC6,"mE:hki tS@"},
  {'D',"d@"},
  {0xD0,"dZ@"},
  {'E',"E:"},
  {'F',"f@"},
  {'G',"g@"},
  {'H',"h@"},
  {'I',"i"},
  {'J',"j@"},
  {'K',"k@"},
  {'L',"l@"},
  {'M',"m@"},
  {'N',"n@"},
  {'O',"O:"},
  {'P',"p@"},
  {'Q',"ku"},
  {'R',"r@"},
  {'S',"s@"},
  {0x8A,"S@"},
  {'T',"t@"},
  {'U',"u"},
  {'V',"v@"},
  {'W',"dvo:Ini v@"},
  {'X',"iks"},
  {'Y',"ipsi:lOn"},
  {'Z',"z@"},
  {0x8E,"Z@"},
  {'~',"ti:lda"},
  {'(',"prE:tklEpaI"},
  {')',"za:klEpaI"},
  {'{',"zavi:ti prE:tklEpaI"},
  {'}',"zavi:ti za:klEpaI"},
  {'[',"Ogla:ti prE:tklEpaI"},
  {']',"Ogla:ti za:klEpaI"},
  {'@',"a:fna"},
  {'`',"krati:vEts"},
  {'^',"stre:Sitsa"},
  {'"',"narEkOva:I"},
  {'$',"do:lar"},
  {'%',"prOtsE:nt"},
  {'&',"in"},
  {',',"vE:jitsa"},
  {'.',"pi:ka"},
  {'*',"kra:t"},
  {'+',"plu:s"},
  {'/',"pOSe:Unitsa"},
  {'\\',"pOwra:tna pOSe:Unitsa"},
  {'<',"jEma:nSEOd"},
  {'=',"jEEna:kO"},
  {'>',"jEve:tSjEOd"},
  {';',"pO:tpitSjE"},
  {':',"dvo:pitSjE"},
  {'\'',"apOstro:f"},
  {'*',"zve:zditsa"},
  {'-',"mi:nus"},
  {'_',"pO:ttSrtaI"},
  {'#',"lo:Itra"},
  {'|',"pOkO:ntSnitsa"},
  {'!',"klitsa:I"},
  {'?',"Wpra:SaI"},
  {'1',"E:na"},
  {'2',"dva:"},
  {'3',"tri:"},
  {'4',"Sti:ri"},
  {'5',"pe:t"},
  {'6',"Se:st"},
  {'7',"se:dEm"},
  {'8',"o:sEm"},
  {'9',"dEve:t"},
  {'0',"ni:tS"},
  {0,""}
};


const char* GFP_1crka(char crka) {
    int i = 0;
    while (pretvorna_tabela[i].crka != 0) {
        if (crka == pretvorna_tabela[i].crka) {
            return pretvorna_tabela[i].sampa;
            break;
        }
        i++;
    }
    return ".";
}
//=================================================================

//inicializacija in deinacializacija
static bool bSintInitialized = false;

bool SintInitialized(void) {
    return bSintInitialized;
}

char *MakePathAbsolute(const char *base_dir, const char *relative_path) {

    const int max_buffer = 512;
    static char buffer[max_buffer + 1];

    if (base_dir == NULL || strlen(base_dir) == 0) {
        strncpy(buffer, relative_path, max_buffer);
        return buffer;
    }

    Tstring base_dir_str;
    Tstring relative_path_str;
    base_dir_str = Tstring(base_dir);
    relative_path_str = Tstring(relative_path);

#if defined(_LINUX_) || defined(LINUX)
    base_dir_str = IncludeTrailingSlash(base_dir_str);
    base_dir_str = LinuxDelimiters(base_dir_str);
    relative_path_str = LinuxDelimiters(relative_path_str);
    if (relative_path_str.pos("./") == 1) { //pot je relativna
        relative_path_str.del(1, 2);
    }
#else
    base_dir_str = IncludeTrailingBackslash(base_dir_str);
    base_dir_str = WinDelimiters(base_dir_str);
    relative_path_str = WinDelimiters(relative_path_str);
    if (relative_path_str.pos(".\\") == 1) { //pot je relativna
        relative_path_str.del(1, 2);
    }

#endif
    if (relative_path_str.len() > 0) {
        if (relative_path_str.pos(":") == 2 || relative_path_str[1] == '/' || relative_path_str[1] == '\\') //pot je absolutna
        {
            strncpy(buffer, relative_path, max_buffer);
            return buffer;
        }
        base_dir_str += relative_path_str;
    }
    strncpy(buffer, base_dir_str.c_str(), max_buffer);
    buffer[max_buffer] = 0;
    return buffer;
}

const int max_base_dir_bufer = 512;
static char base_dir_bufer[max_base_dir_bufer + 1];

int SetBaseDir(const char *base_dir_path) {
    strncpy(base_dir_bufer, base_dir_path, max_base_dir_bufer);
    base_dir_bufer[max_base_dir_bufer] = 0;
    return 0;
}

int ResetBaseDir(void) {
    base_dir_bufer[0] = 0;
    return 0;
}

char *MakePathAbsolute(const char *relative_path, bool use_base) {

    const int max_bufer = 512;
    static char bufer_za_direktorij[max_bufer + 1];
#ifdef _VISUALC_CE_
    wceex_getcwd(bufer_za_direktorij, max_bufer);
#elif _VISUALC_
    _getcwd(bufer_za_direktorij, max_bufer);
#elif defined(ANDROID)
    strcpy(bufer_za_direktorij, "/");
#else
    getcwd(bufer_za_direktorij, max_bufer);
#endif

    if (use_base && base_dir_bufer[0] != 0 && strlen(base_dir_bufer) > 0)
        return MakePathAbsolute(base_dir_bufer, relative_path);
    else
        return MakePathAbsolute(bufer_za_direktorij, relative_path);
}

//------------------------------------------------------------------------------
int process_sint_ini_file(void) {

#if defined(_LINUX_) || defined(LINUX)
    //
#elif defined(_VISUALC_CE_)
    wceex_getcwd(Base_directory, MAX_PATHLEN);
#else
    getcwd(Base_directory, MAX_PATHLEN);
#endif

    strcpy(Sint_res_file, MakePathAbsolute("rsdo-gfp.ini"));
    Sint_locat = new Profile(Sint_res_file);

    if (!Sint_locat->IsOk()) {
        //Ni inicializacijske datoteke
        return(-1);
    }
    else {
        if (Sint_locat->prf_get_str("ini_gfp", Gfp_res_file) == -1) {
            return(-1);
        }  SpacesOut(Gfp_res_file);
#ifdef LINUX
        LinuxDelimiters(Gfp_res_file);
#else
        WinDelimiters(Gfp_res_file);
#endif
        strcpy(Gfp_res_file, MakePathAbsolute(Gfp_res_file));

        if (Sint_locat->prf_get_str("dct_dir", Dct_directory) == -1) {
            return(-1);
        }
        SpacesOut(Dct_directory);
        if (Dct_directory[strlen(Dct_directory)] != '\\') strcat(Dct_directory, "\\");
#ifdef LINUX
        LinuxDelimiters(Dct_directory);
#else
        WinDelimiters(Dct_directory);
#endif
        strcpy(Dct_directory, MakePathAbsolute(Dct_directory));
    }

    return 0;
}

//---------------------------------------------------------------------------
int deallocate_sint_ini_file(void) {
    if (Sint_locat != NULL) delete Sint_locat;
    return 0;
}

//---------------------------------------------------------------------------
int InitSint(void) {

    if (bSintInitialized) {
        return(-1);
    }
    bSintInitialized = false;
    GFP = new TGFP(Gfp_res_file);
    bSintInitialized = true;
    return 0;
}

//------------------------------------------------------------------------------

int DeinitSint(void) {
    if (!bSintInitialized) {
        return(-1);
    }
    bSintInitialized = false;
    delete GFP;
    return 0;
}


///////////////////////////////////////////////////////////////////////////////////
//funkcije za uporabo
///////////////////////////////////////////////////////////////////////////////////

int GrafemskoFonemskaPretvorba(const char *vhod) {
    Tstring msg;
    char *besedilo;
    if (vhod == NULL) return -1;
    besedilo = (char*)malloc(strlen(vhod) + 10);
    if (besedilo == NULL) return -1;
    strcpy(besedilo, vhod);

    if (!bSintInitialized) {
        //GFP Ni inicializirana!
        return -1;
    }

    if (strcmp(besedilo, "") == 0) {
        //Vhod je prazen niz!
        return -1;
    }

    int ret;
    if (strlen(besedilo) == 1) {
        char znak = besedilo[0];
        ret = GFP->GoCopyGFP(GFP_1crka(znak));
    }
    else {
        ret = GFP->Go(besedilo);
    }

    if (besedilo != NULL) { free(besedilo); besedilo = NULL; }
    return ret;
}

//------------------------------------------------------------------------------

const char *IzhodniStavek(void) {
    return GFP->IzhodniStavek_int();
}

//------------------------------------------------------------------------------
