#ifndef THORN_CORE_ATTRIBUTES_H
#define THORN_CORE_ATTRIBUTES_H

/*
 * Portable compiler helpers for C99.
 *
 * Everything here degrades gracefully: unsupported compilers simply get the
 * empty expansion, so the macros are safe to use unconditionally.
 */

#if defined(__has_attribute)
#  define THORN_HAS_ATTRIBUTE(x) __has_attribute(x)
#else
#  define THORN_HAS_ATTRIBUTE(x) 0
#endif

/* C23 provides standard attributes; use them when available. */
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 202311L)
#  define THORN_NODISCARD [[nodiscard]]
#  define THORN_DEPRECATED [[deprecated]]
#elif defined(__GNUC__) || defined(__clang__)
#  define THORN_NODISCARD __attribute__((warn_unused_result))
#  define THORN_DEPRECATED __attribute__((deprecated))
#else
#  define THORN_NODISCARD
#  define THORN_DEPRECATED
#endif

#if defined(__GNUC__) || defined(__clang__)
#  define THORN_NORETURN __attribute__((noreturn))
#  define THORN_UNUSED __attribute__((unused))
#  define THORN_LIKELY(x) __builtin_expect(!!(x), 1)
#  define THORN_UNLIKELY(x) __builtin_expect(!!(x), 0)
#elif defined(_MSC_VER)
#  define THORN_NORETURN __declspec(noreturn)
#  define THORN_UNUSED
#  define THORN_LIKELY(x) (x)
#  define THORN_UNLIKELY(x) (x)
#else
#  define THORN_NORETURN
#  define THORN_UNUSED
#  define THORN_LIKELY(x) (x)
#  define THORN_UNLIKELY(x) (x)
#endif

#if defined(_MSC_VER)
#  define THORN_INLINE static __inline
#elif defined(__GNUC__) || defined(__clang__)
#  define THORN_INLINE static inline
#else
#  define THORN_INLINE static
#endif

#define THORN_ARRAY_LEN(array) (sizeof(array) / sizeof((array)[0]))

#define THORN_MIN(a, b) (((a) < (b)) ? (a) : (b))
#define THORN_MAX(a, b) (((a) > (b)) ? (a) : (b))

#define THORN_STRINGIFY_IMPL(x) #x
#define THORN_STRINGIFY(x) THORN_STRINGIFY_IMPL(x)

#define THORN_JOIN_IMPL(a, b) a##b
#define THORN_JOIN(a, b) THORN_JOIN_IMPL(a, b)

#endif /* THORN_CORE_ATTRIBUTES_H */
