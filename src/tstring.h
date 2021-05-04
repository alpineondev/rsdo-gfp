//---------------------------------------------------------------------------

#ifndef tstringH
#define tstringH

#ifdef DISABLE_WARNING_4996
    #pragma warning(disable : 4996)
#endif

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "tstring.h"
#include <ctype.h>
//---------------------------------------------------------------------------
char *check_valid_input(char *s);
//---------------------------------------------------------------------------
#define TSTRING_INC 100
#define TSTRING_SPRINTF 2L*TSTRING_INC
#define TSTRING_BASE 1
class Tstring{
//------------
  private:
//------------
    char *data;
    int data_len;
    char *text_buf;
//------------
    void make_space(int n_len);
//------------
    void put(const char *n);
//------------
    void put(const Tstring &s);
//------------
    const char *get(void) const;
//------------
  public:
//------------
    Tstring();
//------------
    Tstring(const char *n);
//------------
    Tstring(const Tstring &s);
//------------
    Tstring(const char c);
//------------
    ~Tstring();
//------------
//------------
    void add(const char *n);
//------------
    void insert(const char *n, int start);
//------------
    void del(int start, int s_len);
//------------
    const char *c_str(void) const;
//------------
    int len(void);
//------------
    Tstring operator + (const char *n){
       Tstring result(get());
       result.add(n);
       return result;
    }
//------------
    Tstring operator + (const Tstring &s){
       Tstring result(get());
       result.add(s.get());
       return result;
    }
//------------
    Tstring operator + (const char c){
       Tstring result(get());
       result.sprintfadd("%c",c);
       return result;
    }
//------------
    bool operator == (const char *c){
       if (strcmp(data,c)==0) return true;
       else return false;
    }
//------------
    bool operator == (const Tstring &s){
       if (strcmp(data,s.c_str())==0) return true;
       else return false;
    }
//------------
    void operator += (const char *n){
       add(n);
    }
//------------
    void operator += (const Tstring &s){
       add(s.get());
    }
//------------
    void operator += (const char c){
       sprintfadd("%c",c);
    }
//------------
    void operator = (const char *n){
       put(n);
    }
//------------
    void operator = (const Tstring &s){
       if (this == &s) return;
       put(s.get());
    }
//------------
    char &operator [](const int i){
       if (i<TSTRING_BASE || i>=len()+TSTRING_BASE){
          throw -2;
       }
       return data[i-TSTRING_BASE];
    }
//------------
    void sprintf(const char *n, ...);
//------------
    void vprintf(const char *n, va_list argptr);
//------------
    void sprintfadd(const char *n, ...);
//------------
    Tstring substr(int start, int s_len);
//------------
    Tstring trim(void);
    Tstring trim_spaces_only(void);
    Tstring trim_user_defined_int(const char *spaces);
    Tstring trim_user_defined(const char *spaces);
    Tstring trim_user_defined(Tstring spaces);
//------------
    Tstring to_lower_case(void);
//------------
    Tstring to_upper_case(void);
//------------
    int pos(const Tstring &s, int start=TSTRING_BASE);
//------------
};

//---------------------------------------------------------------------------

Tstring IncludeTrailingBackslash(const Tstring s);
Tstring IncludeTrailingSlash(const Tstring s);
Tstring ExtractFileExt(const Tstring s);
Tstring LinuxDelimiters(const Tstring s);
Tstring WinDelimiters(const Tstring s);
char *  LinuxDelimiters(char *s);
char *  WinDelimiters(char *s);
Tstring ChangeFileExt(const Tstring s,const Tstring ext);
Tstring ChangeFileExt(const Tstring s,const char *ext);
Tstring ExtractFileName(const Tstring s);
Tstring ExtractFilePath(const Tstring s);
Tstring int_to_str(int i);

bool FileExists(const Tstring FileName);
FILE *FileCreate(const Tstring name);
FILE *FileOpen (const Tstring name, Tstring attr);
void FileSeek(FILE *fp,const int offset,const int origin);
int FileWrite(FILE *fp, char *data, int data_len);
void FileClose(FILE *fp);
int StrToInt(Tstring s);
int str_to_int_def(Tstring s,int d);
//---------------------------------------------------------------------------
#endif
