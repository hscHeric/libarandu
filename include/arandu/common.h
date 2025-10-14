#ifndef ARANDU_COMMON_H
#define ARANDU_COMMON_H

#if __STDC_VERSION__ >= 199901L
#define ARANDU_C99 1
#endif
#if __STDC_VERSION__ >= 201112L
#define ARANDU_C11 1
#endif
#if __STDC_VERSION__ >= 201710L
#define ARANDU_C17 1
#endif
#if __STDC_VERSION__ >= 202311L
#define ARANDU_C23 1
#endif

#include <float.h>
#include <limits.h>
#include <stddef.h>
#include <stdint.h>

#if defined(ARANDU_C11) || defined(ARANDU_C23)
#include <stdalign.h>
#include <stdnoreturn.h>
#endif

#ifndef __has_c_attribute
#define __has_c_attribute(x) 0
#endif
#ifndef __has_attribute
#define __has_attribute(x) 0
#endif
#ifndef __has_builtin
#define __has_builtin(x) 0
#endif

#if defined(ARANDU_C23) && __has_c_attribute(nodiscard)
#define ARANDU_NODISCARD [[nodiscard]]
#elif __has_attribute(warn_unused_result)
#define ARANDU_NODISCARD __attribute__((warn_unused_result))
#elif defined(_MSC_VER)
#define ARANDU_NODISCARD _Check_return_
#else
#define ARANDU_NODISCARD
#endif

#if __has_attribute(always_inline)
#define ARANDU_ALWAYS_INLINE inline __attribute__((always_inline))
#elif defined(_MSC_VER)
#define ARANDU_ALWAYS_INLINE __forceinline
#else
#define ARANDU_ALWAYS_INLINE inline
#endif

#if __has_attribute(noinline)
#define ARANDU_NO_INLINE __attribute__((noinline))
#elif defined(_MSC_VER)
#define ARANDU_NO_INLINE __declspec(noinline)
#else
#define ARANDU_NO_INLINE
#endif

#if __has_builtin(__builtin_expect)
#define ARANDU_LIKELY(x) __builtin_expect(!!(x), 1)
#define ARANDU_UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
#define ARANDU_LIKELY(x) (x)
#define ARANDU_UNLIKELY(x) (x)
#endif

#if __has_builtin(__builtin_unreachable)
#define ARANDU_UNREACHABLE() __builtin_unreachable()
#elif defined(_MSC_VER)
#define ARANDU_UNREACHABLE() __assume(0)
#else
#define ARANDU_UNREACHABLE() ((void)0)
#endif

#if __has_builtin(__builtin_assume)
#define ARANDU_ASSUME(cnd) __builtin_assume(cnd)
#elif defined(_MSC_VER)
#define ARANDU_ASSUME(cnd) __assume(cnd)
#else
#define ARANDU_ASSUME(cnd)                                                     \
  do {                                                                         \
    if (!(cnd)) {                                                              \
    }                                                                          \
  } while (0)
#endif

#if __has_builtin(__builtin_prefetch)
#define ARANDU_PREFETCH(p) __builtin_prefetch((p))
#else
#define ARANDU_PREFETCH(p) ((void)0)
#endif

#if __has_attribute(malloc)
#define ARANDU_MALLOC_LIKE __attribute__((malloc))
#else
#define ARANDU_MALLOC_LIKE
#endif

#if __has_attribute(fallthrough)
#define ARANDU_FALLTHROUGH __attribute__((fallthrough))
#else
#define ARANDU_FALLTHROUGH ((void)0)
#endif

#if defined(ARANDU_C99)
#define ARANDU_RESTRICT restrict
#elif defined(_MSC_VER)
#define ARANDU_RESTRICT __restrict
#else
#define ARANDU_RESTRICT
#endif

#if defined(ARANDU_C11) || defined(ARANDU_C23)
#define ARANDU_ALIGNAS(x) alignas(x)
#define ARANDU_ALIGNOF(x) alignof(x)
#elif defined(__GNUC__) || defined(__clang__)
#define ARANDU_ALIGNAS(x) __attribute__((aligned(x)))
#define ARANDU_ALIGNOF(x) __alignof__(x)
#else
#define ARANDU_ALIGNAS(x)
#define ARANDU_ALIGNOF(x) (sizeof(void *))
#endif

#if defined(ARANDU_C11) || defined(ARANDU_C23)
/* usa _Static_assert nativo */
#else
#define _Static_assert(cond, msg)                                              \
  typedef char arandu_static_assert_##__LINE__[(cond) ? 1 : -1]
#endif

#ifndef ARANDU_MIN
#define ARANDU_MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif
#ifndef ARANDU_MAX
#define ARANDU_MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif
#ifndef ARANDU_COUNT_OF
#define ARANDU_COUNT_OF(arr) (sizeof(arr) / sizeof((arr)[0]))
#endif

#define ARANDU_SWAP(T, a, b)                                                   \
  do {                                                                         \
    T _tmp = (a);                                                              \
    (a) = (b);                                                                 \
    (b) = _tmp;                                                                \
  } while (0)

#define ARANDU_CLAMP(x, lo, hi)                                                \
  (((x) < (lo)) ? (lo) : (((x) > (hi)) ? (hi) : (x)))

typedef enum {
  ARANDU_OK = 0,
  ARANDU_ERR_BADARGS = -1,
  ARANDU_ERR_NOMEM = -2,
  ARANDU_ERR_INTERNAL = -3
} arandu_err;

#ifdef NDEBUG
#define ARANDU_ASSERT(x) ((void)0)
#else
#include <assert.h>
#define ARANDU_ASSERT(x) assert(x)
#endif

/**
 * @brief Soma segura de size_t com verificação de overflow.
 * @return 1 em sucesso; 0 se overflow.
 */
static ARANDU_ALWAYS_INLINE int arandu_size_add_ok(size_t a, size_t b,
                                                   size_t *out) {
  if (SIZE_MAX - a < b)
    return 0;
  *out = a + b;
  return 1;
}

/**
 * @brief Multiplicação segura de size_t com verificação de overflow.
 * @return 1 em sucesso; 0 se overflow.
 */
static ARANDU_ALWAYS_INLINE int arandu_size_mul_ok(size_t a, size_t b,
                                                   size_t *out) {
  if (a == 0 || b == 0) {
    *out = 0;
    return 1;
  }
  if (a > SIZE_MAX / b)
    return 0;
  *out = a * b;
  return 1;
}

#include <math.h>
static ARANDU_ALWAYS_INLINE double arandu_map01(double u, double lo,
                                                double hi) {
  if (ARANDU_UNLIKELY(u < 0.0))
    u = 0.0;
  if (ARANDU_UNLIKELY(u > 1.0))
    u = 1.0;

  const double span = hi - lo;
#if defined(ARANDU_C11) || defined(ARANDU_C23)
  return fma(u, span, lo);
#else
  return u * span + lo;
#endif
}

#endif
