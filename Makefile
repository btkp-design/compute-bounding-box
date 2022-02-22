CXX=c++

.PHONY: all dependencies clean

all: compute-bounding-box

compute-bounding-box: bounding-onion.cpp
	c++ -O3 bounding-onion.cpp -DCGAL_EIGEN3_ENABLED -I/usr/include/eigen3/ -ICGAL-5.4/include/ -Iboost_1_78_0/ -lgmp -o compute-bounding-box

compute-bounding-box.exe: bounding-onion.cpp
	x86_64-w64-mingw32-c++ -O3 -static-libgcc -static-libstdc++ bounding-onion.cpp -DCGAL_EIGEN3_ENABLED -I/usr/include/eigen3/ -ICGAL-5.4/include/ -Iboost_1_78_0/ -Lauxiliary/gmp/lib/ -lgmp-10 -o compute-bounding-box -Wl,-Bstatic -lstdc++ -lpthread

CGAL-5.4.tar.xz:
	curl -OL https://github.com/CGAL/cgal/releases/download/v5.4/CGAL-5.4.tar.xz

boost_1_78_0.tar.bz2:
	curl -OL https://boostorg.jfrog.io/artifactory/main/release/1.78.0/source/boost_1_78_0.tar.bz2

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
