
CXXFLAGS = -Ofast -flto=auto
# CXXFLAGS = -O3 -flto=auto
# CXXFLAGS = -Ofast
# CXXFLAGS = -O3
# CXXFLAGS = -O2
# CXXFLAGS = -O1
# CXXFLAGS = -O0
# CXXFLAGS = -g


TARGET = principal_stress
include ../benchmark.make

all: flags $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LDFLAGS)

$(TARGET).o: $(TARGET).cpp $(TARGET).h Makefile ../benchmark.make


.PHONY: clean flags

flags:
	$(SHELL) ../flags.sh "$(CXX)" "$(CXXFLAGS)"

clean:
	rm -f $(TARGET) $(TARGET).o flags.h link*
