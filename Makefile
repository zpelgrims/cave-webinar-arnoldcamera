# differentiate between linux/osx
OS := $(shell uname)
ifeq ($(OS), Darwin)
	CXX=clang++
	SHARED_LIB_FORMAT=dylib
	SYSTEM=osx
endif
ifeq ($(OS), Linux)
	CXX=g++
	SHARED_LIB_FORMAT=so
	SYSTEM=linux
endif

# where to find the ARNOLD SDK files, in this case in the parent directory
ifeq ($(OS), Darwin)
	ARNOLD_PATH_5.3.0.2=../Arnold-5.3.0.2-darwin
	ARNOLD_PATH_6.0.0.0=../Arnold-6.0.0.0-darwin
endif
ifeq ($(OS), Linux)
	ARNOLD_PATH_5.3.0.2=../Arnold-5.3.0.2-linux
	ARNOLD_PATH_6.0.0.0=../Arnold-6.0.0.0-linux
endif


# c++ option flags, these are really important!
CXXFLAGS=\
	-std=c++17\
	-shared\
	-fPIC\
	-O3

# create the directory structure
$(shell mkdir -p bin bin/5.3.0.2 bin/6.0.0.0)
	
# specify all the targets
all: cave_camera_5.3.0.2\
	 cave_camera_6.0.0.0
	

cave_camera_5.3.0.2: Makefile src/cave_camera.cpp
	${CXX} ${CXXFLAGS} -I${ARNOLD_PATH_5.3.0.2}/include src/cave_camera.cpp -o bin/5.3.0.2/cave_camera.${SHARED_LIB_FORMAT} -L${ARNOLD_PATH_5.3.0.2}/bin -lai 

cave_camera_6.0.0.0: Makefile src/cave_camera.cpp
	${CXX} ${CXXFLAGS} -I${ARNOLD_PATH_6.0.0.0}/include src/cave_camera.cpp -o bin/6.0.0.0/cave_camera.${SHARED_LIB_FORMAT} -L${ARNOLD_PATH_6.0.0.0}/bin -lai 

