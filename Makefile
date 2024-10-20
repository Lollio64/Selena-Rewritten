# Selena Rewritten's build system
# Is it worse then the original's CMake?

# Configurable Settings
TITLE = selena
SOURCE = source
INCLUDE = include

# Compiler Flags
CFLAGS = -o $(TITLE) -I $(INCLUDE) -O2 -std=c++17

# Main Script
all: # Wildcard helps my lazy ass
	@echo "Compiling the source files to an executable..."
	@g++ $(CFLAGS) $(wildcard $(SOURCE)/*.cpp)

# Clean up build files
clean:
	@echo "Cleaning up build files..."
	@rm -rf build -rf lib 
	@make -f Makefile.lib remove
	@rm $(wildcard $(INCLUDE)/*.gch) $(TITLE).exe

# Compiling as a library, calls another Makefile
library: # Well it is now fixed...Woohoo!
	@echo "Compiling the source files to a library..."
	@make -f Makefile.lib install

# Compiling the code as a GDB debuggable executable 
debug: # Only difference is the added -g flag
	@echo "Compiling the source files to a debug executable..."
	@g++ $(CFLAGS) -g $(wildcard $(SOURCE)/*.cpp)