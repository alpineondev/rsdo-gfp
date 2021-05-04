#ifdef _VISUALC_CE_PH_
#include "stdafx.h"
#endif
#include "tstring.h"
#ifdef _BORLANDC_
#include <dir.h>
#endif
#if defined(_VISUALC_)
#include <io.h>
#elif defined(_VISUALC_CE_)
#include <wce_io.h>
#endif

//---------------------------------------------------------------------------

static const char check_valid_num[] = "1234567890";
static const char check_valid_first[] = "-";

#define CHECK_BUFFER 2048
char *check_valid_input(char *s) {
    static char buf[CHECK_BUFFER + 1];
    strcpy(buf, s);
    int i = 0;
    int len = (int)strlen(buf);
    int first, last;
    bool neg = false;
    while (i < len && strchr(check_valid_num, buf[i]) == NULL) {
        if (strchr(check_valid_first, buf[i]) != NULL) neg = true;
        i++;
    }
    first = i;
    while (i < len && strchr(check_valid_num, buf[i]) != NULL) i++;
    last = i - 1;
    if (last - first < 0) {
        buf[0] = 0;
        return buf;
    }

    if (first > 0) {
        if (neg) {
            buf[0] = '-';
            memmove(s + 1, s + first, last - first + 1);
            s[last - first + 2] = 0;
        }
        else {
            memmove(s, s + first, last - first + 1);
            buf[last - first + 1] = 0;
        }
    }
    return buf;
}

//---------------------------------------------------------------------------

Tstring IncludeTrailingBackslash(const Tstring s) {
    Tstring result(s);
    result = result.trim();
    if (result.len() == 0 || result[result.len() - 1 + TSTRING_BASE] != '\\') result += "\\";
    return result;
}

//---------------------------------------------------------------------------
Tstring IncludeTrailingSlash(const Tstring s) {
    Tstring result(s);
    result = result.trim();
    if (result.len() == 0 || result[result.len() - 1 + TSTRING_BASE] != '/') result += "/";
    return result;
}

//---------------------------------------------------------------------------
Tstring LinuxDelimiters(const Tstring s) {
    Tstring result(s);
    int i;
    for (i = TSTRING_BASE; i < result.len() + TSTRING_BASE; i++) {
        if (result[i] == '\\') result[i] = '/';
    }
    return result;
}
//---------------------------------------------------------------------------
char *LinuxDelimiters(char *s) {
    unsigned int i;
    for (i = 0; i < strlen(s); i++) {
        if (s[i] == '\\') s[i] = '/';
    }
    return s;
}
//---------------------------------------------------------------------------
Tstring WinDelimiters(const Tstring s) {
    Tstring result(s);
    int i;
    for (i = TSTRING_BASE; i < result.len() + TSTRING_BASE; i++) {
        if (result[i] == '/') result[i] = '\\';
    }
    return result;
}
//---------------------------------------------------------------------------
char *WinDelimiters(char *s) {
    unsigned int i;
    for (i = 0; i < strlen(s); i++) {
        if (s[i] == '/') s[i] = '\\';
    }
    return s;
}
//---------------------------------------------------------------------------
Tstring ExtractFileExt(const Tstring s) {
    Tstring result(s);
    int last = TSTRING_BASE - 1;
    int now;
    int offset = TSTRING_BASE;
    while ((now = result.pos(".", offset)) > 0) { last = now; offset = last + 1; }
    if (last >= TSTRING_BASE) {
        result = result.substr(last, result.len() - last + TSTRING_BASE);
    }
    else result = "";
    return result;
}

//---------------------------------------------------------------------------
Tstring ChangeFileExt(const Tstring s, const Tstring ext) {
    Tstring result(s);
    int last = TSTRING_BASE - 1;
    int now;
    int offset = TSTRING_BASE;
    while ((now = result.pos(".", offset)) > 0) { last = now; offset = last + 1; }
    if (last >= TSTRING_BASE) {
        result.del(last, result.len() - last + 1);
        result += ext;
    }
    return result;
}

//---------------------------------------------------------------------------
Tstring ChangeFileExt(const Tstring s, const char *ext) {
    return ChangeFileExt(s, Tstring(ext));
}

//---------------------------------------------------------------------------
Tstring ExtractFileName(const Tstring s) {
    Tstring result(s);
    int last = TSTRING_BASE - 1;
    int now;
    int offset = TSTRING_BASE;
#ifdef LINUX
    while ((now = result.pos("/", offset)) > 0)
#else
    while ((now = result.pos("\\", offset)) > 0)
#endif
    {
        last = now; offset = last + 1;
    }
    if (last >= TSTRING_BASE) {
        result = result.substr(last + 1, result.len() - last + TSTRING_BASE - 1);
    }
    return result;
}

