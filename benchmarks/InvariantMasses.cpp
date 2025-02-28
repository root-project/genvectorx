#include "SYCLMath/PtEtaPhiM4D.h"
#include "SYCLMath/Vector4D.h"
#include "SYCLMath/VecOps.h"

#include <benchmark/benchmark.h>
#include <memory>

#ifdef SINGLE_PRECISION
using Scalar = float;
#else
using Scalar = double;
#endif

using LVector =
    ROOT::Experimental::LorentzVector<ROOT::Experimental::PtEtaPhiM4D<Scalar>>;

auto GenVectors(int n) {
  auto vectors = std::make_unique<LVector[]>(n);

  // generate n -4 momentum quantities
  std::for_each(vectors.get(), vectors.get() + n, [](auto &vec) -> void {
    vec = {1., 1., 1., 1.};
  });

  return std::move(vectors);
}

static void BM_InvariantMass(benchmark::State& state) {
  for (auto _ : state) {
    const auto N = state.range(0);
	auto u_vectors = GenVectors(N);
	auto v_vectors = GenVectors(N);

	Scalar *masses = new Scalar(N);
	masses = ROOT::Experimental::InvariantMasses<Scalar, LVector>(u_vectors.get(), v_vectors.get(), N);
  }
}

BENCHMARK(BM_InvariantMass)->RangeMultiplier(2)->Range(1 << 10, 1 << 20);

BENCHMARK_MAIN();
