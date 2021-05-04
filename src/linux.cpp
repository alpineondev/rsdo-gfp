//---------------------------------------------------------------------------
#include "linux.h"
//---------------------------------------------------------------------------
/*
#ifdef LINUX
#include <strings.h>

int strcmpi(const char *s1, const char *s2){
  return strcasecmp(s1,s2);
}

int strnicmp(const char *s1, const char *s2, int maxlen){
  return strncasecmp(s1,s2,maxlen);
}
#endif//LINUX
*/

#ifdef _VISUALC_CE_PH_
#include "stdafx.h"
#endif
#include <string.h>
#include "util_stuff.h"

int m_strcmpi(const char *s1, const char *s2){
  char *str1,*str2;
  int ret=-1;

  str1=(char*)malloc(strlen(s1)+10);
  str2=(char*)malloc(strlen(s2)+10);

  if (str1!=NULL && str2!=NULL){

      strcpy(str1,s1);
      Lower(str1);

      strcpy(str2,s2);
      Lower(str2);

      ret=strcmp(str1,str2);
  }
  if (str1!=NULL) { free(str1); str1=NULL; }
  if (str2!=NULL) { free(str2); str2=NULL; }
  return ret;
}

int m_strnicmp(const char *s1, const char *s2, int maxlen){
  char *str1,*str2;
  int ret=-1;

  str1=(char*)malloc(maxlen+10);
  str2=(char*)malloc(maxlen+10);

  if (str1!=NULL && str2!=NULL){

      memcpy(str1,s1,maxlen);
      str1[maxlen]=0;
      Lower(str1);

      memcpy(str2,s2,maxlen);
      str2[maxlen]=0;
      Lower(str2);

      ret=strcmp(str1,str2);
  }
  if (str1!=NULL) { free(str1); str1=NULL; }
  if (str2!=NULL) { free(str2); str2=NULL; }
  return ret;
}

