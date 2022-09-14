if [ -z "$2" ]; then
  echo "usage: $0 CXX CXXFLAGS"
  exit 1
fi

# compiler command
if [ "x${1}" = "xmpicxx" ]; then
  show=$(${1} -show)
else
  show=${1}
fi

# unique flags without includes
flags=$(echo ${2} | tr " " "\n" | grep -v '\-I' | grep -v '\-DHAVE' | grep -v '\-DPACKAGE' | grep -v '\-DSTDC' | grep -v '\-DVERSION' | grep -v ^v | sort | uniq | xargs)

cat << EOF > flags.h
#define BENCHMARK_COMPILER_COMMAND     "${show}"
#define BENCHMARK_COMPILER_VERSION     "$(${1} --version | head -n1)"
#define BENCHMARK_COMPILER_FLAGS       "${flags}"
EOF
