//==============================================================
// Copyright © 2021 Intel Corporation
//
// SPDX-License-Identifier: MIT
// =============================================================

/*
 *
 *  Content:
 *       This file contains Student's T-test DPC++ implementation with
 *       USM APIs.
 *
 *******************************************************************************/

#include <oneapi/dpl/execution>
#include <oneapi/dpl/numeric>
#include <sycl/sycl.hpp>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <iostream>
#include <locale>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <utility>

#include "oneapi/mkl/rng.hpp"

// Initialization value for random number generator
static const int seed = 7777;
// Quantity of samples to check using Students' T-test
static const int n_samples = 1000000;
// Expected mean value of random samples
static const float expected_mean = 0.0f;
// Expected standard deviation of random samples
static const float expected_std_dev = 1.0f;
// T-test threshold which corresponds to 5% significance level and infinite
// degrees of freedom
static const float threshold = 1.95996f;

// Compute mean and 2nd order central moment with oneDPL.
// The central moment is normalized by n:
//   C_2 = 1/n * sum_{j=1}^{n} (x_j - M)^2
// The input must contain n USM elements whose generation has completed.
// At least two observations are required so that the central moment is
// meaningful for the T-test.
template <typename Policy, typename RealType>
std::pair<RealType, RealType> mean_and_central_moment(Policy& policy,
                                                      const RealType* r,
                                                      std::int64_t n) {
  if (n < 2) {
    throw std::invalid_argument(
        "Mean and central moment require at least two observations");
  }
  const RealType mean = oneapi::dpl::reduce(policy, r, r + n, RealType(0)) /
                        static_cast<RealType>(n);

  // Sum squared deviations from the mean (two-pass) to avoid cancellation.
  const RealType squared_deviations = oneapi::dpl::transform_reduce(
      policy, r, r + n, RealType(0), std::plus<RealType>(),
      [=](RealType value) {
        const RealType difference = value - mean;
        return difference * difference;
      });
  // oneDPL reductions are blocking and return the result directly; this wait
  // only forwards any asynchronous errors to the queue's exception handler.
  policy.queue().wait_and_throw();
  return {mean, squared_deviations / static_cast<RealType>(n)};
}

// T-test function with expected mean
// Returns: 1 - in case of NULL hypothesis should be accepted, 0 - in case of
// NULL hypothesis should be rejected. Throws on invalid input.
template <typename Policy, typename RealType>
std::int32_t t_test(Policy& policy, RealType* r, std::int64_t n,
                    RealType expected_mean) {
  std::int32_t res;
  RealType sqrt_n_observations = std::sqrt(static_cast<RealType>(n));

  // Perform computations of mean and variance
  const auto [mean, variance] = mean_and_central_moment(policy, r, n);
  if (!(variance > static_cast<RealType>(0))) {
    throw std::domain_error("T-test is undefined for zero variance");
  }
  // Check the condition
  if ((std::abs(mean - expected_mean) * sqrt_n_observations /
       std::sqrt(variance)) < static_cast<RealType>(threshold)) {
    res = 1;
  } else {
    res = 0;
  }
  return res;
}

// T-test function with two input arrays
// Returns: 1 - in case of NULL hypothesis should be accepted, 0 - in case of
// NULL hypothesis should be rejected. Throws on invalid input.
template <typename Policy, typename RealType>
std::int32_t t_test(Policy& policy, RealType* r1, std::int64_t n1,
                    RealType* r2, std::int64_t n2) {
  std::int32_t res;
  // Perform computations of mean and variance
  const auto [mean1, variance1] = mean_and_central_moment(policy, r1, n1);
  const auto [mean2, variance2] = mean_and_central_moment(policy, r2, n2);
  if (!(variance1 > static_cast<RealType>(0)) ||
      !(variance2 > static_cast<RealType>(0))) {
    throw std::domain_error("T-test is undefined for zero variance");
  }
  // Check the condition
  bool almost_equal =
      (variance1 < 2 * variance2) || (variance2 < 2 * variance1);
  if (almost_equal) {
    if ((std::abs(mean1 - mean2) /
         std::sqrt((static_cast<RealType>(1.0) / static_cast<RealType>(n1) +
                     static_cast<RealType>(1.0) / static_cast<RealType>(n2)) *
                    (static_cast<RealType>(n1 - 1) *
                         static_cast<RealType>(n1 - 1) * variance1 +
                     static_cast<RealType>(n2 - 1) *
                         static_cast<RealType>(n2 - 1) * variance2) /
                    static_cast<RealType>(n1 + n2 - 2))) <
        static_cast<RealType>(threshold)) {
      res = 1;
    } else {
      res = 0;
    }
  } else {
    if ((std::abs(mean1 - mean2) /
         std::sqrt((variance1 + variance2))) <
        static_cast<RealType>(threshold)) {
      res = 1;
    } else {
      res = 0;
    }
  }
  return res;
}

