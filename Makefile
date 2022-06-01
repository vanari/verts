CFLAGS = -std=c++17 -O2

LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

TARGET = ./build/verts

SOURCE = ./src/main.cpp

verts: ./src/main.cpp
	g++ $(CFLAGS) -o $(TARGET) $(SOURCE) $(LDFLAGS)

.PHONY: test clean

test: verts
	./build/verts

clean:
	rm -f ./build/verts
