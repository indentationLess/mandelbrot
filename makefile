CXX = g++
CXXFLAGS = -std=c++11
LDFLAGS = -lSDL2
TARGET = mandelbrot
SRC = $(wildcard *.cpp)
OBJ = $(SRC:.cpp=.o)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS)
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
