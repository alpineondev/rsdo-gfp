//---------------------------------------------------------------------------

#ifndef ffindH
#define ffindH

#ifdef DISABLE_WARNING_4996
#pragma warning(disable : 4996)
#endif
#if defined(LINUX) || defined(_BORLANDC_)
#include <dirent.h>
#endif
#include "tstring.h"

//---------------------------------------------------------------------------
#ifdef LINUX
struct ffblk {
    Tstring name;
    int size;
    Tstring path;
    Tstring pattern;
    DIR * dir;

    ffblk(void) {
        dir = NULL;
        name = "";
        path = "";
        pattern = "";
    }
};


int findfirst(const char *pathname, struct ffblk *ffblk, int attrib);
int findnext(struct ffblk *ffblk);
void findclose(struct ffblk *ffblk);
#endif //LINUX

long ltell(int filedes);
//dolzina odprte datoteke je ...
long filesize(FILE *file);
//dolzina datoteke je ...
long filesize(char *name);

#endif
