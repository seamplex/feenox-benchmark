#include <random>

#define SEED 123456

#define INIT_SIGMAS \
  std::default_random_engine rng(SEED); \
  std::uniform_real_distribution<double> distribution(-100.0,+100.0); \
  double sigmax = distribution(rng); \
  double sigmay = distribution(rng); \
  double sigmaz = distribution(rng); \
  double tauxy = distribution(rng);  \
  double tauyz = distribution(rng);  \
  double tauzx = distribution(rng);  \
  double sigma1[1]; \
  double sigma2[1]; \
  double sigma3[1];
  
  
#define COMPUTE_PRINCIPAL_STRESS_FROM_CAUCHY \
  double I1 = sigmax + sigmay + sigmaz; \
  double I2 = sigmax*sigmay + sigmay*sigmaz + sigmaz*sigmax - gsl_pow_2(tauxy) - gsl_pow_2(tauyz) - gsl_pow_2(tauzx); \
  double I3 = sigmax*sigmay*sigmaz - sigmax*gsl_pow_2(tauyz) - sigmay*gsl_pow_2(tauzx) - sigmaz*gsl_pow_2(tauxy) + 2*tauxy*tauyz*tauzx; \
\
  double c1 = sqrt(fabs(gsl_pow_2(I1) - 3*I2)); \
  double phi = 1.0/3.0 * acos((2.0*gsl_pow_3(I1) - 9.0*I1*I2 + 27.0*I3)/(2.0*gsl_pow_3(c1))); \
  if (isnan(phi)) { \
    phi = 0; \
  } \
\
  double c2 = I1/3.0; \
  double c3 = 2.0/3.0 * c1; \
  sigma1[0] = c2 + c3 * cos(phi); \
  sigma2[0] = c2 + c3 * cos(phi - 2.0*M_PI/3.0); \
  sigma3[0] = c2 + c3 * cos(phi - 4.0*M_PI/3.0);
  
#define ARGS_DECL (double sigmax, double sigmay, double sigmaz, double tauxy, double tauyz, double tauzx, double *sigma1, double *sigma2, double *sigma3) 
#define ARGS_CALL (sigmax, sigmay, sigmaz, tauxy, tauyz, tauzx, sigma1, sigma2, sigma3)

#define ABS_TOL 1e-6
#define CHECK(val1,val2) assert(fabs(val1 - val2) < ABS_TOL);
// #define CHECK(val1,val2) if (fabs(val1 - val2) > ABS_TOL) printf("mongocho\n");
#define CHECK_RESULTS    CHECK(sigma1[0], 118.679); CHECK(sigma2[0], -31.9952); CHECK(sigma3[0], -173.651);

#define DO_NOT_OPTIMIZE     benchmark::DoNotOptimize(sigma1[0]); benchmark::DoNotOptimize(sigma2[0]); benchmark::DoNotOptimize(sigma3[0]);
// #define DO_NOT_OPTIMIZE     benchmark::ClobberMemory();  
// #define DO_NOT_OPTIMIZE     benchmark::ClobberMemory(); benchmark::DoNotOptimize(sigma1[0]); benchmark::DoNotOptimize(sigma2[0]); benchmark::DoNotOptimize(sigma3[0]);

// #define VERBOSE
#ifdef VERBOSE
 #define PRINT               printf("%g %g %g\n", sigma1[0], sigma2[0], sigma3[0]);
#else
 #define PRINT
#endif
  
int feenox_principal_stress_from_cauchy_call_cpp_aux ARGS_DECL;
