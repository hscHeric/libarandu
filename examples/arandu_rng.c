#include <arandu/rng.h>
#include <stdio.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

int main(void) {
  // 1. Inicialização do Gerador
  // ----------------------------------------------------------------------
  printf("--- 1. Inicializacao do Gerador ---\n");
  arandu_rng rng;
  seed(&rng, 0);  // Usar 0 para uma semente baseada no tempo
  printf("Gerador inicializado com a semente: %llu\n\n",
         (unsigned long long)rng.seed);

  // 2. Geração de Números Básicos
  // ----------------------------------------------------------------------
  printf("--- 2. Geracao de Numeros Basicos ---\n");
  printf("next_u64(): %llu\n", (unsigned long long)next_u64(&rng));
  printf("next_u64(): %llu\n", (unsigned long long)next_u64(&rng));
  printf("next_double() [0, 1): %f\n",
         next_double(&rng));  // Equivalente a `rand()` no HHO
  printf("next_double() [0, 1): %f\n\n", next_double(&rng));

  // 3. Geração em Intervalos (muito usado no HHO)
  // ----------------------------------------------------------------------
  printf("--- 3. Geracao de Numeros em Intervalos ---\n");
  // Equivalente a `E0 = 2*rand() - 1`
  double E0 = next_double_range(&rng, -1.0, 1.0);
  printf("Intervalo de double [-1, 1): %f\n", E0);

  // Equivalente a `(ub-lb)*rand+lb` para inicializar posições
  double lower_bound = -100.0;
  double upper_bound = 100.0;
  double initial_pos = next_double_range(&rng, lower_bound, upper_bound);
  printf("Intervalo de double [%.1f, %.1f): %f\n", lower_bound, upper_bound,
         initial_pos);

  // Equivalente a `floor(N*rand()+1)` para selecionar um "falcão"
  uint64_t population_size = 30;
  uint64_t hawk_index =
      next_u64_range(&rng, 0, population_size);  // Gera de 0 a 29
  printf("Intervalo de inteiro [0, %llu): %llu\n\n",
         (unsigned long long)population_size, (unsigned long long)hawk_index);

  // 4. Teste das funções jump e long_jump para paralelismo
  // ----------------------------------------------------------------------
  printf("--- 4. Teste de Jump para Geracao Paralela ---\n");
  arandu_rng rng1, rng2;
  seed(&rng1, 12345);  // Semente fixa para reprodutibilidade
  seed(&rng2, 12345);  // A mesma semente

  // Avança o segundo gerador 2^128 passos
  jump(&rng2);

  printf("Gerador 1 (sequencia normal):\n");
  for (int i = 0; i < 3; ++i) printf("  %f\n", next_double(&rng1));

  printf("\nGerador 2 (apos um 'jump', sequencia diferente):\n");
  for (int i = 0; i < 3; ++i) printf("  %f\n", next_double(&rng2));
  printf("\nAs sequencias sao diferentes, como esperado.\n");

  return 0;
}
