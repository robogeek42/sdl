# the compiler: gcc for C program, define as g++ for C++
CC = clang

# compiler flags:
#  -g     - this flag adds debugging information to the executable file
#  -Wall  - this flag is used to turn on most compiler warnings
CFLAGS  = -g -Wall

LDLIBS = -lSDL2
INCLUDE = -I /usr/include/SDL2

# The build target 
TARGET = hello

all: $(TARGET)

$(TARGET): $(TARGET).cpp
	$(CC) $(CFLAGS) -o $(TARGET) $(INCLUDE) $(TARGET).cpp $(LDLIBS)

clean:
	$(RM) $(TARGET)
