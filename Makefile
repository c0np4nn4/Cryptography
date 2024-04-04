# Compiler and Compiler Flags
CXX = g++
CXXFLAGS = -Wall -g

# Target executable name
TARGET = example

# Object files
OBJECTS = example.o prince.o prince_core.o key_expansion.o

# Default rule to build the executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Rule to compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

# Clean rule to delete object files and the executable
clean:
	rm -f $(OBJECTS) $(TARGET) $(OBJECTS:.o=.d)

# Include dependencies
-include $(OBJECTS:.o=.d)

# Rule to generate a dependency file for each source file
%.d: %.cpp
	$(CXX) $(CXXFLAGS) -M $< | sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' > $@

