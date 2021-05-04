#ifndef pattern_H
#define pattern_H

#ifdef DISABLE_WARNING_4996
    #pragma warning(disable : 4996)
#endif

int patMatch (const char* pattern, const char* string);
int patiMatch (const char* pattern, const char* string);


#endif