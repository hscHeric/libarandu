#include <arandu/common.h>
#include <arandu/rng.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

static inline uint64_t rotl(const uint64_t x, int k) {
  return (x << k) | (x >> (64 - k));
}

uint64_t next_splitmix64(uint64_t *x) {
  uint64_t z = (*x += 0x9e3779b97f4a7c15);
  z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
  z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
  return z ^ (z >> 31);
}

void seed(arandu_rng *rng, uint64_t seed) {
  if (ARANDU_UNLIKELY(rng == NULL)) {
    return;
  }

  if (seed == 0) {
    srand((unsigned)time(NULL));
    seed = ((uint64_t)time(NULL) << 32) ^ (uint64_t)rand();
  }

  rng->seed = seed;
  uint64_t splitmix_state = seed;

  rng->state[0] = next_splitmix64(&splitmix_state);
  rng->state[1] = next_splitmix64(&splitmix_state);
  rng->state[2] = next_splitmix64(&splitmix_state);
  rng->state[3] = next_splitmix64(&splitmix_state);
}

uint64_t next_u64(arandu_rng *rng) {
  const uint64_t result =
      rotl(rng->state[0] + rng->state[3], 23) + rng->state[0];

  const uint64_t t = rng->state[1] << 17;

  rng->state[2] ^= rng->state[0];
  rng->state[3] ^= rng->state[1];
  rng->state[1] ^= rng->state[2];
  rng->state[0] ^= rng->state[3];

  rng->state[2] ^= t;

  rng->state[3] = rotl(rng->state[3], 45);

  return result;
}

void jump(arandu_rng *rng) {
  static const uint64_t JUMP[] = {0x180ec6d33cfd0aba, 0xd5a61266f0c9392c,
                                  0xa9582618e03fc9aa, 0x39abdc4529b1661c};

  uint64_t s0 = 0;
  uint64_t s1 = 0;
  uint64_t s2 = 0;
  uint64_t s3 = 0;
  for (int i = 0; i < sizeof JUMP / sizeof *JUMP; i++)
    for (int b = 0; b < 64; b++) {
      if (JUMP[i] & UINT64_C(1) << b) {
        s0 ^= rng->state[0];
        s1 ^= rng->state[1];
        s2 ^= rng->state[2];
        s3 ^= rng->state[3];
      }
      next_u64(rng);
    }

  rng->state[0] = s0;
  rng->state[1] = s1;
  rng->state[2] = s2;
  rng->state[3] = s3;
}

void long_jump(arandu_rng *rng) {
  static const uint64_t LONG_JUMP[] = {0x76e15d3efefdcbbf, 0xc5004e441c522fb3,
                                       0x77710069854ee241, 0x39109bb02acbe635};

  uint64_t s0 = 0;
  uint64_t s1 = 0;
  uint64_t s2 = 0;
  uint64_t s3 = 0;
  for (int i = 0; i < sizeof LONG_JUMP / sizeof *LONG_JUMP; i++)
    for (int b = 0; b < 64; b++) {
      if (LONG_JUMP[i] & UINT64_C(1) << b) {
        s0 ^= rng->state[0];
        s1 ^= rng->state[1];
        s2 ^= rng->state[2];
        s3 ^= rng->state[3];
      }
      next_u64(rng);
    }

  rng->state[0] = s0;
  rng->state[1] = s1;
  rng->state[2] = s2;
  rng->state[3] = s3;
}

double next_double(arandu_rng *rng);
double next_double_range(arandu_rng *rng, double min, double max);
uint64_t next_u64_range(arandu_rng *rng, uint64_t min, uint64_t max);
double next_normal(arandu_rng *rng);
