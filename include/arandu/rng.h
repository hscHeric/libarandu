#ifndef ARANDU_RNG_H
#define ARANDU_RNG_H

#include <arandu/common.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  uint64_t state[4];
  uint64_t seed;
} arandu_rng;

// inicializa com um seed 64-bit via splitmix64
void seed(arandu_rng *rng, uint64_t seed);

// retorna uint64 aleatório
uint64_t next_u64(arandu_rng *rng);

// retorna double no intervalo [0,1]
ARANDU_NODISCARD double next_double(arandu_rng *rng);

// salta 2^128 passos
void jump(arandu_rng *rng);

// salta 2^192 passos
void long_jump(arandu_rng *rng);

// retorna um double no intervalo [0,1]
ARANDU_NODISCARD double next_double(arandu_rng *rng);

// retorna um double no intervalo [min, max]
ARANDU_NODISCARD double next_double_range(arandu_rng *rng, double min,
                                          double max);

// retorna um inteiro no intervalo [min, max]
ARANDU_NODISCARD uint64_t next_u64_range(arandu_rng *rng, uint64_t min,
                                         uint64_t max);

// Gera um double aleatório com distribuição normal padrão
ARANDU_NODISCARD double next_normal(arandu_rng *rng);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ARANDU_RNG_H */
