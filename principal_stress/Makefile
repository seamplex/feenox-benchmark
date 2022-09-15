
CXXFLAGS = -Ofast -flto=auto
# CXXFLAGS = -O3 -flto=auto
# CXXFLAGS = -Ofast
# CXXFLAGS = -O3
# CXXFLAGS = -O2
# CXXFLAGS = -O1
# CXXFLAGS = -O0
# CXXFLAGS = -g

# read the flags from the command line (see run.sh)
# CXXFLAGS = $(OPT)

TARGET = principal_stress
include ../benchmark.make

# CXXFLAGS += -I/home/gtheler/codigos/benchmark/include
# LDFLAGS = /home/gtheler/codigos/benchmark/build/src/libbenchmark.a $(SLEPC_LIB) $(PETSC_LIB) $(LIBS) $(DOWNLOADED_GSL_LIBS)

all: flags $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LDFLAGS)

$(TARGET).o: $(TARGET).cpp $(TARGET).h Makefile ../benchmark.make


.PHONY: clean flags

flags:
	$(SHELL) ../flags.sh "$(CXX)" "$(CXXFLAGS)"

clean:
	rm -f $(TARGET) $(TARGET).o flags.h link*
