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

All the following runs use FeenoX compiled with GCC and `CFLAGS=-Ofast -flto=auto` (see `feenox_compiler_flags` in the outputs below).

 1. No optimization in the benchmark's `Makefile`, i.e. `CXXFLAGS=-O0`. The call to FeenoX' code is faster because it was compiled with `-Ofast` and the others use `-O0`:

    ```
    benchmark_compiler_command: g++ -Wl,-z,relro -I/usr/include/x86_64-linux-gnu/mpich -L/usr/lib/x86_64-linux-gnu -lmpichcxx -lmpich
    benchmark_compiler_flags: 
    benchmark_compiler_version: g++ (Debian 12.2.0-1) 12.2.0
    feenox_compiler_command: gcc -Wl,-z,relro -I/usr/include/x86_64-linux-gnu/mpich -L/usr/lib/x86_64-linux-gnu -lmpich
    feenox_compiler_flags: -Ofast -flto=auto
    feenox_compiler_version: gcc (Debian 12.2.0-1) 12.2.0
    -------------------------------------------------------------------------------------
    Benchmark                                           Time             CPU   Iterations
    -------------------------------------------------------------------------------------
    BM_principal_stress_feenox                       72.3 ns         72.3 ns      7829337
    BM_principal_stress_call                         96.8 ns         96.8 ns      7282039
    BM_principal_stress_void                         98.4 ns         98.4 ns      7078540
    BM_principal_stress_wrapper                       104 ns          104 ns      6755073
    BM_principal_stress_wrapper2                      106 ns          106 ns      6468864
    BM_principal_stress_wrapper3                      112 ns          112 ns      6331257
    BM_principal_stress_call_cpp_same                97.0 ns         97.0 ns      7289604
    BM_principal_stress_expanded                     94.0 ns         93.9 ns      7203169
    BM_principal_stress_inline                       97.5 ns         97.5 ns      7039346
    BM_principal_stress_inline_optimized_out         95.7 ns         95.7 ns      7138072
    BM_principal_stress_overhead_sigmax_double       2.38 ns         2.38 ns    290591419
    BM_principal_stress_overhead_sigmax_int          1.88 ns         1.88 ns    372357007
    ```

 2. Intermediate optimization `-O2` in the benchmark. Now the other calls are slightly faster but still slower than FeenoX:

    ```
    benchmark_compiler_command: g++ -Wl,-z,relro -I/usr/include/x86_64-linux-gnu/mpich -L/usr/lib/x86_64-linux-gnu -lmpichcxx -lmpich
    benchmark_compiler_flags: -O2
    benchmark_compiler_version: g++ (Debian 12.2.0-1) 12.2.0
    feenox_compiler_command: gcc -Wl,-z,relro -I/usr/include/x86_64-linux-gnu/mpich -L/usr/lib/x86_64-linux-gnu -lmpich
    feenox_compiler_flags: -Ofast -flto=auto
    feenox_compiler_version: gcc (Debian 12.2.0-1) 12.2.0
    -------------------------------------------------------------------------------------
    Benchmark                                           Time             CPU   Iterations
    -------------------------------------------------------------------------------------
    BM_principal_stress_feenox                       70.1 ns         70.1 ns      8231293
    BM_principal_stress_call                         74.2 ns         74.2 ns      9106319
    BM_principal_stress_void                         73.1 ns         73.1 ns      9528729
    BM_principal_stress_wrapper                      73.9 ns         73.9 ns      9129438
    BM_principal_stress_wrapper2                     73.7 ns         73.7 ns      9572405
    BM_principal_stress_wrapper3                     74.2 ns         74.2 ns      9401638
    BM_principal_stress_call_cpp_same                73.4 ns         73.4 ns      9507737
    BM_principal_stress_expanded                     72.1 ns         72.1 ns      9356419
    BM_principal_stress_inline                       72.4 ns         72.4 ns      9626681
    BM_principal_stress_inline_optimized_out         26.0 ns         26.0 ns     27283808
    BM_principal_stress_overhead_sigmax_double       1.05 ns         1.05 ns    673686331
    BM_principal_stress_overhead_sigmax_int         0.522 ns        0.522 ns   1000000000
    ```

 3. Level-three optimization `-O3` in the benchmark. Closer but FeenoX is still faster, even though some of the benchmark calls can be inlined while the call to FeenoX cannot:

    ```
    benchmark_compiler_command: g++ -Wl,-z,relro -I/usr/include/x86_64-linux-gnu/mpich -L/usr/lib/x86_64-linux-gnu -lmpichcxx -lmpich
    benchmark_compiler_flags: -O3
    benchmark_compiler_version: g++ (Debian 12.2.0-1) 12.2.0
    feenox_compiler_command: gcc -Wl,-z,relro -I/usr/include/x86_64-linux-gnu/mpich -L/usr/lib/x86_64-linux-gnu -lmpich
    feenox_compiler_flags: -Ofast -flto=auto
    feenox_compiler_version: gcc (Debian 12.2.0-1) 12.2.0
    -------------------------------------------------------------------------------------
    Benchmark                                           Time             CPU   Iterations
    -------------------------------------------------------------------------------------
    BM_principal_stress_feenox                       70.8 ns         70.8 ns      8061182
    BM_principal_stress_call                         72.2 ns         72.2 ns      9700876
    BM_principal_stress_void                         72.7 ns         72.7 ns      9441510
    BM_principal_stress_wrapper                      72.6 ns         72.6 ns      9749349
    BM_principal_stress_wrapper2                     72.9 ns         72.9 ns      9295160
    BM_principal_stress_wrapper3                     72.9 ns         72.9 ns      9566969
    BM_principal_stress_call_cpp_same                72.3 ns         72.3 ns      9220847
    BM_principal_stress_expanded                     72.4 ns         72.4 ns      9672055
    BM_principal_stress_inline                       72.7 ns         72.7 ns      9553364
    BM_principal_stress_inline_optimized_out         26.0 ns         26.0 ns     26423158
    BM_principal_stress_overhead_sigmax_double       1.07 ns         1.06 ns    661818956
    BM_principal_stress_overhead_sigmax_int         0.522 ns        0.522 ns   1000000000
    ```

 4. Fast optimization `-Ofast` in the benchmark. Now all the calls in the benchmark are faster because all of them are inlined while the call to FeenoX is not inlined.
 
    ```
    benchmark_compiler_command: g++ -Wl,-z,relro -I/usr/include/x86_64-linux-gnu/mpich -L/usr/lib/x86_64-linux-gnu -lmpichcxx -lmpich
    benchmark_compiler_flags: -Ofast
    benchmark_compiler_version: g++ (Debian 12.2.0-1) 12.2.0
    feenox_compiler_command: gcc -Wl,-z,relro -I/usr/include/x86_64-linux-gnu/mpich -L/usr/lib/x86_64-linux-gnu -lmpich
    feenox_compiler_flags: -Ofast -flto=auto
    feenox_compiler_version: gcc (Debian 12.2.0-1) 12.2.0
    -------------------------------------------------------------------------------------
    Benchmark                                           Time             CPU   Iterations
    -------------------------------------------------------------------------------------
    BM_principal_stress_feenox                       70.0 ns         70.0 ns      8231407
    BM_principal_stress_call                         66.4 ns         66.4 ns     10278649
    BM_principal_stress_void                         66.0 ns         66.0 ns     10674543
    BM_principal_stress_wrapper                      66.2 ns         66.2 ns     10429742
    BM_principal_stress_wrapper2                     65.9 ns         65.9 ns     10682682
    BM_principal_stress_wrapper3                     66.4 ns         66.4 ns     10319174
    BM_principal_stress_call_cpp_same                66.4 ns         66.4 ns     10632631
    BM_principal_stress_expanded                     66.0 ns         66.0 ns     10160912
    BM_principal_stress_inline                       66.2 ns         66.2 ns     10610910
    BM_principal_stress_inline_optimized_out        0.000 ns        0.000 ns   1000000000
    BM_principal_stress_overhead_sigmax_double       1.06 ns         1.06 ns    661165736
    BM_principal_stress_overhead_sigmax_int         0.524 ns        0.524 ns   1000000000
    ```

 5. Level-three optimization and link-time optimization `-O3 -flto` in the benchmark. FeenoX is slightly faster but the call does not seem to be inlined automatically, i.e. the effect of `-flto` is not obvious.
 
    ```
    benchmark_compiler_command: g++ -Wl,-z,relro -I/usr/include/x86_64-linux-gnu/mpich -L/usr/lib/x86_64-linux-gnu -lmpichcxx -lmpich
    benchmark_compiler_flags: -flto=auto -O3
    benchmark_compiler_version: g++ (Debian 12.2.0-1) 12.2.0
    feenox_compiler_command: gcc -Wl,-z,relro -I/usr/include/x86_64-linux-gnu/mpich -L/usr/lib/x86_64-linux-gnu -lmpich
    feenox_compiler_flags: -Ofast -flto=auto
    feenox_compiler_version: gcc (Debian 12.2.0-1) 12.2.0
    -------------------------------------------------------------------------------------
    Benchmark                                           Time             CPU   Iterations
    -------------------------------------------------------------------------------------
    BM_principal_stress_feenox                       71.2 ns         71.2 ns      8141022
    BM_principal_stress_call                         71.8 ns         71.8 ns      9698680
    BM_principal_stress_void                         72.9 ns         72.9 ns      9588978
    BM_principal_stress_wrapper                      72.4 ns         72.4 ns      9652976
    BM_principal_stress_wrapper2                     71.4 ns         71.4 ns      9351013
    BM_principal_stress_wrapper3                     72.5 ns         72.5 ns      9801144
    BM_principal_stress_call_cpp_same                72.2 ns         72.2 ns      9520815
    BM_principal_stress_expanded                     73.1 ns         73.1 ns      9768862
    BM_principal_stress_inline                       72.0 ns         72.0 ns      9721819
    BM_principal_stress_inline_optimized_out         26.2 ns         26.2 ns     26201980
    BM_principal_stress_overhead_sigmax_double       1.06 ns         1.06 ns    648864982
    BM_principal_stress_overhead_sigmax_int         0.524 ns        0.524 ns   1000000000
    ```

 6. Fast optimization and link-time optimization `-Ofast -flto` in the benchmark. Now the call to FeenoX is equivalent to the inlined and fast-optimized code within the benchmark.
 
    ```
    benchmark_compiler_command: g++ -Wl,-z,relro -I/usr/include/x86_64-linux-gnu/mpich -L/usr/lib/x86_64-linux-gnu -lmpichcxx -lmpich
    benchmark_compiler_flags: -flto=auto -Ofast
    benchmark_compiler_version: g++ (Debian 12.2.0-1) 12.2.0
    feenox_compiler_command: gcc -Wl,-z,relro -I/usr/include/x86_64-linux-gnu/mpich -L/usr/lib/x86_64-linux-gnu -lmpich
    feenox_compiler_flags: -Ofast -flto=auto
    feenox_compiler_version: gcc (Debian 12.2.0-1) 12.2.0
    -------------------------------------------------------------------------------------
    Benchmark                                           Time             CPU   Iterations
    -------------------------------------------------------------------------------------
    BM_principal_stress_feenox                       66.7 ns         66.7 ns      8693376
    BM_principal_stress_call                         66.0 ns         66.0 ns     10472553
    BM_principal_stress_void                         66.1 ns         66.1 ns     10317352
    BM_principal_stress_wrapper                      66.1 ns         66.1 ns     10133735
    BM_principal_stress_wrapper2                     66.5 ns         66.4 ns     10577702
    BM_principal_stress_wrapper3                     66.2 ns         66.1 ns     10156528
    BM_principal_stress_call_cpp_same                66.5 ns         66.5 ns     10597228
    BM_principal_stress_expanded                     66.7 ns         66.6 ns     10326400
    BM_principal_stress_inline                       66.7 ns         66.6 ns     10601185
    BM_principal_stress_inline_optimized_out        0.000 ns        0.000 ns   1000000000
    BM_principal_stress_overhead_sigmax_double       1.05 ns         1.05 ns    667892504
    BM_principal_stress_overhead_sigmax_int         0.519 ns        0.519 ns   1000000000
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
