#include "SYCLMath/Boost.h"
#include "SYCLMath/GenVector/MathUtil.h"
#include "SYCLMath/VecOps.h"
#include "SYCLMath/Vector4D.h"
#include <benchmark/benchmark.h>
#include <cuda_runtime.h>
#include <memory>
#include <vector>

#ifdef SINGLE_PRECISION
using Scalar = float;
#else
using Scalar = double;
#endif

using LVector =
    ROOT::Experimental::LorentzVector<ROOT::Experimental::PxPyPzE4D<Scalar>>;
using Boost = ROOT::Experimental::Boost;

template <class T> using Vector = std::vector<T>; // ROOT::RVec<T>;

auto GenVectors(int n) {
  auto vectors = std::make_unique<LVector[]>(n);

  // generate n -4 momentum quantities
  std::for_each(vectors.get(), vectors.get() + n,
                [](auto &vec) -> void { vec = {1., 1., 1., 1.}; });

  return std::move(vectors);
}

static void BM_ApplyBoost(benchmark::State &state) {
  int count;
  cudaGetDeviceCount(&count);
  cudaSetDevice(count - 1);
  cudaStream_t stream;
  cudaStreamCreate(&stream);

  for (auto _ : state) {
    const auto N = state.range(0);
    size_t local_size = 128;

    auto lvectors = GenVectors(N);
    LVector *lvectorsboosted = new LVector[N];

    Boost bst(0.3, 0.4, 0.5);

    lvectorsboosted = ROOT::Experimental::ApplyBoost<Boost, LVector>(
        lvectors.get(), bst, N, local_size, stream);

    delete[] lvectorsboosted;
  }
  cudaStreamDestroy(stream);
}

BENCHMARK(BM_ApplyBoost)->RangeMultiplier(2)->Range(1 << 10, 1 << 20)->UseRealTime();

BENCHMARK_MAIN();
