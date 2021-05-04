#include <string>
#include "Proteus_GFP.h"
#include <iostream>
#include <fstream>
#include <cstring>
#ifdef _MSC_VER
#include <windows.h>
#endif

#ifdef _MSC_VER
#pragma execution_character_set( "utf-8" )
#endif

void ProcessOnce(const char *text_utf8);
void ProcessOnce(std::string text_utf8);

int main(int argc, char* argv[])
{
#ifdef _MSC_VER
    SetConsoleOutputCP(65001);
#endif

    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " [OPTIONS] INPUT_FILE\n"
            "Options:\n"
            "  -d DATA_DIR --> set data folder\n"
            "  -b --> break text to lines\n" << std::endl;
        return 0;
    }

    const char* data_dir = ".";
    bool break_to_lines = false;

    int i;
    for (i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            if (!strcmp(argv[i], "-d")) {
                if (++i < argc) {
                    data_dir = argv[i];
                }
            }
            else if (!strcmp(argv[i], "-b")) {
                break_to_lines = true;
            }
        }
        else {
            break;
        }
    }

    if (i != argc - 1) {
        std::cerr << "Missing arguments" << std::endl;
        return 1;
    }

    const char* input_file = argv[argc - 1];

    std::ifstream is(input_file);
    if (!is) {
        std::cerr << "Failed to open input file" << std::endl;
        return 1;
    }

    if (!ProtTTS_INIT(data_dir)) {
        std::cerr << "ProtTTS_INIT failed" << std::endl;
        return 1;
    }

    //char* gfp = nullptr;
    //int gfp_len = 0;
    std::string line;

#if 1
    if (break_to_lines) {
        while (std::getline(is, line)) {
            try {
                ProcessOnce(line);
            }
            catch (...) {
                std::cerr << "Exception in ProtTTS_GFP" << std::endl;
                return 1;
            }

        }
    }
    else {
        std::string lines = "";
        while (std::getline(is, line)) {
            lines += line + " ";
        }
        try {
            ProcessOnce(lines);
        }
        catch (...) {
            std::cerr << "Exception in ProtTTS_GFP" << std::endl;
            return 1;
        }
    }
#endif

    if (!ProtTTS_GFP_reset()) {
        std::cerr << "ProtTTS_GFP_reset failed" << std::endl;
    }

    if (!ProtTTS_DEINIT()) {
        std::cerr << "ProtTTS_DEINIT failed" << std::endl;
    }

    return 0;
}


void ProcessOnce(const char *text_utf8) {
    ProcessOnce(std::string(text_utf8));
}

void ProcessOnce(std::string text_utf8)
{
    int result;
    size_t length = 0;
    char *gfp;
    int gfp_len;
    
    result = ProtTTS_GFP(text_utf8.c_str(), &gfp, &gfp_len);
    printf("tekst = \"%s\"\n\n", text_utf8.c_str());
    if (result > 0) {
        length += gfp_len;
        printf("  gfp = \"%s\"\n\n", gfp);
    }
}
