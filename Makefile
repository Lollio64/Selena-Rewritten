# Configurable Settings
TITLE = selena
INCLUDE = include/

# Compiler Flags
CFLAGS = -o $(TITLE)  -I $(INCLUDE) 

# Main Script
all: lexer.cpp main.cpp symbol.cpp
    g++ $(CFLAGS) lexer.cpp main.cpp symbol.cpp