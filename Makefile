# Find all source files in this directory and firter some out.
ALL_CPP_SOURCES = $(shell ls *.cpp)
ALL_CPP_HEADERS = $(shell ls *.hpp)
# Here put excluded files.
EXCLUDED_CPP_SOURCES =
EXCLUDED_CPP_HEADERS =
# Sources and headers to be used.
CPP_SOURCES = $(filter-out $(EXCLUDED_CPP_SOURCES), $(ALL_CPP_SOURCES))
CPP_HEADERS = $(filter-out $(EXCLUDED_CPP_HEADERS), $(ALL_CPP_HEADERS))
# Object files.
OBJECTS = $(patsubst %.cpp,%.o,$(CPP_SOURCES))

OUTPUT_NAME = blockchain_test.bin
CXX_FLAGS = -c --std=c++11
LD_FLAGS = -lpthread

# Main target.
build: $(OUTPUT_NAME)

# Link.
$(OUTPUT_NAME): $(OBJECTS)
	@$(CXX) $^ $(LD_FLAGS) -o $@

# Compile CPP source.
%.o: %.cpp $(CPP_HEADERS)
	@$(CXX) $(CXX_FLAGS) $(basename $@).cpp -o $@

# Clean project.
clean:
	@$(RM) *.o
	@$(RM) *.bin
