#include "SYCLMath/GenVector/MathUtil.h"
#include "SYCLMath/Boost.h"
#include "SYCLMath/Vector4D.h"
#include "SYCLMath/VecOps.h"
#include <chrono>
#include <vector>
#include <hip/hip_runtime.h>

#ifdef SINGLE_PRECISION
using Scalar = float;
#else
using Scalar = double;
#endif

using LVector = ROOT::Experimental::LorentzVector<ROOT::Experimental::PxPyPzE4D<Scalar>>;
using Boost = ROOT::Experimental::Boost;

template <class T>
using Vector = std::vector<T>;  // ROOT::RVec<T>;

LVector* GenVectors(int n) {
  LVector* vectors = new LVector[n];

  // generate n -4 momentum quantities
  for (int i = 0; i < n; ++i) {
    // fill vectors
    vectors[i] = {1., 1., 1., 1.};
  }

  return vectors;
}

bool print_if_false(const bool assertion, size_t i) {
  if (!assertion) {
    std::cout << "Assertion failed at index " << i << std::endl;
  }
  return assertion;
}

int main(int argc, char** argv) {
  std::string arg1 = argv[1];
  std::size_t pos;
  std::size_t N = std::stoi(arg1, &pos);
  std::string arg2 = argv[2];
  std::size_t nruns = std::stoi(arg2, &pos);
  size_t local_size = 128;

  int count;
  hipGetDeviceCount(&count);
  std::cout << "HIP available devices: " << count << std::endl;
  hipSetDevice(count - 1);

  LVector* lv = GenVectors(N);
  LVector* lvb = new LVector[N];

  Boost bst(0.3, 0.4, 0.5);

  for (size_t i = 0; i < nruns; i++)
    lvb = ROOT::Experimental::ApplyBoost<Boost, LVector>(lv, bst, N, local_size);

  for (size_t i = 0; i < N; i++) {
    assert(print_if_false((std::abs(lvb[i].Pt() - 1.7225) <= 1e-5), i));
    assert(print_if_false((std::abs(lvb[i].Eta() - 1.96333) <= 1e-5), i));
    assert(print_if_false((std::abs(lvb[i].Phi() - 2.20416) <= 1e-5), i));
    assert(print_if_false((std::abs(lvb[i].M() - 3.11127) <= 1e-5), i));
  }

  delete[] lv;
  delete[] lvb;
  return 0;
}
