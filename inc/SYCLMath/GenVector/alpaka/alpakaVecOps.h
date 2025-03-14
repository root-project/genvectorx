#include "SYCLMath/GenVector/MathUtil.h"
#include <cstdio>
#include <chrono>
#include <iostream>
#include <type_traits>
#include <alpaka/alpaka.hpp>
#include "config.h"

template <typename TAcc,
          typename TDim,
          typename TIdx,
          typename = std::enable_if_t<alpaka::isAccelerator<TAcc>>>
ALPAKA_FN_HOST auto getWorkDiv(TIdx gridSize, TIdx elementsOrThreads) {
  using WorkDiv = alpaka::WorkDivMembers<TDim, TIdx>;
  if constexpr (std::is_same_v<TAcc, alpaka::AccGpuCudaRt<TDim, TIdx>> ||
                std::is_same_v<TAcc, alpaka::AccGpuHipRt<TDim, TIdx>>) {
    const TIdx elementsPerThread = 1;
    return WorkDiv{gridSize, elementsOrThreads, elementsPerThread};
  } else {
    const TIdx threadsPerBlock = 1;
    return WorkDiv{gridSize, threadsPerBlock, elementsOrThreads};
  }
}

namespace ROOT {

  namespace Experimental {

    template <typename TVector, typename TBoost>
    struct ApplyBoostKernel {
      template <typename TAcc>
      ALPAKA_FN_ACC void operator()(const TAcc& acc,
                                    const TVector* lv,
                                    TVector* lvb,
                                    const TBoost* bst,
                                    size_t N) const {
        for (auto i : alpaka::uniformElements(acc, N)) {
          TBoost bst_loc = bst[0];
          lvb[i] = bst_loc(lv[i]);
        }
      }
    };

    template <typename TAcc,
              typename TVector,
              typename TBoost,
              typename TQueue,
              typename = std::enable_if_t<alpaka::isAccelerator<TAcc>>,
              typename = std::enable_if_t<alpaka::isQueue<TQueue>>>
    ALPAKA_FN_HOST TVector* ApplyBoost(
        TVector* lv, const TBoost& bst, TQueue& queue, size_t N, size_t blocksize) {
      using Dim = alpaka::DimInt<1u>;
      using Idx = uint32_t;
      using Vec = alpaka::Vec<Dim, Idx>;
      using WorkDiv = alpaka::WorkDivMembers<Dim, Idx>;
      const auto devHost = alpaka::getDevByIdx(alpaka::PlatformCpu{}, 0);

      TVector* lvb = new TVector[N];

#ifdef ROOT_MEAS_TIMING
      auto start = std::chrono::system_clock::now();
#endif

      // allocate device buffers
      auto d_lv = alpaka::allocAsyncBuf<TVector, Idx>(queue, static_cast<Idx>(N));
      auto d_lvb = alpaka::allocAsyncBuf<TVector, Idx>(queue, static_cast<Idx>(N));
      auto d_bst = alpaka::allocAsyncBuf<TBoost, Idx>(queue, 1u);

      // memcpy
      alpaka::memcpy(queue, d_lv, alpaka::createView(devHost, lv, Vec{N}));
      alpaka::memcpy(queue, d_bst, alpaka::createView(devHost, &bst, Vec{1}));

      const Idx gridsize = (N + blocksize - 1) / blocksize;
      const auto workdiv = getWorkDiv<TAcc, Dim>(gridsize, static_cast<Idx>(blocksize));

      // launch kernel
      alpaka::exec<TAcc>(queue,
                         workdiv,
                         ApplyBoostKernel<TVector, TBoost>{},
                         d_lv.data(),
                         d_lvb.data(),
                         d_bst.data(),
                         static_cast<Idx>(N));

      // memcpy
      alpaka::memcpy(queue, alpaka::createView(devHost, lvb, Vec{N}), d_lvb);

#ifdef ROOT_MEAS_TIMING
      auto end = std::chrono::system_clock::now();
      auto duration =
          std::chrono::duration_cast<std::chrono::microseconds>(end - start)
              .count() *
          1e-6;
      std::cout << "alpaka time " << duration << " (s)" << std::endl;
#endif

      return lvb;
    }

    template <typename TVector, typename TScalar>
    struct InvariantMassKernel {
      template <typename TAcc>
      ALPAKA_FN_ACC void operator()(const TAcc& acc,
                                    const TVector* v1,
                                    TScalar* m,
                                    size_t N) const {
        for (auto i : alpaka::uniformElements(acc, N)) {
          m[i] = v1[i].mass();
        }
      }
    };

    template <typename TVector, typename TScalar>
    struct InvariantMassesKernel {
      template <typename TAcc>
      ALPAKA_FN_ACC void operator()(const TAcc& acc,
                                    const TVector* v1,
                                    const TVector* v2,
                                    TScalar* m,
                                    size_t N) const {
        for (auto i : alpaka::uniformElements(acc, N)) {
          const auto w = v1[i] + v2[i];
          m[i] = w.mass();
        }
      }
    };

