//---------------------------------------------------------------------------
#ifdef _VISUALC_CE_PH_
#include "stdafx.h"
#endif
#include <stdio.h>
#include "tstring.h"
//---------------------------------------------------------------------------

#ifdef LINUX
#include "ffind.h"
#include <dirent.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include "pattern.h"

int findfirst(const char *pathname, struct ffblk *ffblk, int /*attrib*/) {

#ifdef LINUX
    ffblk->path = IncludeTrailingSlash(ExtractFilePath(Tstring(pathname)));
#else
    ffblk->path = IncludeTrailingBackslash(ExtractFilePath(Tstring(pathname)));
#endif
    ffblk->pattern = ExtractFileName(Tstring(pathname));

    ffblk->dir = opendir(ffblk->path.c_str());
    struct dirent * de;
    if (ffblk->dir != NULL) {
        while ((de = readdir(ffblk->dir)) != NULL) {
            ffblk->name = de->d_name;
            ffblk->size = 0;
            if (ffblk->name.pos(".") <= 0) ffblk->name += ".";
            if (patiMatch(ffblk->pattern.c_str(), ffblk->name.c_str())) {
                ffblk->size = filesize((char*)(ffblk->path + de->d_name).c_str());
                return 0;
            }
        }
    }
    return -1;
}

int findnext(struct ffblk *ffblk) {
    struct dirent * de;
    if (ffblk->dir == NULL) return -1;
    while ((de = readdir(ffblk->dir)) != NULL) {
        ffblk->name = de->d_name;
        ffblk->size = 0;
        if (ffblk->name.pos(".") <= 0) ffblk->name += ".";
        if (patiMatch(ffblk->pattern.c_str(), ffblk->name.c_str())) {
            ffblk->size = filesize((char*)(ffblk->path + de->d_name).c_str());
            return 0;
        }
    }
    return -1;
}

void findclose(struct ffblk *ffblk) {
    if (ffblk->dir != NULL) {
        closedir(ffblk->dir);
        ffblk->dir = NULL;
        ffblk->name = "";
        ffblk->path = "";
        ffblk->pattern = "";
    }
}

long ltell(int filedes) {
    return (long)lseek(filedes, 0L, SEEK_CUR);
} //LINUX
#elif defined(_BORLANDC_)
#include <io.h>
long ltell(int filedes) {
    return (long)tell(filedes);
}
#elif defined(_VISUALC_)
#include <io.h>
long ltell(int filedes) {
    return (long)_lseek(filedes, 0L, SEEK_CUR);
}
#endif

//dolzina odprte datoteke je ...
long filesize(FILE *file) {
    long end = 0, tell = ftell(file);
    fseek(file, 0, SEEK_END);
    end = ftell(file);
    fseek(file, tell, SEEK_SET);
    return end;
}

//dolzina datoteke je ...
long filesize(char *name) {
    long end = 0;
    FILE *dat = fopen(name, "rb");
    if (dat != NULL) {
        fseek(dat, 0, SEEK_END);
        end = ftell(dat);
        fclose(dat);
    }
    return end;
}


