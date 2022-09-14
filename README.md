This repository contains tools to benchmark the finite-element(ish) tool [FeenoX](https://www.seamplex.com) with Google’s [Benchmark](https://github.com/google/benchmark/) micro-benchmarking library.


# Pre-requisites

You'll need the [Google Benchmark](https://github.com/google/benchmark) library (and headers).
Luckily it is on Debian's (and probably Ubuntu's) repositories:

```
sudo apt-get install libbenchmark-dev
```

You'll also need to have everything needed in order to compile FeenoX sources.
See <https://www.seamplex.com/feenox/doc/compilation.html>.


# Bootstrap

The benchmarks use actual FeenoX code, so you need to have FeenoX configured and compile in a subdirectory called `feenox`. 
There is a script `bootstrap.sh` that will perform all the steps with the default options, which should be enough to start.
In any case, these are the steps

 1. Clone FeenoX from Github
 2. Bootstrap FeenoX
 3. Configure FeenoX
 4. Compile FeenoX
 5. Create a `benchmark.make` file out of FeenoX’ makefiles

## Basic bootstrapping

Run `./bootstrap.sh` to get everything set up (with the default options).

## Advanced bootstrapping

You can repeat the steps above and tweak a little bit the set up:

 1. Clone FeenoX from Github: either through https or ssh
  
    ```terminal
    git clone https://www.github.com/seamplex/feenox
    ```
    
 2. Bootstrap FeenoX: run `autogen.sh`, this will call `autogen.sh` and `make clean` under the hood so everything will start from scratch
 
    ```terminal
    cd feenox
    ./autogen.sh
    ```
 
 3. Configure FeenoX: run `configure.sh`, optionally changing the flags and/or the compiler, e.g.
 
    ```terminal
    ./configure CFLAGS="-O3 -flto" MPICH_CC="clang"
    ```
    
    Make sure the PETSc/SLEPc architecture is no-debug!
 
 4. Compile FeenoX: call `make` in parallel
 
    ```terminal
    make -j6
    ```
 
 5. Create a `benchmark.make` file out of FeenoX’ makefiles: call `bootstrap.sh` after all the other steps have been performed
 
    ```terminal
    cd ..
    ./bootstrap.sh
    ```


 
# Compile and run existing benchmarks

The procedure to compile an existing benchmark is

 1. Go to the benchmark directory
 2. Check and/or edit the `Makefile` to customize the benchmark’s compilation flags. Note that the benchmark is C++ while FeenoX is C, so mind the diffence between `CXXFLAGS` and `CFLAGS`.
 3. Run `make`
 4. Run the benchmark executable

Ideally each benchmark should show some custom context with the compiler and flags used for both FeenoX and the benchmark itself:

```
benchmark_compiler_command: g++ -Wl,-z,relro -I/usr/include/x86_64-linux-gnu/mpich -L/usr/lib/x86_64-linux-gnu -lmpichcxx -lmpich
benchmark_compiler_flags: -O2
benchmark_compiler_version: g++ (Debian 12.2.0-1) 12.2.0
feenox_compiler_command: gcc -Wl,-z,relro -I/usr/include/x86_64-linux-gnu/mpich -L/usr/lib/x86_64-linux-gnu -lmpich
feenox_compiler_flags: -Ofast -flto=auto
feenox_compiler_version: gcc (Debian 12.2.0-1) 12.2.0
```
 
## Principal stresses

This benchmark uses only one call to FeenoX, namely `feenox_principal_stress_from_cauchy()`.
The other lines do the same job in different ways. 

```terminal
cd principal_stress
make
./principal_stress
```

No optimization:

