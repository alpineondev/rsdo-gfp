#include <string>
#include <cstring>
#include <cctype>

#include "conv.h"

static constexpr struct
{
    const char* latin2;
    const char* utf8;
} table[] = {
    {"\xe8", "\xc4\x8d"}, // č
    {"\xc8", "\xc4\x8c"}, // Č
    {"\xe6", "\xc4\x87"}, // ć
    {"\xc6", "\xc4\x86"}, // Ć
    {"\xf0", "\xc4\x91"}, // đ
    {"\xd0", "\xc4\x90"}, // Đ
    {"\xb9", "\xc5\xa1"}, // š
    {"\xa9", "\xc5\xa0"}, // Š
    {"\xbe", "\xc5\xbe"}, // ž
    {"\xae", "\xc5\xbd"}, // Ž
};

static  size_t table_size = sizeof(table) / sizeof(*table);

static  size_t table_utf8_len(size_t i)
{
    return strlen(table[i].utf8);
}

static  size_t table_latin2_len(size_t i)
{
    return strlen(table[i].latin2);
}

static int find_utf8(const char* a) {
    for (size_t i = 0; i < table_size; i++)
        if (strncmp(a, table[i].utf8, table_utf8_len(i)) == 0)
            return (int)i;

    return (int)table_size;
}

static int find_latin2(const char* a) {
    for (size_t i = 0; i < table_size; i++) {
        if (strncmp(a, table[i].latin2, table_latin2_len(i)) == 0)
            return (int)i;
    }

    return (int)table_size;
}

static const char* from_chars(size_t index, convert_direction dir)
{
    return dir == convert_direction::FROM_UTF8_TO_LATIN2
        ? table[index].utf8
        : table[index].latin2;
}

static const char* to_chars(size_t index, convert_direction dir)
{
    return dir == convert_direction::FROM_UTF8_TO_LATIN2
        ? table[index].latin2
        : table[index].utf8;
}

//branje in preverjanje veljavnosti utf-8 znaka
int getUTF8Char(const unsigned char *utf, int *len)
{
    unsigned int c;

    if (utf == NULL)
        goto error;
    if (len == NULL)
        goto error;
    if (*len < 1)
        goto error;

    c = utf[0];
    if (c & 0x80) {
        if (*len < 2)
            goto error;
        if ((utf[1] & 0xc0) != 0x80)
            goto error;
        if ((c & 0xe0) == 0xe0) {
            if (*len < 3)
                goto error;
            if ((utf[2] & 0xc0) != 0x80)
                goto error;
            if ((c & 0xf0) == 0xf0) {
                if (*len < 4)
                    goto error;
                if ((c & 0xf8) != 0xf0 || (utf[3] & 0xc0) != 0x80)
                    goto error;
                *len = 4;
                // 4-bajtna koda
                c = (utf[0] & 0x7) << 18;
                c |= (utf[1] & 0x3f) << 12;
                c |= (utf[2] & 0x3f) << 6;
                c |= utf[3] & 0x3f;
            }
            else {
                // 3-bajtna koda
                *len = 3;
                c = (utf[0] & 0xf) << 12;
                c |= (utf[1] & 0x3f) << 6;
                c |= utf[2] & 0x3f;
            }
        }
        else {
            // 2-bajtna koda
            *len = 2;
            c = (utf[0] & 0x1f) << 6;
            c |= utf[1] & 0x3f;
        }
    }
    else {
        // 1-bajtna koda
        *len = 1;
    }
    return(c);

error:
    *len = 0;
    return(-1);
}


void convert_from_utf8_to_latin2(const std::string& from, std::string& to)
{
    const size_t len = from.length();
    const char* bytes = from.c_str();
    to.resize(0);
    to.reserve(len);

    convert_direction dir = convert_direction::FROM_UTF8_TO_LATIN2;
    int(*find)(const char*) = find_utf8;
    int utf_len, char_value;
    const unsigned char* ubytes = (const unsigned char*)bytes;

    for (size_t i = 0; i < from.length(); ) {
        utf_len = (int)(len - i);
        char_value = getUTF8Char(ubytes + i, &utf_len);

        if (char_value != -1) {
            if (utf_len == 1) {
                //@@@@@ dodano - filtriranje cifer
                if (!std::isdigit(bytes[i])) {
                    to.push_back(bytes[i]);
                }
            }
            else if (utf_len == 2) {
                //preveri preslikovalno tabelo
                size_t index = find(&bytes[i]);
                if (index < table_size) {
                    const char* t = to_chars(index, dir);
                    to.append(t);
                }
            }
            i += utf_len;
        }
        else {
            //preskoči znak
            i++;
        }
    }
}

void convert_from_latin2_to_utf8(const std::string& from, std::string& to)
{
    const size_t len = from.length();
    const char* bytes = from.c_str();
    to.resize(0);
    to.reserve(len);

    convert_direction dir = convert_direction::FROM_LATIN2_TO_UTF8;
    int(*find)(const char*);
    find = find_latin2;

    for (size_t i = 0; i < from.length(); ) {
        size_t index = find(&bytes[i]);
        if (index < table_size) {
            const char* f = from_chars(index, dir);
            const char* t = to_chars(index, dir);
            to.append(t);
            i += strlen(f);
        }
        else {
            to.push_back(bytes[i]);
            i++;
        }
    }
}

#if 0
int main()
{
    std::string from = "Ta suhi škafec pušča. žŽšŠčČ";
    std::string to;

    convert_from_utf8_to_latin2(from, to);
    printf("%s\n", to.c_str());

    convert_from_latin2_to_utf8(to, from);
    printf("%s\n", from.c_str());

    return 0;
}
#endif

