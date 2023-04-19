HESea Cryptography Library
=====================================

HESea is a cutting-edge fully homomorphic encryption (FHE) library that provides developers with a powerful, flexible, and easy-to-use tool for secure computation. Built with state-of-the-art cryptographic techniques and optimized for high performance, HESea is ideal for a wide range of use cases. HESea offers various FHE schemes, including TFHE, CKKS, BGV, BFV, and more, which enable users to perform computations on encrypted data without the need to decrypt it. This ensures that sensitive data remains secure and protected against privacy breaches and security threats. 

The library offers a variety of functions for basic primitive operation(Boolen gate, algebraic addition and multiplication), arithmetic operation(ReLu,Sigmoid) and logical operations(sorting,comparison, extraction).The library is highly optimized with advanced techniques such as ciphertext packing and batching, which minimizes the number of operations required to process large data sets, thereby improving efficiency and performance.

One of the significant advantages of HESea is its ease of use. It provides a simple and intuitive API, making it accessible to both novice and expert users. HESea is also highly efficient, with optimizations for underlying mathematical acceleration techniques such as NTT and FFT, making it an excellent choice for a variety of use cases.

HESea is an open-source library, which means that users can inspect and modify the code as needed. The library also offers comprehensive documentation and support, including tutorials and examples, making it easy for developers to integrate HESea into their projects.

Overall, HESea is a powerful and robust FHE library that offers a wide range of advanced features and optimizations for developers who want to implement secure computation in privacy-preserving machine learning. With its advanced functionalities, ease of use, and high performance, HESea is an ideal choice for developers looking to build secure and efficient applications.

[License Information](LICENSE)


Build Instructions
=====================================

We use CMake to build HESea. The high-level (platform-independent) procedure for building HESEA is as follows (for OS-specific instructions, see the section "Detailed information about building HESEA" at the bottom of this page):

1. Install system prerequisites (if not already installed), including a C++ compiler with OMP support, cmake, make, and autoconf.

2. Clone the HESEA repo to your local machine.

3. Create a directory where the binaries will be built. The typical choice is a subfolder "build". In this case, the commands are:
```
mkdir build
cd build
cmake ..
```

Note that CMake will check for any system dependencies that are needed for the build process. If the CMake build does not complete successfully, please review the error CMake shows at the end. If the error does not go away (even though you installed the dependency), try running "make clean" to clear the CMake cache.

4. If you want to use any external libraries, such as NTL/GMP or tcmalloc, install these libraries.

5. Build HESEA by running the following command (this will take few minutes; using the -j make command-line flag is suggested to speed up the build)
```
make
```
If you want to build only library files or some other subset of HESEA, please review the last paragraph of this page.

After the "make" completes, you should see the HESEA library files in the lib folder, binaries of demos in bin/demo, binaries of benchmarks in bib/benchmark, and binaries for unit tests in the unittest folder.

6. Install HESEA to a system directory (if desired or for production purposes)
```
make install
```
You would probably need to run `sudo make install` unless you are specifying some other install location. You can change the install location by running
`cmake -DCMAKE_INSTALL_PREFIX=/your/path ..`. The header files are placed in the "include/palisade" folder of the specified path, and the binaries of the library
are copied directly to the "lib" folder. For example, if no installation path is provided in Ubuntu (and many other Unix-based OSes), the header and library
binary files will be placed in "/usr/local/include/palisade" and "/usr/local/lib", respectively.

Testing and cleaning the build
-------------------

Run unit tests to make sure all capabilities operate as expected
```
make testall
```

Run sample code to test, e.g.,
```
bin/examples/pke/simple-integers
```

To remove the files built by make, you can execute
```
make clean
```

Example
-------------------
```
example....
```

Supported Operating Systems
--------------------------
HESEA CI continually tests our builds on the following operating systems:

* Ubuntu [18.04] [20.04]
* macOS [Mojave]
* Centos 7
* NVIDIA Xavier [Linux for Tegra 4.2.2]
* MinGW (64-bit) on Windows 10

HESEA users have reported successful operation on the following systems:

* FreeBSD
* Ubuntu [16.04]
* Arch Linux
* Manjaro Linux

Please let us know the results if you have run HESEA any additional systems not listed above.

Detailed information about building HESEA
------------------------------

More detailed steps for some common platforms are provided in the following the articles:

[Instructions for building HESEA in Linux](https://github.com/Privasea/HESEA_Lib/blob/main/doc/Instructions-for-building-HESEA-in-Linux.md)

[Instructions for building HESEA in Windows](https://github.com/Privasea/HESEA_Lib/blob/main/doc/Instructions-for-building-HESEA-in-Windows.md)

[Instructions for building HESEA in macOS](https://github.com/Privasea/HESEA_Lib/blob/main/doc/Instructions-for-building-HESEA-in-macOS.md)

HESEA provides many CMake/make configuration options, such as installing specific modules of the library, compiling only libraries w/o any unit tests and demos, choosing the Debug mode for compilation, turning on/off NTL/GMP. These options are described in detail in the following Wiki article:

[Use of CMake in HESEA](https://github.com/Privasea/HESEA_Lib/blob/main/doc/Use-of-CMake-in-HESEA.md)

[Instructions for building user projects that use HESEA](https://github.com/Privasea/HESEA_Lib/blob/main/doc/Instructions-for-building-user-projects-that-use-HESEA.md)
