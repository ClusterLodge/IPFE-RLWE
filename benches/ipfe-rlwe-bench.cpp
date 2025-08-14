#include "gmp.h"
#include <memory>

extern "C" {
#include "rlwe_sife.h"
#include "sample.h"
#include <stdint.h>
}
#include <benchmark/benchmark.h>

const size_t LEN = 64;

static void BM_rlwe_sife_setup(benchmark::State &state) {
  for (auto _ : state) {
    auto mpk = std::unique_ptr<uint32_t[][SIFE_NMODULI][SIFE_N]>(
        new uint32_t[SIFE_L + 1][SIFE_NMODULI][SIFE_N]());
    auto msk = std::unique_ptr<uint32_t[][SIFE_NMODULI][SIFE_N]>(
        new uint32_t[SIFE_L][SIFE_NMODULI][SIFE_N]());

    rlwe_sife_setup(mpk.get(), msk.get());
  }
}
BENCHMARK(BM_rlwe_sife_setup);

static void BM_rlwe_sife_keygen(benchmark::State &state) {
  auto y = std::unique_ptr<uint32_t[]>(new uint32_t[SIFE_L]);
  sample_y(y.get());

  auto mpk = std::unique_ptr<uint32_t[][SIFE_NMODULI][SIFE_N]>(
      new uint32_t[SIFE_L + 1][SIFE_NMODULI][SIFE_N]());
  auto msk = std::unique_ptr<uint32_t[][SIFE_NMODULI][SIFE_N]>(
      new uint32_t[SIFE_L][SIFE_NMODULI][SIFE_N]());
  rlwe_sife_setup(mpk.get(), msk.get());

  for (auto _ : state) {
    auto sk_y = std::unique_ptr<uint32_t[][SIFE_N]>(
        new uint32_t[SIFE_NMODULI][SIFE_N]());
    rlwe_sife_keygen(y.get(), msk.get(), sk_y.get());
  }
}
BENCHMARK(BM_rlwe_sife_keygen);

static void BM_rlwe_sife_encrypt(benchmark::State &state) {
  auto m = std::unique_ptr<uint32_t[]>(new uint32_t[SIFE_L]);
  sample_x(m.get());

  auto mpk = std::unique_ptr<uint32_t[][SIFE_NMODULI][SIFE_N]>(
      new uint32_t[SIFE_L + 1][SIFE_NMODULI][SIFE_N]());
  auto msk = std::unique_ptr<uint32_t[][SIFE_NMODULI][SIFE_N]>(
      new uint32_t[SIFE_L][SIFE_NMODULI][SIFE_N]());
  rlwe_sife_setup(mpk.get(), msk.get());

  for (auto _ : state) {
    auto c = std::unique_ptr<uint32_t[][SIFE_NMODULI][SIFE_N]>(
        new uint32_t[SIFE_L + 1][SIFE_NMODULI][SIFE_N]());
    rlwe_sife_encrypt(m.get(), mpk.get(), c.get());
  }
}
BENCHMARK(BM_rlwe_sife_encrypt);

static void BM_rlwe_sife_decrypt(benchmark::State &state) {
  auto c = std::unique_ptr<uint32_t[][SIFE_NMODULI][SIFE_N]>(
      new uint32_t[SIFE_L + 1][SIFE_NMODULI][SIFE_N]());
  auto y = std::unique_ptr<uint32_t[]>(new uint32_t[SIFE_L]);
  sample_y(y.get());

  auto mpk = std::unique_ptr<uint32_t[][SIFE_NMODULI][SIFE_N]>(
      new uint32_t[SIFE_L + 1][SIFE_NMODULI][SIFE_N]());
  auto msk = std::unique_ptr<uint32_t[][SIFE_NMODULI][SIFE_N]>(
      new uint32_t[SIFE_L][SIFE_NMODULI][SIFE_N]());
  rlwe_sife_setup(mpk.get(), msk.get());

  auto sk_y =
      std::unique_ptr<uint32_t[][SIFE_N]>(new uint32_t[SIFE_NMODULI][SIFE_N]());
  rlwe_sife_keygen(y.get(), msk.get(), sk_y.get());

  mpz_t dy[SIFE_N];
  for (size_t i = 0; i < SIFE_N; ++i) {
    mpz_init(dy[i]);
  }

  for (auto _ : state) {
    rlwe_sife_decrypt_gmp(c.get(), y.get(), sk_y.get(), dy);
  }

  for (size_t i = 0; i < SIFE_N; ++i) {
    mpz_clear(dy[i]);
  }
}
BENCHMARK(BM_rlwe_sife_decrypt);

BENCHMARK_MAIN();
