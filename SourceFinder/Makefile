CC = g++
CFLAGS = -Wall -std=c++17 \
         -I/opt/homebrew/include \
         -I/opt/homebrew/opt/pytorch/include \
         -I/opt/homebrew/opt/pytorch/include/torch/csrc/api/include \
         `python3-config --includes`

LDFLAGS = -L/opt/homebrew/lib \
          -L/opt/homebrew/opt/pytorch/lib \
          -L/opt/homebrew/opt/python@3.13/Frameworks/Python.framework/Versions/3.13/lib \
          -ltorch -ltorch_cpu -lc10 -lpython3.13 -ldl -framework CoreFoundation

# Ensure runtime loader can find the libraries
export DYLD_LIBRARY_PATH=/opt/homebrew/opt/pytorch/lib:$DYLD_LIBRARY_PATH

# Directories
EXEC_DIR = src/executables
CLASS_DIR = src/classFiles

# Manually list executables
TARGETS = DataPrep TrainModel # Add more executables here if needed

# Class files that need to be compiled into objects
CLASS_SOURCES = $(wildcard $(CLASS_DIR)/*.cpp)
CLASS_OBJECTS = $(CLASS_SOURCES:.cpp=.o)

# Default target: Show available executables
all:
	@echo "Specify an executable to build, e.g., 'make DataPrep'."
	@echo "Available targets: $(TARGETS)"

# Compile each executable manually
DataPrep: $(EXEC_DIR)/DataPrep.cpp $(CLASS_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

TrainModel: $(EXEC_DIR)/TrainModel.cpp $(CLASS_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Add other executables as needed:
# SomeOtherExecutable: $(EXEC_DIR)/SomeOtherExecutable.cpp $(CLASS_OBJECTS)
#     $(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compile object files for class files
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Clean all compiled files
clean:
	rm -f $(TARGETS) $(CLASS_OBJECTS)
