#Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2

#Source files
SRC = $(wildcard *.c Objects/*.c Materials/*.c Textures/*.c Utility/*.c)
OBJ = $(SRC:.c=.o)
CWD = $()

ifeq ($(OS),Windows_NT)
	TARGET = raytracer.exe
else
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
ifeq ($(OS),Windows_NT)
	cmd /C "for /R %%f in (*.o) do del /Q %%f"
	cmd /C del /Q *.o *\*.o $(TARGET)
else
	rm -f $(OBJ) $(TARGET)
endif