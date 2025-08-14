#CXX=c++
#CGAL_DIR = ./CGAL-6.0.1
#EIGEN_DIR = /usr/include/eigen3
#BOOST_DIR = ./boost_1_78_0

#.PHONY: all dependencies clean

#all: compute-bounding-box CGAL-boolean-script CGAL-boolean-script.exe

#compute-bounding-box: bounding-onion.cpp
#	$(CXX) -O3 bounding-onion.cpp -DCGAL_EIGEN3_ENABLED \
#	-I/usr/include/eigen3/ -ICGAL-5.4/include/ -Iboost_1_78_0/ \
#	-lgmp -lmpfr -o compute-bounding-box

#compute-bounding-box.exe: bounding-onion.cpp
#	x86_64-w64-mingw32-c++-posix -O3 -static-libgcc -static-libstdc++ bounding-onion.cpp \
#	-DCGAL_EIGEN3_ENABLED -I/usr/include/eigen3/ -ICGAL-5.4/include/ -Iboost_1_78_0/ \
#	-Lauxiliary/gmp/lib/ -lgmp-10 -Wl,-Bstatic -lstdc++ -lpthread -o compute-bounding-box

#CGAL-boolean-script: CGAL-boolean-script.cpp
#	$(CXX) -O3 -std=c++17 CGAL-boolean-script.cpp \
#	-I$(CGAL_DIR)/include -I$(EIGEN_DIR) -I$(BOOST_DIR) \
#	-DCGAL_EIGEN3_ENABLED \
#	-lgmp -lmpfr -o CGAL-boolean-script

#CGAL-boolean-script.exe: CGAL-boolean-script.cpp
#	-rm -f CGAL-boolean-script.o CGAL-boolean-script.exe
#	x86_64-w64-mingw32-c++-posix -O3 -std=c++17 -static-libgcc -static-libstdc++ CGAL-boolean-script.cpp \
	-I. -I$(CGAL_DIR)/include -I$(EIGEN_DIR) -I$(BOOST_DIR) -DCGAL_EIGEN3_ENABLED \
	-lgmp -lmpfr -Wl,-Bstatic -lstdc++ -lpthread -o CGAL-boolean-script.exe

#CGAL-5.4.tar.xz:
#	curl -OL https://github.com/CGAL/cgal/releases/download/v5.4/CGAL-5.4.tar.xz

#boost_1_78_0.tar.bz2:
#	curl -OL https://boostorg.jfrog.io/artifactory/main/release/1.78.0/source/boost_1_78_0.tar.bz2

#CGAL-5.4-win64-auxiliary-libraries-gmp-mpfr.zip:
#	curl -OL https://github.com/CGAL/cgal/releases/download/v5.4/CGAL-5.4-win64-auxiliary-libraries-gmp-mpfr.zip

#gmp-6.2.1.tar.xz:
#	curl -OL https://gmplib.org/download/gmp/gmp-6.2.1.tar.xz

#dependencies: CGAL-5.4.tar.xz boost_1_78_0.tar.bz2 CGAL-5.4-win64-auxiliary-libraries-gmp-mpfr.zip
#	-rm -r CGAL-5.4 boost_1_78_0 auxiliary
#	tar -xf CGAL-5.4.tar.xz
#	tar -xf boost_1_78_0.tar.bz2
#	unzip CGAL-5.4-win64-auxiliary-libraries-gmp-mpfr.zip

#clean:
#	-rm compute-bounding-box compute-bounding-box.exe CGAL-boolean-script CGAL-boolean-script.exe

CXX=x86_64-w64-mingw32-c++

.PHONY: all dependencies clean

all: CGAL-boolean-script.exe

CGAL-boolean-script.exe: CGAL-boolean-script.cpp
	$(CXX) -O3 -static-libgcc -static-libstdc++ $^ -ICGAL-5.4/include/ -Iboost_1_78_0/ -Lauxiliary/gmp/lib/ -lgmp-10 -o $@ -Wl,-Bstatic -lstdc++ -lpthread

CGAL-5.4.tar.xz:
	curl -OL https://github.com/CGAL/cgal/releases/download/v5.4/CGAL-5.4.tar.xz

boost_1_78_0.tar.bz2:
	curl -OL https://archives.boost.io/release/1.78.0/source/boost_1_78_0.tar.bz2

CGAL-5.4-win64-auxiliary-libraries-gmp-mpfr.zip:
	curl -OL https://github.com/CGAL/cgal/releases/download/v5.4/CGAL-5.4-win64-auxiliary-libraries-gmp-mpfr.zip

gmp-6.2.1.tar.xz:
	curl -OL https://gmplib.org/download/gmp/gmp-6.2.1.tar.xz

dependencies: CGAL-5.4.tar.xz boost_1_78_0.tar.bz2 CGAL-5.4-win64-auxiliary-libraries-gmp-mpfr.zip
	-rm -r CGAL-5.4 boost_1_78_0 auxillary
	tar -xf CGAL-5.4.tar.xz
	tar -xf boost_1_78_0.tar.bz2
	unzip CGAL-5.4-win64-auxiliary-libraries-gmp-mpfr.zip

clean:
	-rm compute-bounding-box compute-bounding-box.exe
