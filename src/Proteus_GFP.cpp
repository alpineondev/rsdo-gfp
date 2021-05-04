#include "sint1.h"
#include "switches.h"
#include "paths.h"
#include "Proteus_GFP.h"
#include <string>
#include <cassert>

#ifdef _WIN32
#include <windows.h>
#else
#include <time.h>
#include <unistd.h>
#endif

#if defined(LINUX) || defined(_LINUX_)
#include <codecvt>
#include <locale>
#endif

#if defined(_VISUALC_)
#include <codecvt>
#include <locale>
#endif

#include "conv.h"

#define MAX_LABELS_LENGTH 10000

//---------------------------------------------------------------------------

bool ProtTTS_INIT(const char *base_dir) {

    SetBaseDir(base_dir);
    try {
        if (process_sint_ini_file() != -1) IniOK = true;
        else {
            return false;
        }
        //inicializacija
        if (IniOK && InitSint() != -1) {
            SintOK = true;
        }
        else {
            deallocate_sint_ini_file();
            return false;
        }
    }
    catch (...) {
        try {
            DeinitSint();
            deallocate_sint_ini_file();
        }
        catch (...) {}
        return false;
    }
    return true;
}

//---------------------------------------------------------------------------

bool ProtTTS_DEINIT(void) {
    try {
        DeinitSint();
        deallocate_sint_ini_file();
    }
    catch (...) {}
    return true;
}

//---------------------------------------------------------------------------

int ProtTTS_GFP(const char *text_utf8, char **text_phonemes, int *text_phonemes_len) {
    std::string text_latin2;

    if (text_utf8 == NULL || strlen(text_utf8) == 0) {
        *text_phonemes = NULL;
        *text_phonemes_len = 0;
        return 0;
    }

    convert_from_utf8_to_latin2(
        std::string(text_utf8),
        text_latin2);
    const char* text = text_latin2.c_str();

    if (SintOK) {

        int ret_g;
        if (text == NULL || strlen(text) == 0) {
            *text_phonemes = NULL;
            *text_phonemes_len = 0;
            return 0;
        }

        ret_g = GrafemskoFonemskaPretvorba((char*)text);

        if (ret_g > 0) {
            *text_phonemes = (char*)IzhodniStavek();
            *text_phonemes_len = (int)strlen(IzhodniStavek());
        }
        else {
            *text_phonemes = NULL;
            *text_phonemes_len = 0;
        }
        if (ret_g < 0) return -1;
        else return ret_g;
    }
    else return -1;
}

//---------------------------------------------------------------------------

bool ProtTTS_GFP_reset(void) {
    if (SintOK) {
        GrafemskoFonemskaPretvorba(".");
        return true;
    }
    return false;
}

//---------------------------------------------------------------------------
