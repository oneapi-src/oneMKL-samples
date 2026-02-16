# `Block LU Decomposition` Sample
Block LU Decomposition shows how to use the Intel® oneAPI Math Kernel Library (oneMKL) BLAS and LAPACK functionality to solve a block tridiagonal linear equation.

## Purpose
Block LU Decomposition consists of two small applications (`factor.cpp` and `solve.cpp`).
The factor.cpp generates a tridiagonal block matrix, then performs a block LU factorization using oneMKL BLAS and LAPACK routines. The solve.cpp application uses this factorization to solve a linear system with the block tridiagonal matrix on the left-hand side.
Both factoring and solving require several oneMKL routines. Some steps can be parallelized, while others must be ordered sequentially. The sample code shows how to inform oneMKL of the existing dependencies between routines using SYCL*-compliant events. This code sample uses pointer-based programming with Unified Shared Memory (USM), allowing individual oneMKL routines to work on submatrices of the original matrices.

This article explains in detail how oneMKL LAPACK routines can be used to solve a system of linear equations with an LU-factored block tridiagonal coefficient matrix: [Solving a system of linear equations with an LU-factored block tridiagonal coefficient matrix](https://www.intel.com/content/www/us/en/docs/onemkl/cookbook/current/slv-sys-lin-eq-lu-factor-blk-tridiag-coeff-mat.html).

## Key Implementation Details
This sample illustrates several important oneMKL routines: matrix multiplication, triangular solves from BLAS (`gemm`, `trsm`), and LU factorization (`getrf`) from LAPACK, as well as several other utility routines.

## Using Visual Studio Code* (Optional)
You can use Visual Studio Code (VS Code) extensions to set your environment, create launch configurations,
and browse and download samples.

The basic steps to build and run a sample using VS Code include:
 - Download a sample using the extension **Code Sample Browser for Intel Software Developer Tools**.
 - Configure the oneAPI environment with the extension **Environment Configurator for Intel Software Developer Tools**.
 - Open a Terminal in VS Code (**Terminal>New Terminal**).
 - Run the sample in the VS Code terminal using the instructions below.
 - (Linux only) Debug your GPU application with GDB for Intel® oneAPI toolkits using the **Generate Launch Configurations** extension.

To learn more about the extensions, see the
[Using Visual Studio Code with Intel® oneAPI Toolkits User Guide](https://www.intel.com/content/www/us/en/develop/documentation/using-vs-code-with-intel-oneapi/top.html).

## Building the Block LU Decomposition Sample

### On a Linux* System
Run `make` to build and run the factor and solve programs. You can remove all generated files with `make clean`.

### On a Windows* System
Run `nmake` to build and run the sample. `nmake clean` removes temporary files.

## Running the Block LU Decomposition Sample

This sample performs its computations on the default SYCL* device. You can set
the `ONEAPI_DEVICE_SELECTOR` environment variable to `"*:cpu"` or `"*:gpu"`
to select the device to use.
To find more information about the variable follow the link:
[ONEAPI_DEVICE_SELECTOR](https://github.com/intel/llvm/blob/sycl/sycl/doc/EnvironmentVariables.md#oneapi_device_selector).

### Example of Output
After building, if everything is working correctly, you will see the output from both the `factor` and `solve` programs. Each includes an accuracy check at the end. The example output below shows a successful run with a very small floating-point error.
```
./factor
Testing the accuracy of LU factorization with pivoting
of randomly generated block tridiagonal matrix
by calculating norm of the residual matrix.
||A - LU||_F/||A||_F = 3.65246e-16

./solve
Testing accuracy of solution of linear equations system
with randomly generated block tridiagonal coefficient
matrix by calculating ratios of residuals
to RHS vectors norms.
max_(i=1,...,nrhs){||ax(i)-f(i)||/||f(i)||} = 6.88457e-13
```

## License

Code samples are licensed under the MIT license. See
[License.txt](License.txt) for details.

## Notices and Disclaimers

© Intel Corporation. Intel, the Intel logo, and other Intel marks are trademarks of Intel Corporation or its subsidiaries. Other names and brands may be claimed as the property of others.
