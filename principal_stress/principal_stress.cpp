#include <iostream>
#include <benchmark/benchmark.h>

#include "principal_stress.h"
#include "flags.h"

extern "C" {
#include "../feenox/src/feenox.h"
feenox_t feenox;
#include "../feenox/src/version.h"

inline int feenox_principal_stress_from_cauchy_inline ARGS_DECL {
  COMPUTE_PRINCIPAL_STRESS_FROM_CAUCHY;
  return FEENOX_OK;
}

int feenox_principal_stress_from_cauchy_call ARGS_DECL {
  COMPUTE_PRINCIPAL_STRESS_FROM_CAUCHY;
  return FEENOX_OK;
}

int feenox_principal_stress_from_cauchy_wrapper ARGS_DECL {
  feenox_principal_stress_from_cauchy_call ARGS_CALL;
  return FEENOX_OK;
}

int feenox_principal_stress_from_cauchy_wrapper2 ARGS_DECL {
  feenox_principal_stress_from_cauchy_wrapper ARGS_CALL;
  return FEENOX_OK;
}

int feenox_principal_stress_from_cauchy_wrapper3 ARGS_DECL {
  feenox_principal_stress_from_cauchy_wrapper2 ARGS_CALL;
  return FEENOX_OK;
}

void feenox_principal_stress_from_cauchy_void ARGS_DECL {
  COMPUTE_PRINCIPAL_STRESS_FROM_CAUCHY;
}


} // extern C

int feenox_principal_stress_from_cauchy_call_cpp_same ARGS_DECL {
  COMPUTE_PRINCIPAL_STRESS_FROM_CAUCHY;
  return FEENOX_OK;
}


static void BM_principal_stress_feenox(benchmark::State& state) {
  INIT_SIGMAS;
  
  size_t i = 0;
  for (auto _ : state) {
    sigmax = (double)(i++);
    feenox_principal_stress_from_cauchy ARGS_CALL;
    DO_NOT_OPTIMIZE;
  }
  PRINT;
}

static void BM_principal_stress_call(benchmark::State& state) {
  INIT_SIGMAS;
  
  size_t i = 0;
  for (auto _ : state) {
    sigmax = (double)(i++);
    feenox_principal_stress_from_cauchy_call ARGS_CALL;
    DO_NOT_OPTIMIZE;
  }
  PRINT;
}

static void BM_principal_stress_wrapper(benchmark::State& state) {
  INIT_SIGMAS;
  
  size_t i = 0;
  for (auto _ : state) {
    sigmax = (double)(i++);
    feenox_principal_stress_from_cauchy_wrapper ARGS_CALL;
    DO_NOT_OPTIMIZE;
  }
  PRINT;
}

static void BM_principal_stress_wrapper2(benchmark::State& state) {
  INIT_SIGMAS;
  
  size_t i = 0;
  for (auto _ : state) {
    sigmax = (double)(i++);
    feenox_principal_stress_from_cauchy_wrapper2 ARGS_CALL;
    DO_NOT_OPTIMIZE;
  }
  PRINT;
}

static void BM_principal_stress_wrapper3(benchmark::State& state) {
  INIT_SIGMAS;
  
  size_t i = 0;
  for (auto _ : state) {
    sigmax = (double)(i++);
    feenox_principal_stress_from_cauchy_wrapper3 ARGS_CALL;
    DO_NOT_OPTIMIZE;
  }
  PRINT;
}


static void BM_principal_stress_void(benchmark::State& state) {
  INIT_SIGMAS;
  
  size_t i = 0;
  for (auto _ : state) {
    sigmax = (double)(i++);
    feenox_principal_stress_from_cauchy_void ARGS_CALL;
    DO_NOT_OPTIMIZE;
  }
  PRINT;
}

static void BM_principal_stress_call_cpp_same(benchmark::State& state) {
  INIT_SIGMAS;
  
  size_t i = 0;
  for (auto _ : state) {
    sigmax = (double)(i++);
    feenox_principal_stress_from_cauchy_call_cpp_same ARGS_CALL;
    DO_NOT_OPTIMIZE;
  }
  PRINT;
}

static void BM_principal_stress_expanded(benchmark::State& state) {
  INIT_SIGMAS;
  
  size_t i = 0;
  for (auto _ : state) {
    sigmax = (double)(i++);
    COMPUTE_PRINCIPAL_STRESS_FROM_CAUCHY;
    DO_NOT_OPTIMIZE;
  }
  PRINT;
}

static void BM_principal_stress_inline(benchmark::State& state) {
  INIT_SIGMAS;
  
  size_t i = 0;
  for (auto _ : state) {
    sigmax = (double)(i++);
    feenox_principal_stress_from_cauchy_inline ARGS_CALL;
    DO_NOT_OPTIMIZE;
  }
  PRINT;
}

static void BM_principal_stress_inline_optimized_out(benchmark::State& state) {
  INIT_SIGMAS;
  
  size_t i = 0;
  for (auto _ : state) {
    sigmax = (double)(i++);
    feenox_principal_stress_from_cauchy_inline ARGS_CALL;
  }
  PRINT;
}

static void BM_principal_stress_overhead_sigmax_double(benchmark::State& state) {
  INIT_SIGMAS;
  
  double d = 0;
  for (auto _ : state) {
    benchmark::DoNotOptimize(sigmax = (d += 1));
  }
  PRINT;
}

static void BM_principal_stress_overhead_sigmax_int(benchmark::State& state) {
  INIT_SIGMAS;
  
  size_t i = 0;
  for (auto _ : state) {
    benchmark::DoNotOptimize(sigmax = (double)(i++));
  }
  PRINT;
}


BENCHMARK(BM_principal_stress_feenox);
BENCHMARK(BM_principal_stress_call);
BENCHMARK(BM_principal_stress_void);
BENCHMARK(BM_principal_stress_wrapper);
BENCHMARK(BM_principal_stress_wrapper2);
BENCHMARK(BM_principal_stress_wrapper3);
BENCHMARK(BM_principal_stress_call_cpp_same);
BENCHMARK(BM_principal_stress_expanded);
BENCHMARK(BM_principal_stress_inline);
BENCHMARK(BM_principal_stress_inline_optimized_out);
BENCHMARK(BM_principal_stress_overhead_sigmax_double);
BENCHMARK(BM_principal_stress_overhead_sigmax_int);


int main(int argc, char** argv) {
  ::benchmark::Initialize(&argc, argv);
  
  benchmark::AddCustomContext("benchmark_compiler_command", BENCHMARK_COMPILER_COMMAND);
  benchmark::AddCustomContext("benchmark_compiler_version", BENCHMARK_COMPILER_VERSION);
  benchmark::AddCustomContext("benchmark_compiler_flags",   BENCHMARK_COMPILER_FLAGS);
  benchmark::AddCustomContext("feenox_compiler_command", FEENOX_COMPILER_SHOW);
  benchmark::AddCustomContext("feenox_compiler_version", FEENOX_COMPILER_VERSION);
  benchmark::AddCustomContext("feenox_compiler_flags",   FEENOX_COMPILER_CFLAGS);
  
  if (::benchmark::ReportUnrecognizedArguments(argc, argv)) return 1;
  ::benchmark::RunSpecifiedBenchmarks();
  ::benchmark::Shutdown();
  return 0;
} 

