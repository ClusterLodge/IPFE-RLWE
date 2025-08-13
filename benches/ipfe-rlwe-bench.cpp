#include "gmp.h"

extern "C" {
    #include <stdint.h>
    #include "rlwe_sife.h"
    #include "sample.h"
}
#include <benchmark/benchmark.h>

const size_t LEN = 64;

static void BM_rlwe_sife_setup(benchmark::State& state) {
    for (auto _ : state) {
        uint32_t mpk[SIFE_L+1][SIFE_NMODULI][SIFE_N];
        uint32_t msk[SIFE_L][SIFE_NMODULI][SIFE_N];
        rlwe_sife_setup(mpk, msk);
    }
}
BENCHMARK(BM_rlwe_sife_setup);

static void BM_rlwe_sife_keygen(benchmark::State& state) {
    uint32_t y[SIFE_L];
    sample_y(y);

    uint32_t mpk[SIFE_L+1][SIFE_NMODULI][SIFE_N];
    uint32_t msk[SIFE_L][SIFE_NMODULI][SIFE_N];
    rlwe_sife_setup(nullptr, nullptr);

    for (auto _ : state) {
        uint32_t msk[SIFE_L][SIFE_NMODULI][SIFE_N];
        uint32_t sk_y[SIFE_NMODULI][SIFE_N];
        rlwe_sife_keygen(y, msk, sk_y);
    }
}
BENCHMARK(BM_rlwe_sife_keygen);

static void BM_rlwe_sife_encrypt(benchmark::State& state) {
    uint32_t m[SIFE_L];
    sample_x(m);

    uint32_t mpk[SIFE_L+1][SIFE_NMODULI][SIFE_N];
    uint32_t msk[SIFE_L][SIFE_NMODULI][SIFE_N];
    rlwe_sife_setup(nullptr, nullptr);

    for (auto _ : state) {
        uint32_t c[SIFE_L+1][SIFE_NMODULI][SIFE_N];
        rlwe_sife_encrypt(m, mpk, c);
    }
}
BENCHMARK(BM_rlwe_sife_encrypt);

static void BM_rlwe_sife_decrypt(benchmark::State& state) {
    uint32_t c[SIFE_L+1][SIFE_NMODULI][SIFE_N];
    uint32_t y[SIFE_L];
    sample_y(y);

    uint32_t sk_y[SIFE_NMODULI][SIFE_N];
    rlwe_sife_keygen(y, nullptr, sk_y);

    mpz_t dy[SIFE_N];
    for (size_t i = 0; i < SIFE_N; ++i) {
        mpz_init(dy[i]);
    }

    for (auto _ : state) {
        rlwe_sife_decrypt_gmp(c, y, sk_y, dy);
    }

    for (size_t i = 0; i < SIFE_N; ++i) {
        mpz_clear(dy[i]);
    }
}
BENCHMARK(BM_rlwe_sife_decrypt);

BENCHMARK_MAIN();
