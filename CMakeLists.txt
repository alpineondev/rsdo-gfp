cmake_minimum_required(VERSION 3.12)
project(rsdo-gfp)

add_executable(rsdo-gfp
    main.cpp
    src/aparsablestring.cpp
    src/conv.cpp
    src/ffind.cpp
    src/gfp.cpp
    src/linux.cpp
    src/mystrings.cpp
    src/naglas.cpp
    src/paths.cpp
    src/pattern.cpp
    src/pretvori.cpp
    src/profile5.cpp
    src/Proteus_GFP.cpp
    src/samcrke.cpp
    src/simboli.cpp
    src/sint1.cpp
    src/slovar.cpp
    src/stavek.cpp
    src/switches.cpp
    src/tstring.cpp
    src/util_stuff.cpp
    src/velcrke.cpp
    src/zlogi.cpp
    src/aparsablestring.h
    src/conv.h
    src/ffind.h
    src/gfp.h
    src/linux.h
    src/mystrings.h
    src/paths.h
    src/pattern.h
    src/profile5.h
    src/Proteus_GFP.h
    src/sint1.h
    src/switches.h
    src/tstring.h
    src/util_stuff.h
)

target_include_directories(rsdo-gfp PUBLIC
    src
)

target_compile_definitions(rsdo-gfp PUBLIC
    $<$<PLATFORM_ID:Linux>:LINUX>
)

target_compile_options(rsdo-gfp PUBLIC
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall;-Wextra;-Wpedantic>
)
