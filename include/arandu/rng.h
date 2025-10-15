#ifndef ARANDU_RNG_H
#define ARANDU_RNG_H

#include <arandu/common.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Mantém o estado interno do gerador de números aleatórios.
 *
 * A estrutura contém um array de 4 palavras de 64 bits que compõem o estado
 * do xoshiro256++ e uma cópia da semente original usada na inicialização.
 */
typedef struct {
  uint64_t state[4];
  uint64_t seed;
} arandu_rng;

/**
 * @brief Inicializa o estado do gerador com uma semente de 64 bits.
 *
 * Utiliza o algoritmo splitmix64 para popular o estado interno do xoshiro256+,
 * que requer 256 bits.
 *
 * @note Se o valor de `seed` for 0, uma nova semente será gerada internamente
 * usando a hora atual e `rand()`, garantindo uma sequência diferente
 * a cada execução.
 *
 * @param rng Ponteiro para a estrutura do gerador a ser inicializada.
 * @param seed O valor de 64 bits para a semente.
 */
void seed(arandu_rng *rng, uint64_t seed);

/**
 * @brief Gera o próximo número inteiro de 64 bits da sequência.
 *
 * @param rng Ponteiro para a estrutura do gerador.
 * @return Um número pseudoaleatório `uint64_t`.
 */
uint64_t next_u64(arandu_rng *rng);

/**
 * @brief Gera um double precisão no intervalo [0.0, 1.0)
 *
 * O valor é gerado utilizando os 53 bits mais significativos do próximo
 * `uint64_t` da sequência para garantir 53 bits de precisão na mantissa.
 *
 * @param rng Ponteiro para a estrutura do gerador.
 * @return Um `double` pseudoaleatório no intervalo [0.0, 1.0)
 */
ARANDU_NODISCARD double next_double(arandu_rng *rng);

/**
 * @brief Avança o estado do gerador em 2^128 passos.
 *
 * É equivalente a chamar next_u64() 2^128 vezes.
 *
 * @param rng Ponteiro para a estrutura do gerador.
 */
void jump(arandu_rng *rng);

/**
 * @brief Avança o estado do gerador em 2^192 passos.
 *
 * É equivalente a chamar next_u64() 2^192 vezes.
 *
 * @param rng Ponteiro para a estrutura do gerador.
 */
void long_jump(arandu_rng *rng);

/** @brief Gera um `uint64_t` uniforme no intervalo [min, max) sem viés.
 *
 * Utiliza um método de rejeição para evitar o viés modular que ocorre ao
 * usar o operador `%` em um intervalo que não é divisor de 2^64.
 *
 * @note Se `min >= max`, a função retorna `min`.
 *
 * @param rng Ponteiro para a estrutura do gerador.
 * @param min O limite inferior (inclusivo) do intervalo.
 * @param max O limite superior (exclusivo) do intervalo.
 * @return Um `uint64_t` pseudoaleatório e sem viés no intervalo especificado.
 */
ARANDU_NODISCARD double next_double_range(arandu_rng *rng, double min,
                                          double max);

/**
 * @brief Gera um `uint64_t` uniforme no intervalo [min, max) sem viés.
 *
 * Utiliza um método de rejeição para evitar o viés modular que ocorre ao
 * usar o operador `%` em um intervalo que não é divisor de 2^64.
 *
 * @note Se `min >= max`, a função retorna `min`.
 *
 * @param rng Ponteiro para a estrutura do gerador.
 * @param min O limite inferior (inclusivo) do intervalo.
 * @param max O limite superior (exclusivo) do intervalo.
 * @return Um `uint64_t` pseudoaleatório e sem viés no intervalo especificado.
 */
ARANDU_NODISCARD uint64_t next_u64_range(arandu_rng *rng, uint64_t min,
                                         uint64_t max);

/**
 * @brief Gera um `double` com distribuição Normal Padrão (N(0, 1)).
 *
 * Utiliza a transformada de Box-Muller para gerar um valor com média 0 e
 * desvio padrão 1.
 *
 * @note Esta implementação calcula dois valores gaussianos (par `cos` e `sin`),
 * mas descarta o segundo. Para aplicações que exigem alto desempenho,
 * pode ser útil modificar a função para armazenar em cache e reutilizar
 * o segundo valor.
 *
 * @param rng Ponteiro para a estrutura do gerador.
 * @return Um `double` pseudoaleatório de uma distribuição normal padrão.
 */
ARANDU_NODISCARD double next_normal(arandu_rng *rng);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ARANDU_RNG_H */
