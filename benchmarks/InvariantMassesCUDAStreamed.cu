#include "SYCLMath/VecOps.h"
#include "SYCLMath/Vector4D.h"
#include <benchmark/benchmark.h>
#include <chrono>
#include <cuda_runtime.h>
#include <memory>
#include <vector>

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
  std::for_each(vectors.get(), vectors.get() + n,
                [](auto &vec) -> void { vec = {1., 1., 1., 1.}; });

  return std::move(vectors);
}

void BM_InvariantMass(benchmark::State &state) {
  int count;
  cudaGetDeviceCount(&count);
  cudaSetDevice(count - 1);
  cudaStream_t stream;
  cudaStreamCreate(&stream);

  for (auto _ : state) {

    const auto N = state.range(0);
    size_t local_size = 128;

    auto u_vectors = GenVectors(N);
    auto v_vectors = GenVectors(N);

    arithmetic_type *masses = new arithmetic_type[N];

    masses = ROOT::Experimental::InvariantMasses<arithmetic_type, vec4d>(
        u_vectors.get(), v_vectors.get(), N, local_size);
  }
  cudaStreamDestroy(stream);
}

BENCHMARK(BM_InvariantMass)->RangeMultiplier(2)->Range(1 << 10, 1 << 20)->UseRealTime();

BENCHMARK_MAIN();
