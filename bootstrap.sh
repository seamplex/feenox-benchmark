#!/bin/bash

if [ ! -e feenox ]; then
  echo "cloning FeenoX from Github"
  git clone https://www.github.com/seamplex/feenox || exit 1
else
  echo "FeenoX already cloned in feenox"
fi


cd feenox
 if [ ! -e configure ]; then
  echo "bootstrapping FeenoX"
  ./autogen.sh || exit 1
 else
   echo "FeenoX already bootstrapped"
 fi

 if [ ! -e Makefile ]; then
  echo "configuring FeenoX"
  ./configure || exit 1
 else
  echo "FeenoX already configured" 
 fi

 if [ ! -e src/feenox-feenox.o ]; then
  echo "compiling FeenoX"
  make || exit 1
 else
  echo "FeenoX already compiled"
 fi

 if [ ! -e ../benchmark.make ]; then
  echo "creating benchmark.make"
  cat << EOF > ../benchmark.make
include ../feenox/src/variables.mak

# this CFLAGS is for reference only
# $(grep "^CFLAGS =" src/Makefile)
$(grep "^DEFS =" src/Makefile)
$(grep "^LIBS =" src/Makefile)
$(grep "^DOWNLOADED_GSL_INCLUDES =" src/Makefile)
$(grep "^DOWNLOADED_GSL_LIBS =" src/Makefile)

CXXFLAGS += \$(DEFS) \$(SLEPC_CC_INCLUDES) \$(PETSC_CC_INCLUDES) \$(DOWNLOADED_GSL_INCLUDES)
LDFLAGS += -lbenchmark \$(SLEPC_LIB) \$(PETSC_LIB) \$(LIBS) \$(DOWNLOADED_GSL_LIBS)

OBJ = \$(TARGET).o $(find -L ../feenox/src -name "*.o" | grep -v feenox-feenox | xargs)
EOF
 else
  echo "benchmark.make already created"
 fi
cd ..
