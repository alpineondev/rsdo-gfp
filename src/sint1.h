#ifndef _SINT1_H_
#define _SINT1_H_


#include "gfp.h"
#include "tstring.h"

bool SintInitialized(void);
int InitSint(void);
int DeinitSint(void);

extern bool IniOK;
extern bool SintOK;
int process_sint_ini_file(void);
int deallocate_sint_ini_file(void);

char *MakePathAbsolute(const char *base_dir, const char *relative_path);
char *MakePathAbsolute(const char *relative_path, bool use_base = true);
int SetBaseDir(const char *base_dir_path);
int ResetBaseDir(void);
const char *IzhodniStavek(void);
int GrafemskoFonemskaPretvorba(const char *vhod);

#endif
