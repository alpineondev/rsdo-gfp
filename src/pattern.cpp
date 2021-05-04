//
// Simple string pattern matching functions using wildcards ('?' & '*').
//

#ifdef _VISUALC_CE_PH_
#include "stdafx.h"
#endif
#include "pattern.h"
#include <ctype.h>

//
// Case-sensitive pattern match
//
int patMatch (const char* pattern, const char* string){

    switch (pattern[0]){
        case '\0': return !string[0];

        case '*': return patMatch(pattern+1, string) || (string[0] && patMatch(pattern, string+1));

        case '?': return string[0] && patMatch(pattern+1, string+1);

        default: return (pattern[0] == string[0]) && patMatch(pattern+1, string+1);
    }
}

//
// Case-insensitive pattern match
//
int patiMatch (const char* pattern, const char* string){

    switch (pattern[0]){
        case '\0': return !string[0];

        case '*': return patiMatch(pattern+1, string) || (string[0] && patiMatch(pattern, string+1));

        case '?': return string[0] && patiMatch(pattern+1, string+1);

        default:  return (toupper(pattern[0]) == toupper(string[0])) && patiMatch(pattern+1, string+1);
    }
}


