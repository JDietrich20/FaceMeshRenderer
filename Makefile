CXX = g++

CXXFLAGS = -std=c++17 -Wall -g -I./include \
		   -I/Users/juliannadietrich/development/Libraries/glfw-3.4/include	



LIBS = 	-L/Users/juliannadietrich/development/Libraries/glfw-3.4 \
		-lglfw -framework OpenGL 

SRCS = main.cpp 
TARGET = main

all: $(TARGET)

$(TARGET):
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET) $(LIBS)

clean:
	rm -f $(TARGET)
