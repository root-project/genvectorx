#include "SYCLMath/VecOps.h"
#include "SYCLMath/Vector4D.h"
#include <benchmark/benchmark.h>
#include <chrono>
#include <sycl/sycl.hpp>

#ifdef SINGLE_PRECISION
using arithmetic_type = float;
#else
using arithmetic_type = double;
#endif

using vec4d = ROOT::Experimental::LorentzVector<
    ROOT::Experimental::PtEtaPhiM4D<arithmetic_type>>;
template <class T> using Vector = std::vector<T>;

auto GenVectors(int n) {
  auto vectors = std::make_unique<vec4d[]>(n);

  // generate n -4 momentum quantities
  std::for_each(vectors.get(), vectors.get() + n, [](auto &vec) -> void {
    vec = {1., 1., 1., 1.};
  });

  return std::move(vectors);
}

static void BM_InvariantMass(benchmark::State &state) {
  for (auto _ : state) {
    const auto N = state.range(0);
    const auto local_size = 128;
    auto u_vectors = GenVectors(N);
    auto v_vectors = GenVectors(N);

    static sycl::queue queue{sycl::default_selector_v};

    arithmetic_type *masses = new arithmetic_type[N];
    masses = ROOT::Experimental::InvariantMasses<arithmetic_type, vec4d>(
        u_vectors.get(), v_vectors.get(), N, local_size, queue);

    delete[] masses;
  }
}

BENCHMARK(BM_InvariantMass)->RangeMultiplier(2)->Range(1 << 10, 1 << 20);

BENCHMARK_MAIN();
