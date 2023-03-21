OBJ=source/Main.cxx source/Window.cxx vendor/glad.c

apple:
	clang++ -std=c++20 -stdlib=libc++ -lglfw -framework CoreVideo -framework OpenGL -framework IOKit -framework Cocoa -framework Carbon $(OBJ) -o 101 && mv 101 build
