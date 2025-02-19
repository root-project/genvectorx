
#pragma once

#include <alpaka/alpaka.hpp>

namespace alpaka {

  // miscellanea
  template <std::size_t N>
  using DimInt = std::integral_constant<std::size_t, N>;

  template <typename TDim, typename TVal>
  class Vec;

  template <typename TDim, typename TIdx>
  class WorkDivMembers;

  // API
  struct ApiCudaRt;
  struct ApiHipRt;

  // Platforms
  class PltfCpu;
  template <typename TApi>
  class PltfUniformCudaHipRt;
  using PltfCudaRt = PltfUniformCudaHipRt<ApiCudaRt>;
  using PltfHipRt = PltfUniformCudaHipRt<ApiHipRt>;

  // Devices
  class DevCpu;
  template <typename TApi>
  class DevUniformCudaHipRt;
  using DevCudaRt = DevUniformCudaHipRt<ApiCudaRt>;
  using DevHipRt = DevUniformCudaHipRt<ApiHipRt>;

  // Queues
  template <typename TDev>
  class QueueGenericThreadsBlocking;
  using QueueCpuBlocking = QueueGenericThreadsBlocking<DevCpu>;

  template <typename TDev>
  class QueueGenericThreadsNonBlocking;
  using QueueCpuNonBlocking = QueueGenericThreadsNonBlocking<DevCpu>;

  namespace uniform_cuda_hip::detail {
    template <typename TApi, bool TBlocking>
    class QueueUniformCudaHipRt;
  }
  using QueueCudaRtBlocking =
      uniform_cuda_hip::detail::QueueUniformCudaHipRt<ApiCudaRt, true>;
  using QueueCudaRtNonBlocking =
      uniform_cuda_hip::detail::QueueUniformCudaHipRt<ApiCudaRt, false>;
  using QueueHipRtBlocking =
      uniform_cuda_hip::detail::QueueUniformCudaHipRt<ApiHipRt, true>;
  using QueueHipRtNonBlocking =
      uniform_cuda_hip::detail::QueueUniformCudaHipRt<ApiHipRt, false>;

  // Events
  template <typename TDev>
  class EventGenericThreads;
  using EventCpu = EventGenericThreads<DevCpu>;

  template <typename TApi>
  class EventUniformCudaHipRt;
  using EventCudaRt = EventUniformCudaHipRt<ApiCudaRt>;
  using EventHipRt = EventUniformCudaHipRt<ApiHipRt>;

  // Accelerators
  template <typename TApi, typename TDim, typename TIdx>
  class AccGpuUniformCudaHipRt;

  template <typename TDim, typename TIdx>
  using AccGpuCudaRt = AccGpuUniformCudaHipRt<ApiCudaRt, TDim, TIdx>;

  template <typename TDim, typename TIdx>
  using AccGpuHipRt = AccGpuUniformCudaHipRt<ApiHipRt, TDim, TIdx>;

  template <typename TDim, typename TIdx>
  class AccCpuSerial;

  template <typename TDim, typename TIdx>
  class AccCpuTbbBlocks;

  template <typename TDim, typename TIdx>
  class AccCpuOmp2Blocks;

}  // namespace alpaka

namespace alpaka_common {

  // common types and dimensions
  using Idx = uint32_t;
  using Extent = uint32_t;
  using Offsets = Extent;

  using Dim0D = alpaka::DimInt<0u>;
  using Dim1D = alpaka::DimInt<1u>;
  using Dim2D = alpaka::DimInt<2u>;
  using Dim3D = alpaka::DimInt<3u>;

  template <typename TDim>
  using Vec = alpaka::Vec<TDim, Idx>;
  using Vec1D = Vec<Dim1D>;
  using Vec2D = Vec<Dim2D>;
  using Vec3D = Vec<Dim3D>;

  template <typename TDim>
  using WorkDiv = alpaka::WorkDivMembers<TDim, Idx>;
  using WorkDiv1D = WorkDiv<Dim1D>;
  using WorkDiv2D = WorkDiv<Dim2D>;
  using WorkDiv3D = WorkDiv<Dim3D>;

  // host types
  using DevHost = alpaka::DevCpu;
  using PlatformHost = alpaka::PlatformCpu;

}  // namespace alpaka_common

#ifdef ALPAKA_ACC_GPU_CUDA_ENABLED
namespace alpaka_cuda_async {
  using namespace alpaka_common;

  using Platform = alpaka::PlatformCudaRt;
  using Device = alpaka::DevCudaRt;
  using Queue = alpaka::QueueCudaRtNonBlocking;
  using Event = alpaka::EventCudaRt;

  template <typename TDim>
  using Acc = alpaka::AccGpuCudaRt<TDim, Idx>;
  using Acc1D = Acc<Dim1D>;
  using Acc2D = Acc<Dim2D>;
  using Acc3D = Acc<Dim3D>;

#define ALPAKA_ACCELERATOR_NAMESPACE alpaka_cuda_async
}  // namespace alpaka_cuda_async

#endif  // ALPAKA_ACC_GPU_CUDA_ASYNC_BACKEND

#ifdef ALPAKA_ACC_GPU_HIP_ENABLED
namespace alpaka_rocm_async {
  using namespace alpaka_common;

  using Platform = alpaka::PlatformHipRt;
  using Device = alpaka::DevHipRt;
  using Queue = alpaka::QueueHipRtNonBlocking;
  using Event = alpaka::EventHipRt;

  template <typename TDim>
  using Acc = alpaka::AccGpuHipRt<TDim, Idx>;
  using Acc1D = Acc<Dim1D>;
  using Acc2D = Acc<Dim2D>;
  using Acc3D = Acc<Dim3D>;

#define ALPAKA_ACCELERATOR_NAMESPACE alpaka_rocm_async
}  // namespace alpaka_rocm_async
#endif

#ifdef ALPAKA_ACC_CPU_B_SEQ_T_SEQ_ENABLED
namespace alpaka_serial_sync {
  using namespace alpaka_common;

  using Platform = alpaka::PlatformCpu;
  using Device = alpaka::DevCpu;
  using Queue = alpaka::QueueCpuBlocking;
  using Event = alpaka::EventCpu;

  template <typename TDim>
  using Acc = alpaka::AccCpuSerial<TDim, Idx>;
  using Acc1D = Acc<Dim1D>;
  using Acc2D = Acc<Dim2D>;
  using Acc3D = Acc<Dim3D>;

#define ALPAKA_ACCELERATOR_NAMESPACE alpaka_serial_sync
}  // namespace alpaka_serial_sync

#endif  // ALPAKA_ACC_CPU_B_SEQ_T_SEQ_ENABLED

