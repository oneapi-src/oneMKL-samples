//==============================================================
// Copyright © 2026 Intel Corporation
//
// SPDX-License-Identifier: MIT
// =============================================================

#include <sycl/sycl.hpp>

namespace internal
{

template <typename Item, typename T, class BinaryOperation>
inline T
exclusive_scan(const Item &item, T input, T init, BinaryOperation binary_op, T &group_aggregate)
{
  T output = sycl::exclusive_scan_over_group(item.get_group(), input, init, binary_op);
  if (item.get_local_linear_id() == item.get_local_range().size() - 1)
  {
    group_aggregate = binary_op(output, input);
  }

  group_aggregate = sycl::group_broadcast(
    item.get_group(), group_aggregate, item.get_local_range().size() - 1);
  return output;
}

/// Util function to check whether a device supports some kinds of sycl::aspect.
inline void
has_capability_or_fail(const sycl::device &dev, sycl::aspect aspect)
{
    if (aspect == sycl::aspect::fp64 && !dev.has(aspect))
    {
        throw std::runtime_error("'double' is not supported in '" +
                                 dev.get_info<sycl::info::device::name>() +
                               "' device");
    }
}

}
