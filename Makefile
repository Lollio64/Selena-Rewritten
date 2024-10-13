# Selena Rewritten's build system
# Is it worse then the original's CMake?

# Configurable Settings
TITLE = selena
INCLUDE = include

# Compiler Flags
CFLAGS = -o $(TITLE) -I $(INCLUDE) -O2

# Main Script, do you really have to specify all source files
all: # without having to do some goofy Makefile stuff? Yeah fix this
	@echo "Compiling the source files to an executable..."
	@g++ $(CFLAGS) source/lexer.cpp source/main.cpp source/symbol.cpp

# Clean up build files
clean:
	@echo "Cleaning up build files..."
	@rm -rf build $(TITLE).exe

# Compiling as a library, calls another Makefile
library: # Ugh, fix this 
	@echo "Compiling the source files to a library..."
	@make -f library.mk 