#ifndef Proteus_GFPH
#define Proteus_GFPH

//#pragma warning(disable : 4996)

extern "C" {

#ifdef _MSC_VER
#define EXPORT __declspec( dllexport )
#else
#define EXPORT __attribute__((visibility("default")))
#endif

//---------------------------------------------------------------------------

EXPORT bool ProtTTS_INIT(const char *base_dir);
EXPORT bool ProtTTS_DEINIT(void);
EXPORT bool ProtTTS_GFP_reset (void);
EXPORT int ProtTTS_GFP(const char *text_utf8, char **text_phonemes, int *text_phonemes_len);

//---------------------------------------------------------------------------
} // extern "C"

#endif
