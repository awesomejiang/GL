# 1. GLFW
## download
	http://www.glfw.org/download.html
## dependency
	sudo apt-get install xorg-dev
    sudo apt install libgl1-mesa-dev
## install
	mkdir build
	cd build
	cmake [glfw folder]
	make
	sudo make install

# 2. GLAD
## download
	http://glad.dav1d.de/
	gl: ver3.3+
	Profile: Core
	Generate a loader: ticked
## install
	sudo mv glad/ KHR/ /usr/local/include
	sudo mv glad.c [my project folder]

# 3. stb_image.h
## download
	wget https://github.com/nothings/stb/blob/master/stb_image.h

## install
	sudo mv stb_image.h [my project folder]

# 4. glm
## download
	https://glm.g-truc.net/0.9.9/index.html
## install
	mkdir build
	cd build
	cmake [glm folder]
	make
	sudo make install

# 5. assimp
## download
	https://github.com/assimp/assimp/releases/tag/v4.1.0
## install
	mkdir build
	cd build
	cmake [assimp folder]
	make
	sudo make install