#ifndef THORN_CORE_EXPORT_H
#define THORN_CORE_EXPORT_H

/*
 * Symbol visibility for shared library builds.
 *
 * THORN_STATIC_DEFINE is defined by the build system when the library is
 * compiled or consumed as a static library. THORN_BUILDING_LIBRARY is defined
 * only while compiling the library itself.
 */

#ifdef THORN_STATIC_DEFINE
#  define THORN_API
#else
#  if defined(_WIN32) || defined(__CYGWIN__)
#    ifdef THORN_BUILDING_LIBRARY
#      define THORN_API __declspec(dllexport)
#    else
#      define THORN_API __declspec(dllimport)
#    endif
#  elif defined(__GNUC__) && (__GNUC__ >= 4)
#    define THORN_API __attribute__((visibility("default")))
#  elif defined(__clang__)
#    define THORN_API __attribute__((visibility("default")))
#  else
#    define THORN_API
#  endif
#endif

#endif /* THORN_CORE_EXPORT_H */
