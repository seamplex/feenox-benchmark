
# CXXFLAGS = -Ofast -flto=auto
# CXXFLAGS = -O3 -flto=auto
# CXXFLAGS = -Ofast
CXXFLAGS = -O3
# CXXFLAGS = -O2
# CXXFLAGS = -O1
# CXXFLAGS = -O0
# CXXFLAGS = -g


TARGET = stiffness
include ../benchmark.make

all: flags $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LDFLAGS)

$(TARGET).o: $(TARGET).cpp Makefile ../benchmark.make ../feenox/src/version.h


.PHONY: clean flags

flags:
	$(SHELL) ../flags.sh '$(CXX)' '$(CXXFLAGS)'

clean:
	rm -f $(TARGET) $(TARGET).o flags.h