//---------------------------------------------------------------------------
Tstring ExtractFilePath(const Tstring s) {
    Tstring result(s);
    int last = TSTRING_BASE - 1;
    int now;
    int offset = TSTRING_BASE;
#ifdef LINUX
    while ((now = result.pos("/", offset)) > 0)
#else
    while ((now = result.pos("\\", offset)) > 0)
#endif
    {
        last = now; offset = last + 1;
    }
    if (last >= TSTRING_BASE) {
        result = result.substr(TSTRING_BASE, last + TSTRING_BASE - 1);
    }
    else result = "";
    return result;
}

//---------------------------------------------------------------------------
Tstring int_to_str(int i) {
    Tstring result("");
    result.sprintf("%ld", i);
    return result;
}

//---------------------------------------------------------------------------
bool FileExists(const Tstring FileName) {
#ifdef LINUX
    FILE *dat;
    dat = fopen(FileName.c_str(), "rb");
    if (dat != NULL) {
        fclose(dat);
        return true;
    }
    else return false;
#endif

#ifdef _BORLANDC_
    struct ffblk info;
    if (findfirst(FileName.c_str(), &info, 0) == 0) return true; else return false;
#endif

#if defined(_VISUALC_)
    struct _finddata_t info;
    intptr_t handle;
    if ((handle = _findfirst(FileName.c_str(), &info)) != -1) {
        _findclose(handle);
        return true;
    }
    else return false;
#elif defined(_VISUALC_CE_)
    struct _finddata_t info;
    intptr_t handle;
    if ((handle = wceex_findfirst(FileName.c_str(), &info)) != -1) {
        wceex_findclose(handle);
        return true;
    }
    else return false;
#endif
}

//---------------------------------------------------------------------------
FILE *FileCreate(const Tstring name, Tstring attr) {
    return fopen(name.c_str(), attr.c_str());
}

//---------------------------------------------------------------------------
FILE *FileOpen(const Tstring name, Tstring attr) {
    return fopen(name.c_str(), attr.c_str());
}

//---------------------------------------------------------------------------
void FileSeek(FILE *fp, const int offset, const int origin) {
    int SEEK_OR;
    switch (origin) {
    case 0: SEEK_OR = SEEK_SET; break;
    case 1: SEEK_OR = SEEK_CUR; break;
    case 2: SEEK_OR = SEEK_END; break;
    default: SEEK_OR = SEEK_SET; break;
    }
    fseek(fp, offset, SEEK_OR);
}

//---------------------------------------------------------------------------
int FileWrite(FILE *fp, char *data, int data_len) {
    return (int)fwrite(data, 1, data_len, fp);
}

//---------------------------------------------------------------------------
void FileClose(FILE *fp) {
    fclose(fp);
}

//---------------------------------------------------------------------------
int StrToInt(Tstring s) {
    return atoi(s.c_str());
}

//---------------------------------------------------------------------------
int str_to_int_def(Tstring s, int d) {
    bool ok = true;
    for (int i = TSTRING_BASE; i < s.len() + TSTRING_BASE; i++)
        if (isdigit(s[i]) == 0 && s[i] != '+' && s[i] != '-') { ok = false; break; }
    if (!ok) return d; else return atoi(s.c_str());
}

//---------------------------------------------------------------------------

void Tstring::make_space(int n_len) {
    if (data_len <= n_len) {
        int old_data_len = data_len;
        while (data_len <= n_len) data_len += TSTRING_INC;
        if (data != NULL) {
            char *temp = data;
            data = new char[data_len + 1];
            memcpy(data, temp, old_data_len + 1);
            delete[] temp;
        }
        else {
            data = new char[data_len + 1];
            memset(data, 0, data_len + 1);
        }
    }
    if (data == NULL) throw - 1;
}
//------------
void Tstring::put(const char *n) {
    if (n == NULL) return;
    int n_len = (int)strlen(n);
    make_space(n_len);
    strcpy(data, n);
}
//------------
void Tstring::put(const Tstring &s) {
    put(s.get());
}
//------------
const char *Tstring::get(void) const {
    return data;
}
//------------
Tstring::Tstring() {
    data = NULL;
    data_len = 0;
}
//------------
Tstring::Tstring(const char *n) {
    data_len = 0;
    data = NULL;
    put(n);
}
//------------
Tstring::Tstring(const Tstring &s) {
    data_len = 0;
    data = NULL;
    put(s);
}
//------------
Tstring::Tstring(const char c) {
    data_len = 0;
    data = NULL;
    sprintf("%c", c);
}
//------------
Tstring::~Tstring() {
    if (data != NULL) delete[] data;
}
//------------
void Tstring::add(const char *n) {
    int n_len = len() + (int)strlen(n);
    make_space(n_len);
    strcat(data, n);
}
//------------
void Tstring::insert(const char *n, int start) {
    start -= TSTRING_BASE;
    if (start < 0 || start >= len()) {
        add(n);
        return;
    }
    int n_len = len() + (int)strlen(n);
    make_space(n_len);
    memmove(data + start + strlen(n), data + start, len() - start + 1);
    memcpy(data + start, n, strlen(n));
}
//------------
void Tstring::del(int start, int s_len) {
    start -= TSTRING_BASE;
    if (start >= len()) return;
    if (start < 0) { s_len -= start; start = 0; }
    if (start + s_len >= len()) { data[start] = 0; return; }
    if (len() - start - s_len + 1 > 0) memmove(data + start, data + start + s_len, len() - start - s_len + 1);
}
//------------
const char *Tstring::c_str(void) const {
    return data;
}
//------------
int Tstring::len(void) {
    if (data == NULL) return 0;
    else return (int)strlen(data);
}
//------------
void Tstring::sprintf(const char *n, ...)
{
    va_list argptr;
    text_buf = (char*)malloc(TSTRING_SPRINTF);
    if (text_buf != NULL) {
        if (n)
        {
            va_start(argptr, n);
#ifndef _VISUALC_CE_
            vsnprintf(text_buf, TSTRING_SPRINTF, n, argptr);
#else
            _vsnprintf(text_buf, TSTRING_SPRINTF, n, argptr);
#endif
            put(text_buf);
        }
        free(text_buf);
        text_buf = NULL;
    }
    }
