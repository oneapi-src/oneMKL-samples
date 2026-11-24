# `Student's T-test` Sample
The Student's T-test sample shows how to use the Intel® oneAPI Math Kernel
Library (oneMKL) random number generation and Intel® oneAPI DPC++ Library (oneDPL)
reductions to decide if the null hypothesis should be accepted or rejected.

## Purpose

Student’s t-test, in statistics, a method of testing hypotheses about the mean
of a small sample drawn from a normally distributed population when the
population standard deviation is unknown. It is usually first to formulate a
null hypothesis, which states that there is no effective difference between the
observed sample mean and the hypothesized or stated population mean—i.e., that
any measured difference is due only to chance.

This sample uses oneMKL to produce the random numbers and oneDPL reductions to
compute statistics.

## Key Implementation Details

The student's t-test sample illustrates how to create an RNG engine object (the
source of pseudo-randomness), a distribution object (specifying the desired
probability distribution), and generate the random numbers themselves. After the
numbers are produced, basic statistical properties such as mean and standard
deviation are computed to be processed inside the Student's T-test algorithm.
The mean and the 2nd order central moment are computed using
`oneapi::dpl::reduce` and `oneapi::dpl::transform_reduce` with a device execution
policy bound to the SYCL queue. The algorithms read USM data and return scalar
results. The central moment is the sum of squared deviations from the mean
normalized by `n`, consistent with the definition used by
`oneapi::mkl::stats::central_moment`. This replaces the deprecated oneMKL SYCL
Summary Statistics APIs.

## Building the Student's T-test Sample

The sample requires oneMKL, oneDPL, and a SYCL compiler with C++17 support.
Initialize their environments before building. For a separate oneDPL installation
or source checkout, source `<oneDPL-root>/env/vars.sh` on Linux, or run
`<oneDPL-root>\env\vars.bat` on Windows, to make its headers available.

### Using Visual Studio Code*  (Optional)

You can use Visual Studio Code (VS Code) extensions to set your environment,
create launch configurations, and browse and download samples.

The basic steps to build and run a sample using VS Code include:
 - Download a sample using the extension **Code Sample Browser for Intel Software Developer Tools**.
 - Configure the oneAPI environment with the extension **Environment Configurator for Intel Software Developer Tools**.
 - Open a Terminal in VS Code (**Terminal>New Terminal**).
 - Run the sample in the VS Code terminal using the instructions below.
 - (Linux only) Debug your GPU application with GDB for Intel® oneAPI toolkits using the **Generate Launch Configurations** extension.

To learn more about the extensions, see the
[Using Visual Studio Code with Intel® oneAPI Toolkits User Guide](https://www.intel.com/content/www/us/en/develop/documentation/using-vs-code-with-intel-oneapi/top.html).


### On a Linux* System
Run `make` to build and run the sample. One program (t_test) is generated, which
illustrates the Unified Shared Memory (USM) APIs for random number generation.

You can remove all generated files with `make clean`.

### On a Windows* System
Run `nmake` to build and run the sample. One program (t_test.exe) is generated,
which illustrates the Unified Shared Memory (USM) APIs for random number generation.

You can remove all generated files with `nmake clean`.

## Running the Student's T-test Sample

This sample performs its computations on the default SYCL* device. You can set
the `ONEAPI_DEVICE_SELECTOR` environment variable to `"*:cpu"` or `"*:gpu"`
to select the device to use.
To find more information about the variable follow the link:
[ONEAPI_DEVICE_SELECTOR](https://github.com/intel/llvm/blob/sycl/sycl/doc/EnvironmentVariables.md#oneapi_device_selector).

### Example of Output
If everything is working correctly, after running `make` (`nmake`) you will see
step-by-step output from the example program, providing the decision about
accepting null hypothesis.
```
./t_test

Student's T-test Simulation
Unified Shared Memory Api
-------------------------------------
Number of random samples = 1000000 with mean = 0, std_dev = 1
T-test result with expected mean: 1
T-test result with two input arrays: 1

TEST PASSED
```

## License

Code samples are licensed under the MIT license. See
[License.txt](License.txt) for details.

## Notices and Disclaimers

© Intel Corporation. Intel, the Intel logo, and other Intel marks are trademarks of Intel Corporation or its subsidiaries. Other names and brands may be claimed as the property of others.
