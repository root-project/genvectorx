
#include "SYCLMath/Boost.h"
#include "SYCLMath/VecOps.h"
#include "SYCLMath/Vector4D.h"
#include <benchmark/benchmark.h>
#include <chrono>
#include <memory>

#ifdef SINGLE_PRECISION
using Scalar = float;
#else
using Scalar = double;
#endif

using LVector =
    ROOT::Experimental::LorentzVector<ROOT::Experimental::PtEtaPhiM4D<Scalar>>;
using Boost = ROOT::Experimental::Boost;

auto GenVectors(int n) {
  auto vectors = std::make_unique<LVector[]>(n);

  // generate n -4 momentum quantities
  std::for_each(vectors.get(), vectors.get() + n, [](auto &vec) -> void {
    vec = {1., 1., 1., 1.};
  });

  return std::move(vectors);
}

static void BM_ApplyBoost(benchmark::State &state) {
  for (auto _ : state) {
    const auto N = state.range(0);
    auto lvectors = GenVectors(N);
    auto* lvectorsboosted = new LVector[N];

    Boost bst(0.3, 0.4, 0.5);
	lvectorsboosted = ROOT::Experimental::ApplyBoost(lvectors.get(), bst, N);

	delete[] lvectorsboosted;
  }
}

BENCHMARK(BM_ApplyBoost)->RangeMultiplier(2)->Range(1 << 10, 1 << 20);

BENCHMARK_MAIN();