//------------
void Tstring::vprintf(const char *n, va_list argptr)
{
    text_buf = (char*)malloc(TSTRING_SPRINTF);
    if (text_buf != NULL) {
        if (n)
        {
#ifndef _VISUALC_CE_
            vsnprintf(text_buf, TSTRING_SPRINTF, n, argptr);
#else
            _vsnprintf(text_buf, TSTRING_SPRINTF, n, argptr);
#endif
            put(text_buf);
        }
        free(text_buf);
        text_buf = NULL;
    }
    }
//------------
void Tstring::sprintfadd(const char *n, ...)
{
    va_list argptr;
    text_buf = (char*)malloc(TSTRING_SPRINTF);
    if (text_buf != NULL) {
        if (n)
        {
            va_start(argptr, n);
#ifndef _VISUALC_CE_
            vsnprintf(text_buf, TSTRING_SPRINTF, n, argptr);
#else
            _vsnprintf(text_buf, TSTRING_SPRINTF, n, argptr);
#endif
            add(text_buf);
        }
        free(text_buf);
        text_buf = NULL;
    }
    }
//------------
Tstring Tstring::substr(int start, int s_len) {
    start -= TSTRING_BASE;
    Tstring result(get());
    if (start >= len() || s_len <= 0) {
        result = "";
        return result;
    }
    if (start + s_len <= len()) result.del(start + s_len + 1, len());
    result.del(TSTRING_BASE, start);
    return result;
}
//------------
Tstring Tstring::trim(void) {
    int I, L;
    Tstring Result;
    L = len();
    I = 0;
    while ((I < L) && (data[I] <= ' ' && data[I] >= 0)) I++;
    if (I >= L)
        Result = "";
    else {
        while (L > 0 && (data[L - 1] <= ' ' && data[L - 1] >= 0)) L--;
        Result = substr(I + TSTRING_BASE, L - I);
    }
    return Result;
}
//------------
Tstring Tstring::trim_spaces_only(void) {
    char Spaces[5] = { 0x09,0x0A,0x0D,32,'\0' };
    return trim_user_defined(Spaces);
}

//------------
Tstring Tstring::trim_user_defined_int(const char *spaces) {

    int I, L;
    Tstring Result;
    L = len();
    I = 0;
    while ((I < L) && (strchr(spaces, data[I]) != NULL)) I++;
    if (I >= L)
        Result = "";
    else {
        while (L > 0 && (strchr(spaces, data[L - 1]) != NULL)) L--;
        Result = substr(I + TSTRING_BASE, L - I);
    }
    return Result;
}
//------------
Tstring Tstring::trim_user_defined(const char *spaces) {
    return trim_user_defined_int(spaces);
}
//------------
Tstring Tstring::trim_user_defined(Tstring spaces) {
    return trim_user_defined_int(spaces.c_str());
}
//------------
Tstring Tstring::to_lower_case(void) {
    int i;
    Tstring result(get());
    for (i = 0; i < len(); i++) data[i] = tolower(data[i]);
    return result;
}
//------------

Tstring Tstring::to_upper_case(void) {
    int i;
    Tstring result(get());
    for (i = 0; i < len(); i++) data[i] = toupper(data[i]);
    return result;
}
//------------
int Tstring::pos(const Tstring &s, int start) {
    if (start < TSTRING_BASE) start = TSTRING_BASE;
    if (start - TSTRING_BASE > len() - 1) return TSTRING_BASE - 1;
    char *rez = strstr(data + start - TSTRING_BASE, s.c_str());
    if (rez == NULL) return TSTRING_BASE - 1;
    else return (int)((rez - data) + TSTRING_BASE);
}

//---------------------------------------------------------------------------
