DESCRIPTION:
This is a solution that reads all the .png files from a folder and makes an atlas from them
This project uses libpng as I did not want to use a big complex library with a lot of dependencies 
The project only reads only .png (and .PNG) files as the requirement was either .png or .jpeg

ISSUES:
* The dirent header that I use to search the directory content mangles the filename if it contains spaces
Most companies have naming conventions so I did not think this was an issue
* We can only read RGB and RGBA color types and output RGB atlas.png file as I did not implement other color types

DEPENDENCIES:
libpng (libraries for windows included)
dirent (header for windows included, and for linux it is standard header)


BUILD ON UBUNTU:
1. install libpng with:
	sudo apt-get install libpng-dev

2. install cmake if you want to generate makefiles
	sudo apt-get install cmake
2.1 run cmake in root folder (right_atlas)
	cmake .

3. compile the project by running make in the root folder
	make

4. go to "source" folder and run program ./RightAtlas <filepath>
	"images" folder is good example 

BUILD ON WINDOWS:
1. install cmake if you want to generate VS solutions
1.2 open cmake and find the root folder 
1.3 generate the project files

2. open vs solution (I used vs 2010)

3. compile the RightAtlas project into executable

4. run RightAtlas.exe <filepath>
	"images folder is a good example"
