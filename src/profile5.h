#ifndef profile5H
#define profile5H

#ifdef DISABLE_WARNING_4996
#pragma warning(disable : 4996)
#endif

#define PRF_DOUBLE 0
#define PRF_FLOAT 1
#define PRF_LONG 2
#define PRF_INT 3

class Profile {

private:
    enum { MAX_INI_NAME = 512 };
    char*  prf_data;
    char** prf_index;
    int    prf_items;
    char   ini_name[MAX_INI_NAME + 1];

    const char* prf_get_data(const char* name);
    int   OK;

public:
    Profile(char *name);
    ~Profile();

    int IsOk();
    bool SetIniName(char *name);
    const char *GetIniName(void);

    int prf_open(void);
    int prf_close(void);
    int prf_get_int(const char *name, int *val);
    int prf_get_inta(const char *name, char sep, int *intp);
    int prf_get_long(const char *name, long *val);
    int prf_get_double(const char *name, double *val);
    int prf_get_strl(const char* name);
    int prf_get_stra(const char *name, char sep, char **strp);
    int prf_get_str(const char* name, char* str);
    int prf_get_count(const char* name, char  sep);
    int prf_get_array(const char *name, char sep, void *ptr, int type);
    int prf_make_stra(const char *name, char sep, char ***ptr);
};

#endif
