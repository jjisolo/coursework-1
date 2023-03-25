OBJ=source/Main.cxx source/Window.cxx source/rendering/RenderScene.cxx source/rendering/ShaderManager.cxx source/rendering/GlUtils.cxx vendor/glad.c

apple:
	clang++ -std=c++20 -stdlib=libc++ -Wno-macro-redefined -Wno-deprecated-volatile -lglfw -framework CoreVideo -framework OpenGL -framework IOKit -framework Cocoa -framework Carbon $(OBJ) -o 101 && mv 101 build 

run:
	build/101