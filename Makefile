CXX = g++
CXXFLAGS = -Wall -Wextra

TARGET = autograd
DOT = graph.dot
SVG = graph.svg

all: $(SVG)

$(TARGET): autograd.cpp
	$(CXX) $(CXXFLAGS) autograd.cpp -o $(TARGET)

$(DOT): $(TARGET)
	./$(TARGET)

$(SVG): $(DOT)
	dot -Tsvg $(DOT) -o $(SVG)

clean:
	rm -f $(TARGET) $(DOT) $(SVG)