```
benchmark_compiler_command: g++ -Wl,-z,relro -I/usr/include/x86_64-linux-gnu/mpich -L/usr/lib/x86_64-linux-gnu -lmpichcxx -lmpich
benchmark_compiler_flags: 
benchmark_compiler_version: g++ (Debian 12.2.0-1) 12.2.0
feenox_compiler_command: gcc -Wl,-z,relro -I/usr/include/x86_64-linux-gnu/mpich -L/usr/lib/x86_64-linux-gnu -lmpich
feenox_compiler_flags: -Ofast -flto=auto
feenox_compiler_version: gcc (Debian 12.2.0-1) 12.2.0
-----------------------------------------------------------------------------------
Benchmark                                         Time             CPU   Iterations
-----------------------------------------------------------------------------------
BM_principal_stress_feenox                     49.1 ns         49.1 ns     14093997
BM_principal_stress_call                       76.1 ns         76.1 ns      9012373
BM_principal_stress_void                       76.0 ns         76.0 ns      9198482
BM_principal_stress_wrapper                    79.9 ns         79.9 ns      8343373
BM_principal_stress_wrapper2                   85.4 ns         85.4 ns      7921801
BM_principal_stress_wrapper3                   91.0 ns         91.0 ns      7658558
BM_principal_stress_call_cpp_same              76.0 ns         76.0 ns      9309241
BM_principal_stress_expanded                   72.2 ns         72.2 ns      9715853
BM_principal_stress_inline                     75.9 ns         75.9 ns      9358058
BM_principal_stress_inline_optimized_out       74.5 ns         74.5 ns      9517478

```

Intermediate optimization (`-O2`  in the benchmark):

```
benchmark_compiler_command: g++ -Wl,-z,relro -I/usr/include/x86_64-linux-gnu/mpich -L/usr/lib/x86_64-linux-gnu -lmpichcxx -lmpich
benchmark_compiler_flags: -O2
benchmark_compiler_version: g++ (Debian 12.2.0-1) 12.2.0
feenox_compiler_command: gcc -Wl,-z,relro -I/usr/include/x86_64-linux-gnu/mpich -L/usr/lib/x86_64-linux-gnu -lmpich
feenox_compiler_flags: -Ofast -flto=auto
feenox_compiler_version: gcc (Debian 12.2.0-1) 12.2.0
-----------------------------------------------------------------------------------
Benchmark                                         Time             CPU   Iterations
-----------------------------------------------------------------------------------
BM_principal_stress_feenox                     49.0 ns         49.0 ns     14670628
BM_principal_stress_call                       52.4 ns         52.4 ns     13407588
BM_principal_stress_void                       51.7 ns         51.7 ns     13323554
BM_principal_stress_wrapper                    51.5 ns         51.5 ns     13611282
BM_principal_stress_wrapper2                   51.4 ns         51.4 ns     13819114
BM_principal_stress_wrapper3                   51.9 ns         51.9 ns     13530845
BM_principal_stress_call_cpp_same              57.8 ns         57.8 ns     13552037
BM_principal_stress_expanded                  0.319 ns        0.319 ns   1000000000
BM_principal_stress_inline                    0.271 ns        0.271 ns   1000000000
BM_principal_stress_inline_optimized_out      0.000 ns        0.000 ns   1000000000
```

Full optimization (including LTO):

```
benchmark_compiler_command: g++ -Wl,-z,relro -I/usr/include/x86_64-linux-gnu/mpich -L/usr/lib/x86_64-linux-gnu -lmpichcxx -lmpich
benchmark_compiler_flags: -flto=auto -Ofast
benchmark_compiler_version: g++ (Debian 12.2.0-1) 12.2.0
feenox_compiler_command: gcc -Wl,-z,relro -I/usr/include/x86_64-linux-gnu/mpich -L/usr/lib/x86_64-linux-gnu -lmpich
feenox_compiler_flags: -Ofast -flto=auto
feenox_compiler_version: gcc (Debian 12.2.0-1) 12.2.0
-----------------------------------------------------------------------------------
Benchmark                                         Time             CPU   Iterations
-----------------------------------------------------------------------------------
BM_principal_stress_feenox                    0.275 ns        0.274 ns   1000000000
BM_principal_stress_call                      0.267 ns        0.267 ns   1000000000
BM_principal_stress_void                      0.269 ns        0.269 ns   1000000000
BM_principal_stress_wrapper                   0.269 ns        0.269 ns   1000000000
BM_principal_stress_wrapper2                  0.271 ns        0.271 ns   1000000000
BM_principal_stress_wrapper3                  0.263 ns        0.263 ns   1000000000
BM_principal_stress_call_cpp_same             0.268 ns        0.268 ns   1000000000
BM_principal_stress_expanded                  0.265 ns        0.265 ns   1000000000
BM_principal_stress_inline                    0.263 ns        0.263 ns   1000000000
BM_principal_stress_inline_optimized_out      0.000 ns        0.000 ns   1000000000
```


