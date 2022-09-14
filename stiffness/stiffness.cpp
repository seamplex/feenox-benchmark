#include <iostream>
#include <benchmark/benchmark.h>

#include "flags.h"

extern "C" {
#include "../feenox/src/version.h"
#include "../feenox/src/feenox.h"
feenox_t feenox;
#include "../feenox/src/parser/parser.h"
extern feenox_parser_t feenox_parser;
} // extern C

static void BM_build_only(benchmark::State& state) {
  for (auto _ : state) {
    feenox_problem_build();
  }
}

static void BM_dirichlet_eval_only(benchmark::State& state) {
  feenox_problem_build();
  for (auto _ : state) {
    feenox_problem_dirichlet_eval();
  }
}

static void BM_dirichlet_set_only(benchmark::State& state) {
  feenox_problem_build();
  feenox_problem_dirichlet_eval();
  for (auto _ : state) {
    feenox_problem_dirichlet_set_K();
  }
}

static void BM_build_and_eval_bc(benchmark::State& state) {
  for (auto _ : state) {
    feenox_problem_build();
    feenox_problem_dirichlet_eval();
  }
}

static void BM_build_eval_and_set_bc(benchmark::State& state) {
  for (auto _ : state) {
    feenox_problem_build();
    feenox_problem_dirichlet_eval();
    feenox_problem_dirichlet_set_K();
  }
}

BENCHMARK(BM_build_only)->Unit(benchmark::kMillisecond)->MinTime(2);
// BENCHMARK(BM_dirichlet_eval_only)->Unit(benchmark::kMillisecond)->MinTime(2);
// BENCHMARK(BM_dirichlet_set_only)->Unit(benchmark::kMillisecond)->MinTime(2);
// BENCHMARK(BM_build_and_eval_bc)->Unit(benchmark::kMillisecond)->MinTime(2);
// BENCHMARK(BM_build_eval_and_set_bc)->Unit(benchmark::kMillisecond)->MinTime(2);


int BM_init_feenox()
{
  // TODO: API general initialization
  feenox_call(feenox_init_special_objects());
  
  // TODO: API
  feenox_mesh_init_special_objects();
  mesh_t *mesh = (mesh_t *)calloc(1, sizeof(mesh_t));
  feenox_define_file("cube.msh", "cube.msh", 0, NULL);
  mesh->file = feenox_get_file_ptr("cube.msh");
  mesh->file->mode = strdup("r");
  mesh->reader = feenox_mesh_read_gmsh;
  feenox.mesh.mesh_main = mesh;
  feenox_instruction_mesh_read(mesh);
  // TODO: API
/*    
  feenox_check_alloc(feenox_parser.line = strdup("READ_MESH cube.msh"));
  feenox_check_alloc(feenox_parser.full_line = strdup(feenox_parser.line));
  feenox_call(feenox_parse_line());
  feenox_free(feenox_parser.full_line);
  feenox_free(feenox_parser.line);
 */

  feenox_check_alloc(feenox_parser.line = strdup("PROBLEM mechanical"));
  feenox_check_alloc(feenox_parser.full_line = strdup(feenox_parser.line));
  feenox_call(feenox_parse_line());
  feenox_free(feenox_parser.full_line);
  feenox_free(feenox_parser.line);
  
  feenox_call(feenox_add_assignment("E", "200e3"));
  feenox_call(feenox_add_assignment("nu", "0.3"));
  instruction_t *ip = NULL;
  LL_FOREACH(feenox.instructions, ip) {
    feenox_call(ip->routine(ip->argument));
  }  
  
  feenox_call(feenox_add_bc_data("left", "fixed"));
  feenox_call(feenox_add_bc_data("right", "p=1"));

  feenox_call(feenox.pde.init_runtime_particular());
  feenox_call(feenox_problem_init_runtime_general());

  return 0;
}

int main(int argc, char** argv) {
  ::benchmark::Initialize(&argc, argv);
  
  benchmark::AddCustomContext("benchmark_compiler_command", BENCHMARK_COMPILER_COMMAND);
  benchmark::AddCustomContext("benchmark_compiler_version", BENCHMARK_COMPILER_VERSION);
  benchmark::AddCustomContext("benchmark_compiler_flags",   BENCHMARK_COMPILER_FLAGS);
  benchmark::AddCustomContext("feenox_compiler_command", FEENOX_COMPILER_SHOW);
  benchmark::AddCustomContext("feenox_compiler_version", FEENOX_COMPILER_VERSION);
  benchmark::AddCustomContext("feenox_compiler_flags",   FEENOX_COMPILER_CFLAGS);

  benchmark::AddCustomContext("feenox_git_version",   FEENOX_GIT_VERSION);
  benchmark::AddCustomContext("feenox_git_branch",    FEENOX_GIT_BRANCH);
  benchmark::AddCustomContext("feenox_git_date",      FEENOX_GIT_DATE);
  benchmark::AddCustomContext("feenox_git_clean",     FEENOX_GIT_CLEAN ? "yes" : "no");

  if (BM_init_feenox() != FEENOX_OK) {
    feenox_pop_errors();
    exit(EXIT_FAILURE);
  }  
  if (::benchmark::ReportUnrecognizedArguments(argc, argv)) return 1;
  ::benchmark::RunSpecifiedBenchmarks();
  ::benchmark::Shutdown();
  return 0;
} 

