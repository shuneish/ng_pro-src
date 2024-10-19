# Compiler
CC = gcc  # Use GCC as the compiler

# Compiler options
CFLAGS = -Wall -g  # Enable all warnings and include debug information

# Source files
SRC = scan.c id-list.c main.c  # List of source files

# Header files (dependencies)
HEADERS = scan.h  # List of header files that object files depend on

# Output executable name
TARGET = tc  # The name of the final executable

# Object files (replace .c extensions with .o)
OBJ = $(SRC:.c=.o)

# Default target (run 'make' without arguments)
all: $(TARGET)

# Rule to create the final executable from object files
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^  # $@ is the target (tc), $^ is all dependencies (object files)

# Rule to create .o files from .c files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@  # $< is the first prerequisite (source file), $@ is the target (object file)

# Clean up object files and executable
clean:
	rm -f $(OBJ) $(TARGET)

# Rebuild the project (clean and then build everything again)
rebuild: clean all
