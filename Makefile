demo: main.o function_implementation.o
	g++ -o demo main.o function_implementation.o  -lglfw3 -lGLU -Wno-write-strings  -framework OpenGL -framework GLUT -framework Cocoa -framework IOKit -framework CoreVideo                   

main.o: main.cpp header.h
	g++ -c main.cpp

function_implementation.o: function_implementation.cpp header.h
	g++ -c function_implementation.cpp

clean:
	rm *.o demo *~

