#ifndef WGETOPT_H
#define WGETOPT_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef WGETOPT_DLL
#define DECLSPEC __declspec(dllexport)
#else
#define DECLSPEC 
#endif

DECLSPEC extern int optind, opterr;
DECLSPEC extern wchar_t *optarg;

DECLSPEC int wgetopt(int argc, wchar_t * const argv[],
        const wchar_t *optstring);

#ifdef __cplusplus
}
#endif

#endif
