# Selena Rewritten's build system
# Is it worse then the original's CMake?

# Configurable Settings
TITLE = selena
INCLUDE = include

# Compiler Flags
CFLAGS = -o $(TITLE) -I $(INCLUDE)

# Main Script, do you really have to specify all source files
all: # without having to do some goofy Makefile stuff? Yeah fix this
	@g++ $(CFLAGS) source/lexer.cpp source/main.cpp source/symbol.cpp

# Clean up build files, like executables and build folders
clean:
	@echo "Cleaning up build files..."
	@rm -rf build $(TITLE).exe	