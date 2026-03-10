CXX=x86_64-w64-mingw32-c++
CFLAGS=-O3
EIGEN_DIR = /usr/include/eigen3

.PHONY: all dependencies clean

all: compute-bounding-box.exe

compute-bounding-box: bounding-onion.cpp
	$(CXX) $(CFLAGS) -ICGAL-6.0.3/include/ -Iboost_1_83_0/ -DCGAL_DO_NOT_USE_BOOST_MP -DCGAL_EIGEN3_ENABLED bounding-onion.cpp \
	-I$(EIGEN_DIR) \
	-o $@

compute-bounding-box.exe: compute-bounding-box
	cp $^ $@

CGAL-6.0.3.tar.xz:
	curl -OL https://github.com/CGAL/cgal/releases/download/v6.0.3/CGAL-6.0.3.tar.xz

boost_1_83_0.tar.bz2:
	curl -OL https://archives.boost.io/release/1.83.0/source/boost_1_83_0.tar.bz2

dependencies: CGAL-6.0.3.tar.xz boost_1_83_0.tar.bz2
	-rm -r CGAL-6.0.3 boost_1_83_0
	tar -xf CGAL-6.0.3.tar.xz
	tar -xf boost_1_83_0.tar.bz2

clean:
	-rm compute-bounding-box compute-bounding-box.exe
