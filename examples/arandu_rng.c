#include <arandu/rng.h>
#include <stdint.h>
#include <stdio.h>

int main(void) {
  arandu_rng rng;

  // 🔹 Teste determinístico
  printf("== Teste com seed determinística ==\n");
  seed(&rng, 12345678910ULL);
  for (int i = 0; i < 5; i++) {
    uint64_t v = next_u64(&rng);
    printf("%lu\n", v);
  }

  printf("-- Após long_jump --\n");
  long_jump(&rng);
  for (int i = 0; i < 5; i++) {
    uint64_t v = next_u64(&rng);
    printf("%lu\n", v);
  }

  // 🔹 Teste aleatório com seed(0)
  printf("\n== Teste com seed(0) (aleatória) ==\n");
  seed(&rng, 0);
  for (int i = 0; i < 5; i++) {
    uint64_t v = next_u64(&rng);
    printf("%lu\n", v);
  }

  printf("-- Após long_jump --\n");
  long_jump(&rng);
  for (int i = 0; i < 5; i++) {
    uint64_t v = next_u64(&rng);
    printf("%lu\n", v);
  }

  return 0;
}