    template <typename TAcc,
              typename TScalar,
              typename TVector,
              typename TQueue,
              typename = std::enable_if_t<alpaka::isAccelerator<TAcc>>,
              typename = std::enable_if_t<alpaka::isQueue<TQueue>>>
    ALPAKA_FN_HOST TScalar* InvariantMass(const TVector* v1,
                                          size_t N,
                                          TQueue& queue,
                                          size_t blocksize) {
      using Dim = alpaka::DimInt<1>;
      using Idx = uint32_t;
      using Vec = alpaka::Vec<Dim, Idx>;
      using WorkDiv = alpaka::WorkDivMembers<Dim, Idx>;
      auto const devHost = alpaka::getDevByIdx(alpaka::PlatformCpu{}, 0);

      TScalar* invMasses = new TScalar[N];

#ifdef ROOT_MEAS_TIMING
      auto start = std::chrono::system_clock::now();
#endif

      // allocate device buffers
      auto d_lv = alpaka::allocAsyncBuf<TVector, Idx>(queue, static_cast<Idx>(N));
      auto d_m = alpaka::allocAsyncBuf<TScalar, Idx>(queue, static_cast<Idx>(N));

      // memcpy
      alpaka::memcpy(queue, d_lv, alpaka::createView(devHost, v1, Vec{N}));

      const Idx gridsize = (N + blocksize - 1) / blocksize;
      const auto workdiv = getWorkDiv<TAcc, Dim>(gridsize, static_cast<Idx>(blocksize));

      // launch kernel
      alpaka::exec<TAcc>(queue,
                         workdiv,
                         InvariantMassKernel<TVector, TScalar>{},
                         d_lv.data(),
                         d_m.data(),
                         static_cast<Idx>(N));

      // memcpy
      alpaka::memcpy(queue, alpaka::createView(devHost, invMasses, Vec{N}), d_m);

#ifdef ROOT_MEAS_TIMING
      auto end = std::chrono::system_clock::now();
      auto duration =
          std::chrono::duration_cast<std::chrono::microseconds>(end - start)
              .count() *
          1e-6;
      std::cout << "alpaka time " << duration << " (s)" << std::endl;
#endif

      return invMasses;
    }

    template <typename TAcc,
              typename TScalar,
              typename TVector,
              typename TQueue,
              typename = std::enable_if_t<alpaka::isAccelerator<TAcc>>,
              typename = std::enable_if_t<alpaka::isQueue<TQueue>>>
    ALPAKA_FN_HOST TScalar* InvariantMasses(
        const TVector* v1, TVector* v2, size_t N, TQueue& queue, size_t blocksize) {
      using Dim = alpaka::DimInt<1>;
      using Idx = uint32_t;
      using Vec = alpaka::Vec<Dim, Idx>;
      using WorkDiv = alpaka::WorkDivMembers<Dim, Idx>;
      auto const devHost = alpaka::getDevByIdx(alpaka::PlatformCpu{}, 0);

      TScalar* invMasses = new TScalar[N];

#ifdef ROOT_MEAS_TIMING
      auto start = std::chrono::system_clock::now();
#endif

      // allocate device buffers
      auto d_v1 = alpaka::allocAsyncBuf<TVector, Idx>(queue, static_cast<Idx>(N));
      auto d_v2 = alpaka::allocAsyncBuf<TVector, Idx>(queue, static_cast<Idx>(N));
      auto d_m = alpaka::allocAsyncBuf<TScalar, Idx>(queue, static_cast<Idx>(N));

      // memcpy
      alpaka::memcpy(queue, d_v1, alpaka::createView(devHost, v1, Vec{N}));
      alpaka::memcpy(queue, d_v2, alpaka::createView(devHost, v2, Vec{N}));

      const Idx gridsize = (N + blocksize - 1) / blocksize;
      const auto workdiv = getWorkDiv<TAcc, Dim>(gridsize, static_cast<Idx>(blocksize));

      // launch kernel
      alpaka::exec<TAcc>(queue,
                         workdiv,
                         InvariantMassesKernel<TVector, TScalar>{},
                         d_v1.data(),
                         d_v2.data(),
                         d_m.data(),
                         static_cast<Idx>(N));

      // memcpy
      alpaka::memcpy(queue, alpaka::createView(devHost, invMasses, Vec{N}), d_m);

#ifdef ROOT_MEAS_TIMING
      auto end = std::chrono::system_clock::now();
      auto duration =
          std::chrono::duration_cast<std::chrono::microseconds>(end - start)
              .count() *
          1e-6;
      std::cout << "alpaka time " << duration << " (s)" << std::endl;
#endif

      return invMasses;
    }

  }  // namespace Experimental
}  // namespace ROOT
