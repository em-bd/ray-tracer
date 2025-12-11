#Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2

#Source files
SRC = main.c interval.c vectors.c texture-load.c ray.c color.c utils.c material.c object.c camera.c parser.c
OBJ = $(SRC:.c=.o)

# Detect platform
ifeq ($(OS),Windows_NT)
    RM = del /Q
	TARGET = raytracer.exe
else
    RM = rm -f
	TARGET = raytracer
endif

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Compile source into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up rule
clean:
	$(RM) $(OBJ) $(TARGET)