## Stifness matrix

This benchmark measures the time neded for FeenoX to build a mechanical stiffness matrix with a call to `feenox_problem_build()`. This case is slightly more complex because an actual mechanical problem has to be set up, including

 1. reading the mesh
 2. setting the material properties
 3. setting the boundary conditions

Using GCC: 
 
```
benchmark_compiler_command: g++ -Wl,-z,relro -I/usr/include/x86_64-linux-gnu/mpich -L/usr/lib/x86_64-linux-gnu -lmpichcxx -lmpich
benchmark_compiler_flags: -O3
benchmark_compiler_version: g++ (Debian 12.2.0-1) 12.2.0
feenox_compiler_command: gcc -Wl,-z,relro -I/usr/include/x86_64-linux-gnu/mpich -L/usr/lib/x86_64-linux-gnu -lmpich
feenox_compiler_flags: -Ofast -flto=auto
feenox_compiler_version: gcc (Debian 12.2.0-1) 12.2.0
feenox_git_branch: main
feenox_git_clean: yes
feenox_git_date: Wed Sep 14 08:03:40 2022 -0300
feenox_git_version: v0.2.129-g8234f97
***WARNING*** Library was built as DEBUG. Timings may be affected.
-----------------------------------------------------------------------
Benchmark                             Time             CPU   Iterations
-----------------------------------------------------------------------
BM_build_only/min_time:2.000       8.23 ms         8.23 ms          319
```
 
Using Clang:

```
benchmark_compiler_command: clang++ -Wl,-z,relro -I/usr/include/x86_64-linux-gnu/mpich -L/usr/lib/x86_64-linux-gnu -lmpichcxx -lmpich
benchmark_compiler_flags: -O3
benchmark_compiler_version: Debian clang version 14.0.6-2
feenox_compiler_command: clang -Wl,-z,relro -I/usr/include/x86_64-linux-gnu/mpich -L/usr/lib/x86_64-linux-gnu -lmpich
feenox_compiler_flags: -O3
feenox_compiler_version: Debian clang version 14.0.6-2
feenox_git_branch: main
feenox_git_clean: yes
feenox_git_date: Wed Sep 14 08:03:40 2022 -0300
feenox_git_version: v0.2.129-g8234f97
***WARNING*** Library was built as DEBUG. Timings may be affected.
-----------------------------------------------------------------------
Benchmark                             Time             CPU   Iterations
-----------------------------------------------------------------------
BM_build_only/min_time:2.000       8.19 ms         8.19 ms          340
```
 

# Add new benchmarks


# Misc


## Disabling CPU Frequency Scaling

> https://github.com/google/benchmark/blob/main/docs/user_guide.md#disabling-cpu-frequency-scaling


If you see this error:

```
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
```

you might want to disable the CPU frequency scaling while running the
benchmark.  Exactly how to do this depends on the Linux distribution,
desktop environment, and installed programs.  Specific details are a moving
target, so we will not attempt to exhaustively document them here.

One simple option is to use the `cpupower` program to change the
performance governor to "performance".  This tool is maintained along with
the Linux kernel and provided by your distribution.

It must be run as root, like this:

```bash
sudo cpupower frequency-set --governor performance
```

After this you can verify that all CPUs are using the performance governor
by running this command:

```bash
cpupower frequency-info -o proc
```

The benchmarks you subsequently run will have less variance.

Note that changing the governor in this way will not persist across
reboots.  To set the governor back, run the first command again with the
governor your system usually runs with, which varies.

If you find yourself doing this often, there are probably better options
than running the commands above.  Some approaches allow you to do this
without root access, or by using a GUI, etc.  The Arch Wiki [Cpu frequency
scaling](https://wiki.archlinux.org/title/CPU_frequency_scaling) page is a
good place to start looking for options.

## Library compiled with debug

```
CXXFLAGS += -I/home/gtheler/codigos/benchmark/include $(DEFS) $(SLEPC_CC_INCLUDES) $(PETSC_CC_INCLUDES) $(DOWNLOADED_GSL_INCLUDES)
LDFLAGS += /home/gtheler/codigos/benchmark/build/src/libbenchmark.a $(SLEPC_LIB) $(PETSC_LIB) $(LIBS) $(DOWNLOADED_GSL_LIBS)
```