// Parse a command line argument as a value of type T. The value is updated
// only if the whole argument is a valid number that fits into T.
template <typename T>
bool parse_argument(const char* str, T& value) {
  std::istringstream stream(str);
  // Use '.' as decimal separator regardless of the global locale
  stream.imbue(std::locale::classic());
  T parsed{};
  stream >> parsed;
  if (stream.fail() || !(stream >> std::ws).eof()) {
    return false;
  }
  value = parsed;
  return true;
}

int main(int argc, char** argv) {
  std::cout << "Student's T-test Simulation" << std::endl;
  std::cout << "Unified Shared Memory Api" << std::endl;
  std::cout << "-------------------------------------" << std::endl;

  using fp_type = float;
  std::int64_t n_points = n_samples;
  fp_type mean = expected_mean;
  fp_type std_dev = expected_std_dev;

  // Invalid or out-of-range arguments fall back to the default values
  if (argc >= 2) {
    std::int64_t value = 0;
    if (parse_argument(argv[1], value) && value >= 2) {
      n_points = value;
    }
  }

  if (argc >= 3) {
    parse_argument(argv[2], mean);
  }

  if (argc >= 4) {
    fp_type value = 0;
    if (parse_argument(argv[3], value) && value > static_cast<fp_type>(0.0f)) {
      std_dev = value;
    }
  }

  std::cout << "Number of random samples = " << n_points
            << " with mean = " << mean << ", std_dev = " << std_dev << std::endl;

  // This exception handler with catch async exceptions
  auto exception_handler = [](sycl::exception_list exceptions) {
    for (std::exception_ptr const& e : exceptions) {
      try {
        std::rethrow_exception(e);
      } catch (sycl::exception const& e) {
        std::cout << "Caught asynchronous SYCL exception during generation:\n"
                  << e.what() << std::endl;
      }
    }
  };

  std::int32_t res0, res1;

  try {
    // Queue constructor passed exception handler
    sycl::queue q(sycl::default_selector_v, exception_handler);
    // Allocate memory for random output; memory is released automatically,
    // including when an exception is thrown
    auto usm_deleter = [&q](fp_type* ptr) { sycl::free(ptr, q); };
    using usm_ptr = std::unique_ptr<fp_type, decltype(usm_deleter)>;
    usm_ptr rng_arr0_ptr(
        sycl::malloc_shared<fp_type>(static_cast<std::size_t>(n_points), q),
        usm_deleter);
    usm_ptr rng_arr1_ptr(
        sycl::malloc_shared<fp_type>(static_cast<std::size_t>(n_points), q),
        usm_deleter);
    if (!rng_arr0_ptr || !rng_arr1_ptr) {
      throw std::runtime_error("Failed to allocate USM memory");
    }
    fp_type* rng_arr0 = rng_arr0_ptr.get();
    fp_type* rng_arr1 = rng_arr1_ptr.get();
    // Create engine object
    oneapi::mkl::rng::default_engine engine(q, seed);
    // Create distribution object
    oneapi::mkl::rng::gaussian<fp_type> distribution(mean, std_dev);
    // Perform generation
    oneapi::mkl::rng::generate(distribution, engine, n_points, rng_arr0);
    oneapi::mkl::rng::generate(distribution, engine, n_points, rng_arr1);
    q.wait_and_throw();
    // Create oneDPL device policy once and reuse it for all reductions
    auto policy = oneapi::dpl::execution::make_device_policy(q);
    // Launch T-test with expected mean
    res0 = t_test(policy, rng_arr0, n_points, mean);
    // Launch T-test with two input arrays
    res1 = t_test(policy, rng_arr0, n_points, rng_arr1, n_points);
  } catch (const std::exception& e) {
    std::cout << "Failure: " << e.what() << std::endl;
    return 1;
  } catch (...) {
    // Some other exception detected
    std::cout << "Failure" << std::endl;
    std::terminate();
  }

  // Printing results
  std::cout << "T-test result with expected mean: " << res0 << std::endl;
  std::cout << "T-test result with two input arrays: " << res1 << std::endl << std::endl;

  if(res0 != 1 || res1 != 1) {
    std::cout << "TEST FAILED" << std::endl;
    return 1;
  }

  std::cout << "TEST PASSED" << std::endl;
  return 0;
}
